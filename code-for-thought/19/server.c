//We use multi-threads to handle requests from clients
//We implement a jumble game server using threads
//Each thread can handle one jumble game client

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
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

#define PORT "3100"  // the port users will be connecting to

#define BACKLOG 10	 // how many pending connections queue will hold

#define MAXDATASIZE 100

#define MAX_WORD_COUNT 60000                //we have less than 60000 words
#define MAX_WORD_LENGTH 80                //each word is less than 80 letters

char words[MAX_WORD_COUNT][MAX_WORD_LENGTH];        //2-d array to hold all the words
int count = 0;                    //number of words, initilized to 0

void read_file_to_array(char *filename)
{
    FILE *fp;

    //open the file for reading
    fp = fopen("dict.txt", "r");
    if(fp==NULL)
    {
        printf("Cannot open the file.\n");
        exit(-1);
    }
    while(!feof(fp))
    {
        fscanf(fp, "%s\n", words[count]);
        count ++;
    }
    fclose(fp);
}

int choose_k(int count)
{
    return rand() % count;
}

//scramble a word
void scramble(const char *word, char *scrambled)
{
    strcpy(scrambled, word);
    for(int i=strlen(word); i>=1; i--)
    {
        int k = rand() % i;
        char ch = scrambled[k];
        scrambled[k] = scrambled[i-1];
        scrambled[i-1] = ch;
    }
}

//compare function used for qsort()
int char_cmp(const void *a, const void *b)
{
    if(*(char *)a < *(char *)b) return -1;
    else if(*(char *)a == *(char *)b) return 0;
    else return 1;
}

//sort a string
void string_reset(char *s)
{
    qsort(s, strlen(s), sizeof(char), char_cmp);
}

//test wether a word is in the array words
int is_word(char *word)
{
    //fill in the code below
    //return 1 if word is in the array words
    //otherwise, return 0
    for(int i = 0; i< count; i++)
    {
        if(!strcmp(words[i], word)) return 1;
    }
    return 0;
}

//check whether answer is correct
int correct_answer(char *word, char *answer)
{
    char word_reset[MAX_WORD_LENGTH];
    char answer_reset[MAX_WORD_LENGTH];

    strcpy(word_reset, word);
    strcpy(answer_reset, answer);

    qsort(word_reset, strlen(word_reset), sizeof(char), char_cmp);
    qsort(answer_reset, strlen(answer_reset), sizeof(char), char_cmp);
    return !strcmp(word_reset, answer_reset) && is_word(answer);
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int create_handler(int new_fd);

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

    read_file_to_array("dict.txt");

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
        fprintf(stderr, "server: got connection from %s. fd is %d.\n", s, new_fd);
        if (create_handler(new_fd)) {
            close(new_fd);
        }
    }

    return 0;
}

#include "socketio.c"

#define BUF_SIZE    1024

// Use a structure, although we hvae only one field
// It is easier to add more fields in other programs
typedef struct thread_arg_tag {
    int sockfd;
} thread_arg_t;

void thread_die(thread_arg_t *arg)
{
    printf("thread using socket fd %d exiting ...\n", arg->sockfd);
    close(arg->sockfd);
    free(arg);
    pthread_exit(NULL);
}

/* The main function of the thread handling the client.
 * */
void * thread_main(void * arg_in)
{
    thread_arg_t *arg = arg_in;
    int sockfd = arg->sockfd; 
    int rv;

    char *word;
    char *correct = "Correct! Continue?\n";
    char scrambled[MAX_WORD_LENGTH];

    char answer[BUF_SIZE];
    char incorrect[BUF_SIZE];
    int done = 0;
    do {
        //choose a word
        word = words[choose_k(count)];
        printf("word : %s\n", word);
        //scramble the word
        scramble(word, scrambled);
        //sent the scrabled word to client
		strcat(scrambled, "\n");
    	rv = send_str(sockfd, scrambled);
    	if (rv < 0) thread_die(arg);
  
        printf("scrambled word sent.\n");
        //receive answer from client

        rv = recv_lines(sockfd, answer, BUF_SIZE);
        if (rv < 0) thread_die(arg);

		//remove the new line at the end
		answer[strlen(answer)-1] = '\0';
     	printf("received answer: %s\n", answer);
        //check if the answer is correct
        if(correct_answer(word, answer))
        {
        	rv = send_str(sockfd, correct);
			if (rv < 0) thread_die(arg);
            printf("correct.\n");
        }
        else
        {
            sprintf(incorrect, "Incorrect. The word is %s. Continue?\n", word);
            rv = send_str(sockfd, incorrect);
            if (rv < 0) thread_die(arg);
            printf("%s", incorrect);
        }
        //receive response from client
        rv = recv_lines(sockfd, answer, BUF_SIZE);
        if (rv < 0) thread_die(arg);

        if(!strcmp(answer, "n\n") || !strcmp(answer, "N\n")) done = 1;
    }while(!done);
	thread_die(arg);
	return NULL;
}

/* create a thread to talk to the client.
 *
 * Return values:
 * 0:   success
 * -1:  error. new_fd is not closed.
 * */
int create_handler(int new_fd) 
{
    pthread_t   tid;
    int     rv;

    thread_arg_t * arg = malloc(sizeof(thread_arg_t));

    if (arg == NULL)
        return -1; 

    arg->sockfd = new_fd;

    rv = pthread_create(&tid, NULL, thread_main, arg);
    if (rv) {
        free(arg);
        return -1;
    }

    // not much we can do if it fails. could just kill the process.
    pthread_detach(tid);
    return 0;
}
