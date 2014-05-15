#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#define TRUE 1
#define FALSE 0

typedef struct {
    struct sockaddr_in addr,hAddr;
    int socket;
    char userId[100];
    int isHost;
} ClientInfo;

char** str_split(char* a_str,  char* a_delim);