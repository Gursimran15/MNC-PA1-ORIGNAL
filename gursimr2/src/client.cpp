/**
* @client
* @author  Swetank Kumar Saha <swetankk@buffalo.edu>, Shivang Aggarwal <shivanga@buffalo.edu>
* @version 1.0
*
* @section LICENSE
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 2 of
* the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details at
* http://www.gnu.org/copyleft/gpl.html
*
* @section DESCRIPTION
*
* This file contains the client.
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include "../include/logger.h"
#include <iostream>
#include <unistd.h>

using namespace std;

#define TRUE 1
#define MSG_SIZE 256
#define BUFFER_SIZE 256
#define STDIN 0
#define CMD_SIZE 100
int connect_to_host(char *server_port);
char* findipc(){
	int r;
	int udp_socket,udp_connect;
	struct addrinfo hints, *res;
	struct sockaddr_in sa;
	int sa_length;
	char ip[16];
	sa_length=sizeof(sa);
	memset(&hints, 0, sizeof(hints));
    	hints.ai_family = AF_INET;
    	hints.ai_socktype = SOCK_DGRAM;
	if (getaddrinfo("8.8.8.8","53", &hints, &res) != 0)
		perror("getaddrinfo failed");

	udp_socket = socket(res->ai_family, res->ai_socktype,0);
	if(udp_socket < 0)
		perror("Cannot create socket");
	udp_connect = connect(udp_socket, res->ai_addr, res->ai_addrlen);
	if(udp_connect < 0)
		perror("Connect failed");
	r=getsockname(udp_socket,(sockaddr *)&sa,(socklen_t *)&sa_length);
	inet_ntop(AF_INET, &sa.sin_addr, ip, sizeof(ip));
	close(udp_socket);
	return ip;
}

/**
* main function
*
* @param  argc Number of arguments
* @param  argv The argument list
* @return 0 EXIT_SUCCESS
*/
struct lists {
int list_id;
char hostname[256];
char ipaddr[INET_ADDRSTRLEN];
int port_no;
};
int sendall(int s, char *buf, int *len)
{
    int total = 0;        // how many bytes we've sent
    int bytesleft = *len; // how many we have left to send
    int n;

    while(total < *len) {
        n = send(s, buf+total, bytesleft, 0);
        if (n == -1) { break; }
        total += n;
        bytesleft -= n;
    }

    *len = total; // return number actually sent here

    return n==-1?-1:0; // return -1 on failure, 0 on success
} 
int client(char *port)
{
	// if(c != 3) {
	// 	printf("Usage:%s [ip] [port]\n", arg[0]);
	// 	exit(-1);
	// }
	printf("%s",port);
	int server;
	server = connect_to_host(port);
	
	
	return 0;
}

