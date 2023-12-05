/*
 ** server.c -- a stream socket server demo
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>

#define PORT "3119"  // the port users will be connecting to

#define BACKLOG 10	 // how many pending connections queue will hold

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int create_handler(int new_fd, int r);

int main(void)
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                        p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                    sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    srand(3100);

    while(1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
                get_in_addr((struct sockaddr *)&their_addr),
                s, sizeof s);

        // call a function to create a thread to deal with the client 
        int r = rand();
        fprintf(stderr, "server: got connection from %s. random value is %d\n", s, r);
        if (create_handler(new_fd, r) < 0)
            close(new_fd);
    }

    return 0;
}

#define MAX_VALUE       1000
#define MSG_BUF_SIZE    100

typedef  struct {
    int v;
    int num_attempts;
    char message[MSG_BUF_SIZE];
} gmn_t;

void gmn_init(gmn_t *pg, int seed)
{
    // srand(seed);
    // pg->v = rand() % MAX_VALUE + 1;
    pg->v = seed % MAX_VALUE + 1; 
    pg->num_attempts = 0;
    strcpy(pg->message, "You have not got the right number yet.\n");
}

int gmn_check(gmn_t *pg, int guess)
{
    pg->num_attempts ++;
    if (pg->v == guess) {
        snprintf(pg->message, MSG_BUF_SIZE,
                "It took you %d attempt(s) to guess the number %d.\n",
                pg->num_attempts, pg->v);
        return 0;
    }
    if (pg->v > guess)
        return 1;
    return -1;
}

char * gmn_get_message(gmn_t *pg)
{
    return pg->message;
}

int    gmn_get_max()
{
    return MAX_VALUE;
}

/* If you need to add more functions, add them below */

#include "socketio.c"


typedef struct thread_arg_tag {
    int sockfd;
    int random_value;
} thread_arg_t;


void * thread_main(void * arg_in)
{
    thread_arg_t *arg = arg_in;
    int sockfd = arg->sockfd; 

    gmn_t   gmn;

    gmn_init(&gmn, arg->random_value);

    // TODO
    //  send max value
    //  repeat the following until guess is correct
    //      wait for a guess 
    //      call gmn_check()
    //      send the result and, if the guess is correct, send the final message
    //  clean up: close FD and free memory. 
    //  Do clean up on error. See the demo code.

    return NULL;
}

int create_handler(int new_fd, int r) 
{
    pthread_t   tid;
    int     rv;

    thread_arg_t * arg = malloc(sizeof(thread_arg_t));

    if (arg == NULL)
        return -1; 

    arg->sockfd = new_fd;
    arg->random_value = r;

    rv = pthread_create(&tid, NULL, thread_main, arg);
    if (rv) {
        free(arg);
        return -1;
    }

    // cannot do much if it fails
    pthread_detach(tid);
    return 0;
}
