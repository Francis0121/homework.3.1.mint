//
//  main.c
//  mint
//
//  Created by 김세훈 on 2014. 5. 15..
//  Copyright (c) 2014년 김세훈. All rights reserved.
//

#include <stdio.h>
#include "server.h"

#define PORTNUM 10000



int main(int argc, const char * argv[])
{
    struct sockaddr_in server,client; // Server 의 Socket addr
    int serverSocket; // 서버 연결용 소켓
    
    numOfClient=0;
    
	char buf[1024];
	fd_set temps, reads;
	
    int maxfd;
    long byteread;
    
    int i;
    
    socklen_t client_len;
    
//    printf("%d",user[0].hAddr.sin_addr);
   
    
    
	printf("Student ID : 20093267\n");
	printf("Name : Sung Geun Kim\n");
    
    
	/**
     *	Make Server
     *	Create(Srever)-> Bind(Server) -> Listen(From Client) ->
     *	Accept(From Client) -> Work(Server)
     */
	// Create(Server)
	if(  (serverSocket = socket(PF_INET, SOCK_STREAM, 0) ) < 0 ){
		perror("Error : [Server Socket] doesn't create\n");
		exit(1);
	}// End Create
    
    
	// Setting Address Structure
	memset( (void *) &server, 0, sizeof(server) ); // initiliaze variable tcpServer_addr to 0
	server.sin_family = PF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY); // set IP Address
	server.sin_port = htons( PORTNUM ); // set port number
    
    
	// Bind(Server) Start
	if( bind(serverSocket, (struct sockaddr *) &server, sizeof(server) ) < 0 ){
		perror("Error : [Server Socket] dosen't bind\n");
		exit(1);
	}// Bind(Server) End
    
    
	// Wait Listen(From Client)
	if( listen(serverSocket, SOMAXCONN) < 0){
		perror("Error : [Server Socket] doesn't listen\n");
		exit(1);
	}// Wait Listen(From Client) End
    
    
	FD_ZERO(&reads); // FD_ZERO - 모든 소켓 기술자를 세크(read)에서 제거합니다.
	FD_SET(serverSocket, &reads); // FT_SET - 지정된 소켓 기술자(tcpServ_sock)를 세트(read)에 추가한다.
	
    
	maxfd = serverSocket;
    
    
    while(TRUE)
    {
        temps = reads;
        select(maxfd+1, &temps, 0, 0, NULL);
        
        if(FD_ISSET(serverSocket, &temps))
        {
            client_len = sizeof(client);
			user[numOfClient].socket = accept(serverSocket, (struct sockaddr *) &user[numOfClient].addr, &client_len);
			if(user[numOfClient].socket < 0){
				perror("Error : [Server Socket]  Doesn't accept");
				exit(1);
			}// Accept (From Client) end
			printf("Connection : Host IP %s, Port %d, socket %d\n",
                   inet_ntoa(client.sin_addr),
                   ntohs(client.sin_port),
                   user[numOfClient].socket);
            
            
			FD_SET(user[numOfClient].socket, &reads);
			if (user[numOfClient].socket > maxfd)
                maxfd = user[numOfClient].socket;
			FD_CLR(serverSocket, &temps);
            numOfClient++;
        }
        
        for(i=0; i<numOfClient; i++)
        {
            if(FD_ISSET(user[i].socket, &temps))
            {
            
                byteread=read(user[i].socket, buf , sizeof (buf) -1 );
                
                if (byteread<0) {
					perror("Error : [Server Socket] read\n");
				}
				if (byteread <= 0) {
					//printf("Server: end of file on %d\n",fd);
					FD_CLR(user[i].socket, &reads);
					if (close(user[i].socket)) perror("close");
                    continue;
				}
				buf[byteread] = '\0';
				printf("%s\n", buf);
                
                tokenize(buf,i);
                
                
                
            }
            
        }
        
        
    }
}

    
    
    
    
    