int connect_to_host(char* port)
{
	int fdsocket;
	struct addrinfo hints, *res, *p;
	int head_socket, selret, sock_index, fdconnect=0;
	// , caddr_len
	// struct sockaddr_in client_addr;
	fd_set master_list, watch_list;
	//  int size = 30; 
  
    // // can allocate size dynamically. 
    // char* str2 = (char*)malloc(sizeof(char) * size); 
	char listall[1024];
	char **list = (char **)malloc(4 * sizeof(char *)); 
    for (int i=0; i<4; i++) 
         list[i] = (char *)malloc(1024 * sizeof(char)); 

	// /* Set up hints structure */	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
    // hints.ai_flags = AI_PASSIVE;
	/* Fill up address structures */	
	// if (getaddrinfo(NULL, port, &hints, &res) != 0)
	// 	perror("getaddrinfo failed");
	
	char *ip=new char[INET_ADDRSTRLEN];  
	FD_ZERO(&master_list);
	FD_ZERO(&watch_list);
	
	/* Register the listening socket */
	// FD_SET(client_socket, &master_list);
	/* Register STDIN */
	FD_SET(0, &master_list);
	
	head_socket=0;
	lists l[4];
	int ipp_index=-1;
	int login_flag=0;

	while(TRUE){
		memcpy(&watch_list, &master_list, sizeof(master_list));
		
		//printf("\n[PA1-Server@CSE489/589]$ ");
		//fflush(stdout);
		
		/* select() system call. This will BLOCK */
		selret = select(head_socket+1, &watch_list, NULL, NULL, NULL);
		if(selret < 0)
			perror("select failed.");
		printf("1:%d\n",head_socket);
		/* Check if we have sockets/STDIN to process */
		if(selret > 0){
				printf("4:%d\n",head_socket);
			/* Loop through socket descriptors to check which ones are ready */
			for(sock_index=0; sock_index<=head_socket; sock_index+=1){
				printf("2:%d\n",sock_index);
				if(FD_ISSET(sock_index, &watch_list)){
					printf("3:%d\n",sock_index);
					/* Check if new command on STDIN */
					if (sock_index == STDIN){
						printf("5:%d\n",sock_index);
						char *cmd = (char*) malloc(sizeof(char)*CMD_SIZE);
						memset(cmd, '\0', CMD_SIZE);
						if(fgets(cmd, CMD_SIZE-1, stdin) == NULL) //Mind the newline character that will be written to cmd
							exit(-1);
							printf("6:%d%s\n",sock_index,cmd);
							int s;
							if(strcmp(cmd,"AUTHOR\n")==0){
								s=0;
							}
							if(strcmp(cmd,"PORT\n")==0){
								s=1;
							}
							if(strcmp(cmd,"IP\n")==0){
								s=2;
							}
							char *strforlogin=cmd;
							char *args[3];
							int j=0;
							while(args[j++]=strtok_r(strforlogin, " \n", &strforlogin));
							if(strcmp(args[0],"LOGIN")==0){
								s=3;
							}
							if(strcmp(cmd,"LIST")==0){
								s=4;
							}
							if(strcmp(args[0],"SEND")==0){
								s=5;
							}
							printf("7:%d%s\n",sock_index,cmd);
							// if(s>3){
							// 	printf("You must Login first!");
							// }
							for(int j=0;j<=ipp_index;j++){
								printf("%s\n",list[j]);
							}
						switch(s){
							case 0:{
									if(strcmp(cmd,"AUTHOR")==0){
								cse4589_print_and_log("[%s:SUCCESS]\n", "AUTHOR");
								cse4589_print_and_log("I, %s, have read and understood the course academic integrity policy.\n", "gursimr2");
								cse4589_print_and_log("[%s:END]\n", "AUTHOR");
								}
								else{
									cse4589_print_and_log("[%s:ERROR]\n", "AUTHOR");
									cse4589_print_and_log("[%s:END]\n", "AUTHOR");
								}
								break;
								}
							case 1: {
									if(strcmp(cmd,"PORT")==0){
									cse4589_print_and_log("[%s:SUCCESS]\n", "PORT");
									cse4589_print_and_log("PORT:%s\n",port);
									cse4589_print_and_log("[%s:END]\n", "PORT");
									}
									else{
										cse4589_print_and_log("[%s:ERROR]\n", "PORT");
										cse4589_print_and_log("[%s:END]\n", "PORT");
									}
									break;
								}
							case 2:{ 
									printf("8:%d%s\n",sock_index,cmd);
									// inet_ntop(AF_INET,h->h_addr_list[0],ip, sizeof(ip));
									if(strcmp(cmd,"IP")==0){
									cse4589_print_and_log("[%s:SUCCESS]\n", "IP");
									ip=findipc();
									cse4589_print_and_log("IP:%s\n",ip);
									cse4589_print_and_log("[%s:END]\n", "IP");
									}
									else{
										cse4589_print_and_log("[%s:ERROR]\n", "IP");
										cse4589_print_and_log("[%s:END]\n", "IP");
									}
									break;
									}
							case 3: {
									if(strcmp(args[0],"LOGIN")==0){
									
									printf("l1:%d%s\n",sock_index,cmd);
									// if(sock_index == client_socket){
										// hostent *h = gethostbyaddr((struct sockaddr*)args[1],sizeof(struct in_addr),AF_INET);
									if (getaddrinfo(args[1], args[2], &hints, &p) != 0)
										perror("getaddrinfo failed");
										/* Socket */
										printf("l2:%d%s\n",sock_index,cmd);
									fdsocket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
									if(fdsocket < 0)
										perror("Failed to create socket");
									// caddr_len = sizeof(client_addr);
									fdconnect = connect(fdsocket, p->ai_addr, p->ai_addrlen);
									// fdaccept = accept(server_socket, (struct sockaddr *)&client_addr, &caddr_len);
									if(fdconnect < 0)
										perror("Connect failed");
									
									printf("\nRemote Server Connected!\n");   
									printf("l3:%d%s\n",sock_index,cmd);
									login_flag=1;                     
									char *msg = (char*) malloc(sizeof(char)*MSG_SIZE);
									memset(msg, '\0', MSG_SIZE);
									char *ip2 = new char[16];
									ip2=findipc();
									strcpy(msg,"LOGIN ");
									strcat(msg,ip2);
									strcat(msg," ");
									strcat(msg,port);
									//  strcat(msg,"\n");
									printf("l4:%d%s\n",sock_index,cmd);
									printf("\nSENDing it to the remote server ... %s",msg);
									if(send(fdsocket, msg, strlen(msg), 0) == strlen(msg))
										printf("Done!\n");
									fflush(stdout);
									printf("l5:%d%s\n",sock_index,cmd);
									FD_SET(fdsocket, &master_list);
									if(fdsocket > head_socket)
										 head_socket = fdsocket;
									cse4589_print_and_log("[%s:SUCCESS]\n", args[0]);
									cse4589_print_and_log("[%s:END]\n", args[0]);
									// cout<<cmd<<"\n";
									printf("l6:%d%s\n",sock_index,cmd);
									// if(recv(sock_index,buffer, BUFFER_SIZE, 0) <= 0){ //changed sock_index to fdconnect
									// close(sock_index); //same as above
									// printf("Remote Server terminated connection!\n");
									// }
									printf("l7:%d%s\n",sock_index,cmd);
									// char *strlist=buffer;
									// int i=ipp_index+1;
									// while(list[i++]=strtok_r(strlist, ",", &strlist));
									// ipp_index=i;
									// printf("%s",buffer);
									// printf("ECHOing it back to the remote host ... ");
									// if(send(fdconnect, buffer, strlen(buffer), 0) == strlen(buffer))
									// 	printf("Done!\n");
								
									}
									else{
										cse4589_print_and_log("[%s:ERROR]\n",args[0]);
										cse4589_print_and_log("[%s:END]\n",args[0]);
									}
									break;
									}
							case 4: 
									printf("For List %s %d\n",list[0],ipp_index);
									if(login_flag == 1){
										printf("For List %s %d\n",list[0],ipp_index);
										for(int l=0;l<=ipp_index;l++){
									// printf("I am here in List:%s\n",list[0]);
									// cse4589_print_and_log("%s\n",list[l]);
										// strcat(temp,list[l]);
										// strcat(temp, "\n");
										}
									if(strcmp(cmd,"LIST")==0){
									
									// for(int j=0;j<=ipp_index;j++){
									// printf("%s\n",list[j]);
									// 	}
									// char temp[1024];
									// strcpy(temp, " ");
									// for(int l=0;l<=ipp_index;l++){
									// // printf("I am here in List:%s\n",list[0]);
									// cse4589_print_and_log("%s\n",list[l]);
									// 	// strcat(temp,list[l]);
									// 	// strcat(temp, "\n");
									// 	}
										// printf("%s\n",temp);
									//Problem here
									cse4589_print_and_log("[%s:SUCCESS]\n", "LIST");
									// cse4589_print_and_log("[%s:hello]\n", "LIST");
									cse4589_print_and_log("%s",listall);
									cse4589_print_and_log("[%s:END]\n", "LIST");
									// printf("%s\n",temp);
									}
									else{
										cse4589_print_and_log("[%s:ERROR]\n", "LIST");
										cse4589_print_and_log("[%s:END]\n", "LIST");
									}
									}
									else{
										printf("You need to login first");
									}
									break;
							case 5: if(login_flag == 1){
									// char *buffer = (char*) malloc(sizeof(char)*2*BUFFER_SIZE);
									// 	memset(buffer, '\0', 2*BUFFER_SIZE);
										// printf("For List %s %d\n",list[0],ipp_index);
										// for(int l=0;l<=ipp_index;l++){
									// printf("I am here in List:%s\n",list[0]);
									// cse4589_print_and_log("%s\n",list[l]);
										// strcat(temp,list[l]);
										// strcat(temp, "\n");
										// }
										// char *strforsend=cmd;
										// strtok_r(strforsend, " ", &strforsend);
										// strtok_r(NULL, " ", &strforsend);
										// args[2]=strtok_r(NULL, "\n", &strforsend);
										char *msg = (char*) malloc(sizeof(char)*MSG_SIZE);
									memset(msg, '\0', MSG_SIZE);
									if(strcmp(args[0],"SEND")==0){
									
									// for(int j=0;j<=ipp_index;j++){
									// printf("%s\n",list[j]);
									// 	}
									// char temp[1024];
									// strcpy(temp, " ");
									// for(int l=0;l<=ipp_index;l++){
									// // printf("I am here in List:%s\n",list[0]);
									// cse4589_print_and_log("%s\n",list[l]);
									// 	// strcat(temp,list[l]);
									// 	// strcat(temp, "\n");
									// 	}
										// printf("%s\n",temp);
									//Problem here
									cse4589_print_and_log("[%s:SUCCESS]\n", "SEND");
									// cse4589_print_and_log("[%s:hello]\n", "LIST")
									strcpy(msg,"SEND ");
									strcat(msg,args[1]);
									strcat(msg," ");
									strcat(msg,args[2]);
									int len;

										len = strlen(msg);
										// if (sendall(s, msg, &len) == -1) {
										// 	perror("sendall");
										// 	printf("We only sent %d bytes because of the error!\n", len);
										// } 
									if(send(fdsocket, msg, strlen(msg), 0) == strlen(msg))
										printf("Done!\n");
									fflush(stdout);
									cse4589_print_and_log("[%s:END]\n", "SEND");
									// printf("%s\n",temp);
									}
									else{
										cse4589_print_and_log("[%s:ERROR]\n", "LIST");
										cse4589_print_and_log("[%s:END]\n", "LIST");
									}
									}
									else{
										printf("You need to login first");
									}
									break;
							default:;

							}
							
						free(cmd);
					} //END stdin
					else if(sock_index==fdsocket){
						/* Initialize buffer to receieve response */
						char *buffer = (char*) malloc(sizeof(char)*2*BUFFER_SIZE);
						memset(buffer, '\0', 2*BUFFER_SIZE);
						
						if(recv(sock_index, buffer, 2*BUFFER_SIZE, 0) <= 0){
							close(sock_index);
							printf("Remote Server terminated connection!\n");
							login_flag=0;
							
						// 	/* Remove from watched list */
							FD_CLR(sock_index, &master_list);
						}
						else {
							//Process incoming data from existing clients here ...
							
							printf("\nServer sent me: %s\n", buffer);
									string s=buffer;
									char *strlist=buffer;
									// int i=ipp_index+1;
									char *h;
									h=strtok_r(strlist," ",&strlist); 
									printf("%s\n",h);
									// std::size_t found = s.find("LOGIN");
								if (strncmp(h,"LOGIN",5)==0){
								
									while(h!=NULL)
									{
										h=strtok_r(NULL,",",&strlist); 
										if(h!=NULL && strcmp(h,"LOGIN")!=0){ 
										list[++ipp_index]=h;}
									}
									// list[i++]=strtok_r(NULL, ",", &strlist);
									int j=0;
									strcpy(listall,"");
									while(j<=ipp_index){
									// sprintf(listall,"%s\n",list[j]);
									strcat(listall,list[j]);
										j++;
									}
									
									printf("%s",listall);
									fflush(stdout);
								}
								else{
									printf("%s\n",h);
								// std::size_t found = s.find("SEND");
								cse4589_print_and_log("[RECEIVED:SUCCESS]\n");
								if(strncmp(h,"SEND",4)==0){
									
										char *h1;
										char *h2;
										h1=strtok_r(NULL, " ", &strlist);  
										h2=strtok_r(NULL, " ", &strlist); 
									if(h1!=NULL){ 
										printf("%s\n",h1);
								}
								if(h2!=NULL){
										printf("%s\n",h2);
								}
								cse4589_print_and_log("msg from:%s\n[msg]:%s\n",h1,h2);
								cse4589_print_and_log("[RECEIVED:END]\n");
									fflush(stdout);
								}
							
						}
						}
						free(buffer);
					}		
				}//END ISSET

			} // END FOR
		}
	} // END While
	return fdsocket;
}

						// if(strcmp(cmd,"LIST")){
						// 	printf("You must LOGIN First!");
						// }
						// //AUTHOR
						// if(strcmp(cmd,"AUTHOR\n")==0){
                        // cse4589_print_and_log("[%s:SUCCESS]\n", "AUTHOR");
						// cse4589_print_and_log("I, %s, have read and understood the course academic integrity policy.\n", "gursimr2");
						// cse4589_print_and_log("[%s:END]\n", "AUTHOR");
						// }
						// else{
						// 	cse4589_print_and_log("[%s:ERROR]\n", "AUTHOR");
						// 	cse4589_print_and_log("[%s:END]\n", "AUTHOR");
						// }
						// //PORT
						// if(strcmp(cmd,"PORT\n")==0){
                        // cse4589_print_and_log("[%s:SUCCESS]\n", "PORT");
						// cse4589_print_and_log("PORT:%s\n",port);
						// cse4589_print_and_log("[%s:END]\n", "PORT");
						// }
						// else{
						// 	cse4589_print_and_log("[%s:ERROR]\n", "PORT");
						// 	cse4589_print_and_log("[%s:END]\n", "PORT");
						// }
						// //IP
						// char *ip=new char[INET_ADDRSTRLEN];  
						// // inet_ntop(AF_INET,h->h_addr_list[0],ip, sizeof(ip));
						// if(strcmp(cmd,"IP\n")==0){
                        // cse4589_print_and_log("[%s:SUCCESS]\n", "IP");
						// ip=findipc();
						// cse4589_print_and_log("IP:%s\n",ip);
						// cse4589_print_and_log("[%s:END]\n", "IP");
						// }
						// else{
						// 	cse4589_print_and_log("[%s:ERROR]\n", "IP");
						// 	cse4589_print_and_log("[%s:END]\n", "IP");
						// }

						// //LOGIN
						// // cout<<cmd<<"\n";
						// char *str=cmd;
						// char *args[3];
						// int i=0;
						// while(args[i++]=strtok_r(str, " \n", &str));
						// // char host[100];
						// // char ip[INET_ADDRSTRLEN];  
						// // gethostname(host, sizeof(host));
						// // hostent *h = gethostbyname(host);
						// // inet_ntop(AF_INET,h->h_addr_list[0],ip, sizeof(ip));
						// // string out=string(cmd);
						// if(strcmp(args[0],"LOGIN")==0){
                        // cse4589_print_and_log("[%s %s %s:SUCCESS]\n", args[0],args[1],args[2]);
					
						// if(sock_index == client_socket){
						// 	// hostent *h = gethostbyaddr((struct sockaddr*)args[1],sizeof(struct in_addr),AF_INET);
						// if (getaddrinfo(args[1], args[2], &hints, &p) != 0)
						// 	perror("getaddrinfo failed");
						// 	/* Socket */
						// fdsocket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
						// if(fdsocket < 0)
						// 	perror("Failed to create socket");
						// // caddr_len = sizeof(client_addr);
						// fdconnect = connect(fdsocket, p->ai_addr, p->ai_addrlen);
						// // fdaccept = accept(server_socket, (struct sockaddr *)&client_addr, &caddr_len);
						// if(fdconnect < 0)
						// 	perror("Connect failed");
						
						// printf("\nRemote Server Connected!\n");                        
						// char *msg = (char*) malloc(sizeof(char)*MSG_SIZE);
						// memset(msg, '\0', MSG_SIZE);
						// if(fgets(msg, MSG_SIZE-1, stdin) == NULL) //Mind the newline character that will be written to msg
						// // 	exit(-1);
						// hostent *he;
						// struct in_addr ipv4addr;
						// char *ip2 = findipc();
						// inet_pton(AF_INET,ip2, &ipv4addr);
						// he = gethostbyaddr(&ipv4addr, sizeof(ipv4addr), AF_INET);
						// if(strcmp(msg,"AUTHOR\n")==0){
						//     char n[]="gursimr2";
						//     ("I, %s, have read and understood the course academic integrity policy.\n", n);
						// }
						// printf("I got:%s %s(size:%d chars)",ip,port,strlen(msg));
						// //  strcpy(msg,he->h_name);
						//  char *ip2 = new char[16];
						//  ip2=findipc();
						//  strcpy(msg,"LOGIN ");
						//  strcat(msg,ip2);
						//  strcat(msg," ");
						//  strcat(msg,port);
						// //  strcat(msg,"\n");
						// printf("\nSENDing it to the remote server ... %s",msg);
						// if(send(fdsocket, msg, strlen(msg), 0) == strlen(msg))
						// 	printf("Done!\n");
						// fflush(stdout);
