//In this assignment, we practice using sockets.
//We implement a network game named jumble.

//A client connnect to a server at the port 3100 to play the game.
//The server will give the client a scrabmled word to guess.
//For example, the server sends the client "ubisq". The client will 
//guess the word is "squib", and sends the guess to the server.

//The server will notify the client wether the guess is correct and 
//ask the client to continue the game.
//The client will keep the score based on the response from the server.

//The server code is already given, and a working client code is also given.
//What we need to do is to make the client smart by implementing
//the function guess_word() that guess the correct word from a scrambled word.
//Once we implement this function correctly, the client will play a perfect
//game. Every time, the client will guess the word correctly.   

//read both the server code and the client code carefully to know how to do the
//implementation

//jumble client

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>
#include <assert.h>

#include <arpa/inet.h>

#define PORT "3100" // the port client will be connecting to 

#define MAX_WORD_COUNT 60000                //we have less than 60000 words
#define MAX_WORD_LENGTH 80                //each word is less than 80 letters

char words[MAX_WORD_COUNT][MAX_WORD_LENGTH];        //2-d array to hold all the words
char words_reset[MAX_WORD_COUNT][MAX_WORD_LENGTH];
int count = 0;                    //number of words, initilized to 0

int char_cmp(const void* a, const void* b) {
    if (*(char*)a < *(char*)b) return -1;
    else if (*(char*)a == *(char*)b) return 0;
    else return 1;
}

void string_reset(char* s) {
    qsort(s, strlen(s), sizeof(char), char_cmp);
}

void read_file_to_array(char* filename) {
    FILE* fp;

    //open the file for reading
    fp = fopen("dict.txt", "r");
    if (fp == NULL) {
        printf("Cannot open the file.\n");
        exit(-1);
    }
    while (!feof(fp)) {
        fscanf(fp, "%s\n", words[count]);
        count++;
    }
    fclose(fp);
}

void reset_all_words() {
    int i;

    for (i = 0; i < count; i++) {
        strncpy(words_reset[i], words[i], MAX_WORD_LENGTH);
        string_reset(words_reset[i]);
    }
}

//Implement the following function
//based on scrambled, wirte a word in guess
void guess_word(const char* scrambled, char* guess) {
    // char word_reset[MAX_WORD_LENGTH];
    
    u_int8_t scrambled_letters[26] = { 0 };
    for (size_t i = 0; scrambled[i] != '\0'; ++i) {
        assert(scrambled[i] >= 'a' && scrambled[i] <= 'z');
        scrambled_letters[scrambled[i] - 'a']++;
    }
    
    // strncpy(word_reset, scrambled, MAX_WORD_LENGTH - 5);
    // string_reset(word_reset);
    for (int i = 0; i < count; i++) {
        //fill in code below
        //find the first word in the dictionary that matches
        uint8_t word_letters[26] = { 0 };
        for (size_t j = 0; words[i][j] != '\0'; ++j) {
            if (words[i][j] == '-') continue; // why is "cross-bun" a word???
            assert(words[i][j] >= 'a' && words[i][j] <= 'z');
            word_letters[words[i][j] - 'a']++;
        }
        if (memcmp(scrambled_letters, word_letters, sizeof(scrambled_letters)) == 0) {
            strncpy(guess, words[i], MAX_WORD_LENGTH);
            return;
        }
    }
}


// get sockaddr, IPv4 or IPv6:
void* get_in_addr(struct sockaddr* sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// wtf is this???
#include "socketio.c"

#define	LINE_SIZE	1024

void print_lines(int sid, char* buf, int n) {
    if (recv_lines(sid, buf, n) == -1) {
        fprintf(stderr, "recv_line returned -1.\n");
        exit(EXIT_FAILURE);
    }
    printf("%s", buf);
}

int main(int argc, char* argv[]) {
    int sockfd;
    char buf[LINE_SIZE];
    struct addrinfo hints, * servinfo, * p;
    int rv;
    char s[INET6_ADDRSTRLEN];
    
    if (argc != 2) {
        fprintf(stderr, "usage: client hostname\n");
        exit(1);
    }
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }
    
    // loop through all the results and connect to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }
        
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            perror("client: connect");
            close(sockfd);
            continue;
        }
        
        break;
    }
    
    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }
    
    inet_ntop(p->ai_family, get_in_addr((struct sockaddr*)p->ai_addr), s, sizeof s);
    printf("client: connecting to %s\n", s);
    
    freeaddrinfo(servinfo); // all done with this structure
    
    read_file_to_array("dict.txt");
    reset_all_words();
    
    char guess[LINE_SIZE];
    char yes_no[LINE_SIZE];
    int done = 0;
    int score = 0;
    
    do {
        //receive and print the scrambled word
        rv = recv_lines(sockfd, buf, LINE_SIZE);
        if (rv < 0) {
            perror("recv");
            exit(-1);
        }
        printf("%s", buf);
        buf[strlen(buf) - 1] = '\0';
        //make a guess and send it to server
        //if we uncomment the following line and comment out the line after
        //the code should work. But it is not a smart client.
        //We need to make the client smart by implementing guess_work()
        //scanf("%s", guess);
        guess_word(buf, guess);
        strcat(guess, "\n"); // NOLINT: strcat is safe here because guess is LINE_SIZE long, and guess is at most WORD_SIZE.
        printf("%s", guess);
        rv = send_str(sockfd, guess);
        if (rv < 0) {
            close(sockfd);
            perror("send");
            exit(-1);
        }
        
        //receive the response from server
        rv = recv_lines(sockfd, buf, LINE_SIZE);
        if (rv < 0) {
            close(sockfd);
            perror("recv");
            exit(-1);
        }
        
        printf("%s", buf);
        if (strstr(buf, "Correct!")) {
            score++;
        }
        //indicate to server whether to continue
        scanf("%10s", yes_no);
        strcat(yes_no, "\n"); // NOLINT: strcat is safe here bc yes_no is <= 10 chars
        rv = send_str(sockfd, yes_no);
        if (rv < 0) {
            close(sockfd);
            perror("send");
            exit(-1);
        }
        
        if (!strcmp(yes_no, "n\n") || !strcmp(yes_no, "N\n")) done = 1;
        printf("Score : %d\n", score);
    } while (!done);
    
    close(sockfd);
    
    return 0;
}

