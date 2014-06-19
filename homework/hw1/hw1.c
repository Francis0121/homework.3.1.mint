/*
Student ID : 20093267
Name : Kim Sung Geun
*/

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

#define TRUE 1
#define FALSE 0

int peertcpSocket = -1;	// peer socket
void display();
int clientConnect(char *hostName, char *newPort, char *userid, fd_set reads, fd_set temps);

int main(int argc, char **argv) {

	int tcpServ_sock,
		fd_max,
		bytesread;// new variable
	
	int finishFlag = FALSE; // Using boolean Value

	struct sockaddr_in		tcpServer_addr,	 // Server 측 Socket
										tcpClient_addr;  // Client 측 정보를 받는 Server 측 Socket
								
	char	command[1024],
			*tcpport = NULL, // Server Port Number
			*userid = NULL; // User Name

	socklen_t client_len; // int type client_len re define
	fd_set	reads,  temps;

	/** 
	* Start Server Socket Create 
	* Socket 생성 
	* -> Socket에 이름연결( Bind )
	* -> Client 연결을 기다림( Listen )
	* -> Client 를 받아드림( Accept )
	* -> Client 명령을 받아서 적절한 Service 수행
	*/
	if(argc != 3){
		printf("Usage : %s <tcpport> <userid>\n", argv[0]);
		exit(1);
	}
	display();

	tcpport = argv[1]; // First Argument 는 TCP Port (such as '10000', '20000')
	userid = argv[2]; //  Seconde Argument는 User Name ( such as 'tom', 'jane')

	// AF_INET : Address Familly - AF_INET refers to addresses from the internet, IP addresses specifically
	// PF_INET : Protocol Family - PF_INET refers to anything in the protocol, usually sockets/ports
	// AF_INET == PF_INET
	if(  (tcpServ_sock = socket(PF_INET, SOCK_STREAM, 0) ) < 0 ){
		perror("Error : [Server Socket] doesn't create\n");
		exit(1);
	}// create

	// Setting Address Structure
	// What mean htonl and htons
	memset( (void *) &tcpServer_addr, 0, sizeof(tcpServer_addr) ); // initiliaze variable tcpServer_addr to 0
	tcpServer_addr.sin_family = PF_INET; 
	tcpServer_addr.sin_addr.s_addr = htonl(INADDR_ANY); // set IP Address
	tcpServer_addr.sin_port = htons( (u_short)atoi(tcpport) ); // set port number

	// Binding Start
	if( bind(tcpServ_sock, (struct sockaddr *) &tcpServer_addr, sizeof(tcpServer_addr) ) < 0 ){ 
		perror("Error : [Server Socket] dosen't bind\n");
		exit(1);
	}// Binding End

	// Wait for Client (Listen Start)
	if( listen(tcpServ_sock, SOMAXCONN) < 0){
		perror("Error : [Server Socket] doesn't listen\n");
		exit(1);
	}// Listen end

	// initialize the select mask variables and set the mask with stdin and the tcp server socket

	// &reads를 fd_set 을 비워두고 socket을 통해 받을것인지, stdin (표준입출력)을 통해 받을것인지 지정
	FD_ZERO(&reads); // FD_ZERO - 모든 소켓 기술자를 세크(read)에서 제거합니다.
	FD_SET(tcpServ_sock, &reads); // FT_SET - 지정된 소켓 기술자(tcpServ_sock)를 세트(read)에 추가한다.
	FD_SET(fileno(stdin), &reads); // 지정된 소켓 기술자(fileno(stdin))를 세트(read)에 추가한다.

	fd_max = tcpServ_sock; // why do this?

	// Why printf 출력시 "\n"이 있어여만 출력이 되는것일까?
	printf("%s> \n", userid); // commad input pause 

	while(TRUE) {

		temps = reads;
		int nfound;
		// Select 함수는 한개 또는 그 이상의 소켓 상태를 결정하고
		// 동기 입출력을 수행 하기위해 필요할 경우 대기하는 함수 이다. 
		// [ arg 1 : nfds ] - I/O 변화를 감지할 총 소켓의 갯수 +1 의 값을 지정하는 용도
		// [ arg 2 : readfds ] - 읽기 상태의 변화를 감지하는 소켓을 지정
		// [ arg 3 : writefds ] - 쓰기 상태의 변화를 감지하는 소켓을 지정
		// [ arg 4 : exceptfds ] - 예외 상태의 변화를 감지하는 소켓을 지정
		// [ arg 5 : timeout ] - Select 함수가 기다리기 위한 시간, NULL 인경우 I/O 변화가 발생될때까지 기다림.
		// [ STEP 1 ] - 여기서 요청을 기다림.
		nfound = select(fd_max+1, &temps, 0, 0, NULL);

		// return the number of file descriptors contained in the three returned descriptor
		// which may be zero if the timeout expires before anything interesting happens
		// On error, -1 is returned, and errno is set appropriately
		if (nfound < 0) { // Select 시 위처럼 반환 값을 가지기 때문에 0이하인 경우만 체크한다. 
			if (errno == EINTR) { // errno 가 EINTR 이라면 System Interrupt임으로 다시 시작한다.
				printf("Warn : [Server Socket] Interrupted system call\n");
				continue;
			}
			perror("Error : [Server Socket] select( ) function\n");
			exit(1);
		}

		if(FD_ISSET(fileno(stdin), &temps)) {
			fgets(command, sizeof (command), stdin);
			FD_CLR(fileno(stdin)	 , &temps); // FT_SET - 지정된 소켓 기술자(tcpServ_sock)를 세트(read)에서 제거한다.
			
			int talkFlag = 1,
				pchFlag = TRUE;
			char *pch, *newPort, *hostName,
				command_copy[strlen(command)];

			strcpy(command_copy, command);
			pch = strtok(command_copy, " ");

			// @talk localhost port
			while(pch != NULL){
				if(!pchFlag){
					break;
				}
				switch(talkFlag){
					case 1:
						if(strcmp("@talk", pch) == 0){
							talkFlag++;
						}else if(strcmp("@quit", pch) == 0 || strcmp("@quit ", pch) == 0 || strcmp("@quit\n", pch) == 0){
							pchFlag = FALSE;
							finishFlag = TRUE;
							talkFlag = 0;
							// Server 가 꺼진다는 것을 상대방 소켓 끄고 나도 끄고.
							// TODO : 서버가 꺼진경우 Client 무한 Loop
							if(peertcpSocket != -1){
								FD_CLR(peertcpSocket, &reads);
								if (close(peertcpSocket)) 
									perror("close");
							}
							FD_CLR(tcpServ_sock, &reads);
							if (close(tcpServ_sock)) 
								perror("Error : [Server Socket] close( ) function\n");
						}else{
							talkFlag = 0;
							pchFlag = FALSE;
						}
						break;
					case 2:
						talkFlag++;
						hostName = pch;
						break;
					case 3:
						if(isdigit(pch[0]) != 0 && (strlen(pch) > 0 && strlen(pch) < 7) ){
							talkFlag++;
							newPort = pch;
							pchFlag = FALSE;
						}else{
							talkFlag = 0;
							pchFlag = FALSE;
						}
						break;
					default:
						pchFlag = FALSE;
						break;
				}
				pch = strtok(NULL, " ");
			}

			if(talkFlag){
				finishFlag = clientConnect(hostName, newPort, userid, reads, temps);
			}
			
			if(finishFlag){
				if (close(tcpServ_sock)) 
					perror("Error : [Server Socket] close( ) function\n");
				break;
			}

			char message[strlen(userid)];
			strcpy(message, userid);
			strcat(message, " : ");
			strcat(message, command);

			if (write(peertcpSocket, message, strlen(message)) < 0) {
				perror("Error :  [Server Socket] write( ) function \n");
				exit(1);
			}
			
			printf("%s> \n", userid);			

		}else if(FD_ISSET(tcpServ_sock, &temps)){
			// Accept Start
			client_len = sizeof(tcpClient_addr);
			peertcpSocket = accept(tcpServ_sock, (struct sockaddr *) &tcpClient_addr, &client_len);
			if(peertcpSocket < 0){
				perror("Error : [Server Socket]  Doesn't accept");
				exit(1);
			}// Accept End
			printf("connection from host %s, port %d, socket %d\n", 
					inet_ntoa(tcpClient_addr.sin_addr), ntohs(tcpClient_addr.sin_port), peertcpSocket);

			FD_SET(peertcpSocket, &reads);
			if (peertcpSocket > fd_max)
				fd_max = peertcpSocket;
			FD_CLR(tcpServ_sock, &temps);

		}else if(FD_ISSET(peertcpSocket, &temps)){
			bytesread = read(peertcpSocket, command, sizeof command);
			if (bytesread<0) {
				perror("Error : [Server Socket] read\n");
			}
			if (bytesread <= 0) {
				printf("Connection Closed %d\n", peertcpSocket);
				FD_CLR(peertcpSocket, &temps);
				if (close(peertcpSocket)) 
					perror("Error : [Server Socket] close() function\n");
				// Connectino이 종료되었음으로 Server 초기 상태로 초기화
				FD_ZERO(&reads); 
				FD_SET(tcpServ_sock, &reads);
				FD_SET(fileno(stdin), &reads);
				fd_max = tcpServ_sock;
				peertcpSocket = -1;
				continue;
			}
			command[bytesread] = '\0';
			printf("%s\n", command);
		}
	}//while End
	return 0;
}//main End