//Receive for list						// if(recv(fdsocket,buffer, BUFFER_SIZE, 0) <= 0){ //changed sock_index to fdconnect
						// 	close(fdsocket); //same as above
						// 	printf("Remote Server terminated connection!\n");}
						// 	char *strlist=buffer;
						// 	int i=ipp_index+1;
						// while(list[i++]=strtok_r(strlist, ",", &strlist));
						// 	ipp_index=i;
						// 	printf("%s %s",cmd,buffer);
	//List					// if(strcmp(cmd,"LIST\n")==0){
                        // cse4589_print_and_log("[%s:SUCCESS]\n", "LIST");
						// for(int i=0;i<ipp_index;i++){
						// cse4589_print_and_log("%s",list[i]);
						// 	}
						// cse4589_print_and_log("[%s:END]\n", "LIST");
						// }
						// else{
						// 	cse4589_print_and_log("[%s:ERROR]\n", "LIST");
						// 	cse4589_print_and_log("[%s:END]\n", "LIST");
						// }

						// 	/* Remove from watched list */
						// 	FD_CLR(sock_index, &master_list);
						// }
						// // if(recv(fdsocket, buffer, BUFFER_SIZE, 0) <= 0){ //changed sock_index to fdconnect
						// 	close(fdsocket); //same as above
						// 	printf("Remote Server terminated connection!\n");
							
							/* Remove from watched list */
						// 	FD_CLR(sock_index, &master_list);
						// }
						// //LIST
						// int id=0;
						// char ipstr[INET_ADDRSTRLEN]; 
						// int s,port;
						// string r[4];
						// struct sockaddr *al[4];
						// int al_index=0;
						// if(strcmp(cmd,"LIST\n")==0){
                        // cse4589_print_and_log("[%s:SUCCESS]\n", "LIST");
						// send(fdsocket, "LIST", strlen("LIST"), 0);
						// recv(fdsocket, r, BUFFER_SIZE, 0); //changed sock_index to fdconnect
						// for(int i=0; i<al_index; i+=1){
						// getpeername(s, al[i], (socklen_t*)sizeof(al[i]));
						// struct sockaddr_in *sc = (struct sockaddr_in *)al[i];
						// port = ntohs(sc->sin_port);
						// inet_ntop(AF_INET, &sc->sin_addr, ipstr, sizeof(ipstr));
						// hostent *h = gethostbyaddr((struct sockaddr*)al[i],sizeof(struct in_addr),AF_INET);
						// for(int i=0;i<4;i++){
						// cse4589_print_and_log("%-5d%-63s%\n", i+1, r[i]);
						// 	}
						// cse4589_print_and_log("[%s:END]\n", "LIST");
						// }
						// else{
						// 	cse4589_print_and_log("[%s:ERROR]\n", "LIST");
						// 	cse4589_print_and_log("[%s:END]\n", "LIST");
						// }
						/* Add to watched socket list */
					// 	FD_SET(fdconnect, &master_list);
					// 	if(fdconnect > head_socket) head_socket = fdconnect;
					// }
					/* Read from existing clients */
					// else{
					// 	/* Initialize buffer to receieve response */
					// 	char *buffer = (char*) malloc(sizeof(char)*BUFFER_SIZE);
					// 	memset(buffer, '\0', BUFFER_SIZE);
						
					// 	if(recv(fdsocket, buffer, BUFFER_SIZE, 0) <= 0){ //changed sock_index to fdconnect
					// 		close(fdsocket); //same as above
					// 		printf("Remote Server terminated connection!\n");
							
					// 		/* Remove from watched list */
					// 		FD_CLR(sock_index, &master_list);
					// 	}
					// 	else {
					// 		//Process incoming data from existing clients here ...
							
					// 		printf("\nClient sent me: %s\n", buffer);
					// 		printf("ECHOing it back to the remote host ... ");
					// 		if(send(fdsocket, buffer, strlen(buffer), 0) == strlen(buffer))
					// 			printf("Done!\n");
					// 		fflush(stdout);
					// 	}
						
					// 	freeaddrinfo(res);
					// }

						// cse4589_print_and_log("[%s %s %s:END]\n", args[0],args[1],args[2]);
						// // cout<<cmd<<"\n";
					
						// }
						// else{
						// 	cse4589_print_and_log("[%s %s %s:ERROR]\n",args[0],args[1],args[2]);
						// 	cse4589_print_and_log("[%s %s %s:END]\n",args[0],args[1],args[2]);
						// }
						// string s = *args;
						// printf("\nI got: %s\n", s);
						//Process PA1 commands here ...
						
					// 	free(cmd);
					// }
					/* Check if new client is requesting connection */
					// else if(sock_index == client_socket){
					// 	caddr_len = sizeof(client_addr);
					// 	fdconnect = connect(fdsocket, res->ai_addr, res->ai_addrlen);
					// 	// fdaccept = accept(server_socket, (struct sockaddr *)&client_addr, &caddr_len);
					// 	if(fdconnect < 0)
					// 		perror("Connect failed");
						
					// 	printf("\nRemote Server Connected!\n");                        
						
					// // 	/* Add to watched socket list */
					// 	FD_SET(fdconnect, &master_list);
					// 	if(fdconnect > head_socket) head_socket = fdconnect;
					// }
					// /* Read from existing clients */
					// else{
					// 	/* Initialize buffer to receieve response */
					// 	char *buffer = (char*) malloc(sizeof(char)*BUFFER_SIZE);
					// 	memset(buffer, '\0', BUFFER_SIZE);
						
					// 	if(recv(sock_index, buffer, BUFFER_SIZE, 0) <= 0){
					// 		close(sock_index);
					// 		printf("Remote Server terminated connection!\n");
							
					// 	// 	/* Remove from watched list */
					// 		FD_CLR(sock_index, &master_list);
					// 	}
					// 	else {
					// 		//Process incoming data from existing clients here ...
							
					// 		printf("\nClient sent me: %s\n", buffer);
					// 		printf("ECHOing it back to the remote host ... ");
					// 		if(send(fdconnect, buffer, strlen(buffer), 0) == strlen(buffer))
					// 			printf("Done!\n");
					// 		fflush(stdout);
					// 	}
						
					// 	freeaddrinfo(res);
					// }
	// /* Connect */
	// if(connect(fdsocket, res->ai_addr, res->ai_addrlen) < 0)
	// 	perror("Connect failed");
	
	
