/*
 ** client.c -- a stream socket client demo
 */

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

#include <arpa/inet.h>

#define PORT "3119" // the port client will be connecting to 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

#include "socketio.c"

#define LINE_SIZE 100

void die(char *s)
{
    if (errno)
        perror(s);
    else 
        fprintf(stderr, "Error: %s\n", s);
    exit(EXIT_FAILURE);
}

void print_line(int sid, char * buf, int n)
{
    if (recv_lines(sid, buf, n)) {
        fprintf(stderr, "recv_lines() returned -1.\n");
        exit(EXIT_FAILURE);
    }
    printf("%s", buf);
}

int send_int(int sid, int v)
{
    char buf[64];
    int n;

    n = snprintf(buf, sizeof(buf), "%d\n", v);
    return send_all(sid, buf, n);
}

int get_number(char *s, int *pv)
{
    if (sscanf(s, "%d", pv) != 1) // Removed '\n' from format
        return -1;
    return 0;
}

int main(int argc, char *argv[])
{
    int sockfd;
    char buf[LINE_SIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
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
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                        p->ai_protocol)) == -1) {
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

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure

    int min = 1, max, guess;
    int result = 0;

    /* TODO
     *
     *  get max
     *  repeat the following until result is 0
     *      calculate guess
     *      send guess 
     *      receive result
     *      adjust min or max if necessary
     *  print the final message, which may or may not be received with "0\n"
     *  */

    // Receive the maximum value from the server
    if (recv_lines(sockfd, buf, sizeof(buf)) == -1) {
        die("Error receiving max value from server");
    }

    if (get_number(buf, &max) == -1) {
        die("Error parsing max value");
    }

    printf("%d\n", max);

    // Binary search loop
    while (1) {
        guess = (min + max) / 2;
        printf("My guess: %d\n", guess);

        // Send the guess to the server
        if (send_int(sockfd, guess) == -1) {
            die("Error sending guess to server");
        }

        // Receive the result from the server
        if (recv_lines(sockfd, buf, sizeof(buf)) == -1) {
            die("Error receiving result from server");
        }

        // Extract the result code and any additional message
        char *message = NULL;
        result = strtol(buf, &message, 10);

        printf("%d\n", result);

        if (result == 0) {
            // Correct guess, check if there's a final message
            if (*message != '\0') {
                // Skip newline character if present
                if (*message == '\n') {
                    message++;
                }
                printf("%s", message);
            }
            break;
        } else if (result == 1) {
            // Guess is too small
            min = guess + 1;
        } else if (result == -1) {
            // Guess is too large
            max = guess - 1;
        } else {
            die("Received invalid response from server");
        }
    }

    close(sockfd);
    return 0;
}