void display() {
	printf("Student ID : 20093267\n");
	printf("Name : Kim Sung Geun\n");
}

int clientConnect(char *hostName, char *newPort,  char *userid, fd_set reads, fd_set temps){
	/**
	* Start Client Socket Create
	* Client Socket 생성 
	* -> Server 에 연결 시도( Connect)
	* -> Server에 각종 Command 전달
	*/
	struct hostent *hostp; // new connect server use
	struct sockaddr_in	 newTcp_addr; // Server로 연결하기 위한 Client 측 Socket
	int nfound, bytesread;
	int finishFlag = FALSE;

	int tcpCln_sock;
	static struct timeval timeout = { 5, 0 };  /* 5초 */
	char	command[1024];

	if( (tcpCln_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
		perror("Error : [Client Socket] doesn't create in talk");
		exit(1);
	}

	if( (hostp = gethostbyname(hostName)) == 0){
		perror("Error : [Client Socket] unknown host\n");
		exit(1);
	}

	// Setting Client Address Structure
	memset((void *) &newTcp_addr, 0, sizeof(newTcp_addr));
	newTcp_addr.sin_family = PF_INET;
	memcpy((void *) &newTcp_addr.sin_addr, hostp->h_addr, hostp->h_length);
	newTcp_addr.sin_port = htons((u_short)atoi(newPort));

	if(connect(tcpCln_sock, (struct sockaddr *) &newTcp_addr, sizeof(newTcp_addr)) < 0){
		(void) close(tcpCln_sock);
		printf("Error : [Client Socket] Connect\n");
		exit(1);
	}

	FD_SET(tcpCln_sock, &reads);
	printf("%s> \n", userid);

	while (TRUE) {
		temps = reads;
		nfound = select(FD_SETSIZE, &temps, 0, 0, &timeout);
		if (nfound < 0) {
			if (errno == EINTR) {
				printf("Warn : [Client Socket] Interrupted system call\n");
				continue;
			}
			perror("Error :  [Client Socket] select( ) function\n");
			exit(1);
		}
		
		if (FD_ISSET(fileno(stdin), &temps)) {
			fgets(command, sizeof (command), stdin);
			FD_CLR(fileno(stdin), &temps);
			
			if(strcmp("@quit", command) == 0 || strcmp("@quit ", command) == 0 || strcmp("@quit\n", command) == 0){
				finishFlag = TRUE;
				break;
			}

			printf("%s> \n", userid);

			char message[strlen(userid)];
			strcpy(message, userid);
			strcat(message, " : ");
			strcat(message, command);	

			if (write(tcpCln_sock, message, strlen(message)) < 0) {
				perror("Error : [Client Socket]  write( ) function\n");
				exit(1);
			}
		}
		
		if (FD_ISSET(tcpCln_sock, &temps)) {
			bytesread = read(tcpCln_sock, command, sizeof command );
			command[bytesread] = '\0';
			printf("%s\n", command);
		}
	}

	return finishFlag;
}