// FROM MAIN
// while(TRUE){
	// 	printf("\n[PA1-Client@CSE489/589]$ ");
	// 	fflush(stdout);
		
	// 	char *msg = (char*) malloc(sizeof(char)*MSG_SIZE);
	// 	memset(msg, '\0', MSG_SIZE);
	// 	if(fgets(msg, MSG_SIZE-1, stdin) == NULL) //Mind the newline character that will be written to msg
	// 		exit(-1);
       
	// 	// if(strcmp(msg,"AUTHOR\n")==0){
    //     //     char n[]="gursimr2";
    //     //     ("I, %s, have read and understood the course academic integrity policy.\n", n);
    //     // }
	// 	printf("I got: %s(size:%d chars)", msg, strlen(msg));
		
	// 	printf("\nSENDing it to the remote server ... ");
	// 	if(send(server, msg, strlen(msg), 0) == strlen(msg))
	// 		printf("Done!\n");
	// 	fflush(stdout);
		
	// 	/* Initialize buffer to receieve response */
	// 	char *buffer = (char*) malloc(sizeof(char)*BUFFER_SIZE);
	// 	memset(buffer, '\0', BUFFER_SIZE);
		
	// 	if(recv(server, buffer, BUFFER_SIZE, 0) >= 0){
	// 		printf("Server responded: %s", buffer);
	// 		fflush(stdout);
	// 	}
	// }
// in_addr findaddrc(){
// 	int r;
// 	int udp_socket,udp_connect;
// 	struct addrinfo hints, *res;
// 	struct sockaddr_in sa;
// 	int sa_length;
// 	char ip[16];
// 	sa_length=sizeof(sa);
// 	memset(&hints, 0, sizeof(hints));
//     	hints.ai_family = AF_INET;
//     	hints.ai_socktype = SOCK_DGRAM;
// 	if (getaddrinfo("8.8.8.8","53", &hints, &res) != 0)
// 		perror("getaddrinfo failed");

// 	udp_socket = socket(res->ai_family, res->ai_socktype,0);
// 	if(udp_socket < 0)
// 		perror("Cannot create socket");
// 	udp_connect = connect(udp_socket, res->ai_addr, res->ai_addrlen);
// 	if(udp_connect < 0)
// 		perror("Connect failed");
// 	r=getsockname(udp_socket,(sockaddr *)&sa,(socklen_t *)&sa_length);
// 	close(udp_socket);
// 	printf("%s",sa.sin_addr);
// 	return sa.sin_addr;
// }	
