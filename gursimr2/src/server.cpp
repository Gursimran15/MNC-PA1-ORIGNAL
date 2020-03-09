/**
* @server
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
* This file contains the server init and main while loop for tha application.
* Uses the select() API to multiplex between network I/O and STDIN.
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include "../include/logger.h"
#include <arpa/inet.h>
#include <iostream>
#include<bits/stdc++.h> 
using namespace std;

#define MSG_SIZE 256
#define BACKLOG 5
#define STDIN 0
#define TRUE 1
#define CMD_SIZE 100
#define BUFFER_SIZE 256

/**
* main function
*
* @param  argc Number of arguments
* @param  argv The argument list
* @return 0 EXIT_SUCCESS
*/
struct lists {
int list_id;
char hostname[512];
char ipaddr[INET_ADDRSTRLEN];
int port_no;
int fd;
int login;
};
struct blocking{
	int index[4];
	char blocker[INET_ADDRSTRLEN];
};
bool compare(lists a, lists b) 
{ 
	int t;
        if(a.port_no < b.port_no){
		// if(a.list_id > b.list_id){
		// t=a.list_id;
		// a.list_id=b.list_id;
		// b.list_id=t;}
		return 1;}
		else{
		return 0;}
} 
void sorting(lists ll[],int n){

    sort(ll, ll + n,compare); 
    
    for (int i=0; i<n; i++) 
        ll[i].list_id = i+1; 
}
char* findip(){
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
void add_client(char *buffer,int &i_index,lists l[],int socketfd){
							char *args[3];
							// printf("I am here");
							char *str=buffer;
							// printf("I am here 1");
							args[0]=strtok_r(str, " ", &str);
							// printf("I am here 2");
							// if(strcmp(args[0],"LOGIN")==0){
								printf("Before Local Inc :%d\n",i_index);
								i_index++;
								printf("After Local Inc :%d\n",i_index);
								// printf("I am here 3");
								args[1]=strtok_r(NULL, " ",&str);
								// printf("I am here 4");
								strncpy(l[i_index].ipaddr,args[1],sizeof(l[i_index].ipaddr));
								// printf("I am here 5");
								args[2]=strtok_r(NULL, " ",&str);
								sscanf(args[2], "%d", &l[i_index].port_no);
								// printf("I am here 6");
								hostent *he;
								// printf("I am here 6.1");
								struct in_addr ipv4addr;
								// printf("%s\n",l[ipp_index].ipaddr);
								// printf("I am here 6.2 %s \n",args[1]);
								inet_pton(AF_INET,args[1], &ipv4addr);
								// printf("I am here 7%s\n",ipv4addr.s_addr);
								he = gethostbyaddr(&ipv4addr, sizeof(ipv4addr), AF_INET);
								// printf("I am here 8");
								l[i_index].list_id=i_index+1;
								l[i_index].fd=socketfd;
								l[i_index].login=1;
								printf("%d\n",socketfd);
								// printf("I am here 8.1");
								strncpy(l[i_index].hostname,he->h_name,256);
								// printf("%-5d%-35s%-20s%-8d\n", l[ipp_index].list_id,l[ipp_index].hostname,l[ipp_index].ipaddr,l[ipp_index].port_no);
								// printf("I am here 8.2");
								// 	printf("ECHOing it back to the remote host ... ");
								// strcpy(msg,"");
								// printf("Before Sorting :%d\n",ipp_index);
								// for(int i=0;i<=1;i++){
								// 	printf("%-5d%-35s%-20s%-8d\n",l[ipp_index].list_id,l[ipp_index].hostname,l[ipp_index].ipaddr,l[ipp_index].port_no);
								// }
								sorting(l,i_index + 1);
								// printf("After Sort");
								// printf(" :%d\n",ipp_index);
								// for(int i=0;i<=1;i++){
								// 	printf("%-5d%-35s%-20s%-8d\n",l[ipp_index].list_id,l[ipp_index].hostname,l[ipp_index].ipaddr,l[ipp_index].port_no);
								// }
									// for(int i=0;i<=ipp_index;i++){
									// 	sprintf(buffer,"%-5d%-35s%-20s%-8d\n",l[ipp_index].list_id,l[ipp_index].hostname,l[ipp_index].ipaddr,l[ipp_index].port_no);
									// 	strcat(msg,buffer);
									// 	strcat(msg,",");
									// }
							// }
}
int server(char *arg)
{
	
	// if(argc != 2) {
	// 	printf("Usage:%s [port]\n", argv[0]);
	// 	exit(-1);
	// }
	// blocking b[4];
	map<string, string> blocklist;
	int ipp_index=-1;
	int donotmove=0;
	int server_socket, head_socket, selret, sock_index, fdaccept=0, caddr_len;
	struct sockaddr_in client_addr;
	struct addrinfo hints, *res,*p;
	fd_set master_list, watch_list;
	struct sockaddr *al[4];
	int al_index=0;
	string l_buffer[4];
	/* Set up hints structure */
	memset(&hints, 0, sizeof(hints));
    	hints.ai_family = AF_INET;
    	hints.ai_socktype = SOCK_STREAM;
    	hints.ai_flags = AI_PASSIVE;
	int yes=1;
	/* Fill up address structures */
	if (getaddrinfo(NULL, arg, &hints, &res) != 0)
		perror("getaddrinfo failed");
	/* Socket */
	server_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if(server_socket < 0)
		perror("Cannot create socket");
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	/* Bind */
	if(bind(server_socket, res->ai_addr, res->ai_addrlen) < 0 )
		perror("Bind failed");

	freeaddrinfo(res);
	
	/* Listen */
	if(listen(server_socket, BACKLOG) < 0)
		perror("Unable to listen on port");
	
	/* ---------------------------------------------------------------------------- */
	
	/* Zero select FD sets */
	FD_ZERO(&master_list);
	FD_ZERO(&watch_list);
	
	/* Register the listening socket */
	FD_SET(server_socket, &master_list);
	/* Register STDIN */
	FD_SET(STDIN, &master_list);
	
	head_socket = server_socket;
//    printf("\nI got: here 1\n");
	// char *ip_list[4];
	// char *port_list[4];
	struct lists l[4];
	char *ip=new char[INET_ADDRSTRLEN];  
	while(TRUE){
		memcpy(&watch_list, &master_list, sizeof(master_list));
		
		//printf("\n[PA1-Server@CSE489/589]$ ");
		//fflush(stdout);
		// printf("\nI got: here 2\n");
		/* select() system call. This will BLOCK */
		selret = select(head_socket + 1, &watch_list, NULL, NULL, NULL);
		if(selret < 0)
			perror("select failed.");
		
		/* Check if we have sockets/STDIN to process */
		if(selret > 0){
			/* Loop through socket descriptors to check which ones are ready */
			for(sock_index=0; sock_index<=head_socket; sock_index+=1){
				
				if(FD_ISSET(sock_index, &watch_list)){
					
					/* Check if new command on STDIN */
					if (sock_index == STDIN){
						char *cmd = (char*) malloc(sizeof(char)*CMD_SIZE);
						// printf("\nI got: here 3\n");
						memset(cmd, '\0', CMD_SIZE);
						if(fgets(cmd, CMD_SIZE-1, stdin) == NULL) //Mind the newline character that will be written to cmd
							exit(-1);
							char *strforblocked=cmd;
							char *args[2];
							int j=0;
							while(args[j++]=strtok_r(strforblocked, " \n", &strforblocked));
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
							if(strcmp(cmd,"LIST\n")==0){
								s=3;
							}
							if(strcmp(cmd,"EXIT\n")==0){
								s=4;
							}
							if(strcmp(args[0],"BLOCKED")==0){
								s=5;
							}
							// if(s>3){
							// 	printf("You must Login first!");
							// }
						switch(s){
							case 0:
									if(strcmp(cmd,"AUTHOR\n")==0){
								cse4589_print_and_log("[%s:SUCCESS]\n", "AUTHOR");
								cse4589_print_and_log("I, %s, have read and understood the course academic integrity policy.\n", "gursimr2");
								cse4589_print_and_log("[%s:END]\n", "AUTHOR");
								}
								else{
									cse4589_print_and_log("[%s:ERROR]\n", "AUTHOR");
									cse4589_print_and_log("[%s:END]\n", "AUTHOR");
								}
								break;

							case 1: if(strcmp(cmd,"PORT\n")==0){
									cse4589_print_and_log("[%s:SUCCESS]\n", "PORT");
									cse4589_print_and_log("PORT:%s\n",arg);
									cse4589_print_and_log("[%s:END]\n", "PORT");
									}
									else{
										cse4589_print_and_log("[%s:ERROR]\n", "PORT");
										cse4589_print_and_log("[%s:END]\n", "PORT");
									}
									break;
							case 2: 
									// inet_ntop(AF_INET,h->h_addr_list[0],ip, sizeof(ip));
									if(strcmp(cmd,"IP\n")==0){
									cse4589_print_and_log("[%s:SUCCESS]\n", "IP");
									ip=findip();
									cse4589_print_and_log("IP:%s\n",ip);
									cse4589_print_and_log("[%s:END]\n", "IP");
									}
									else{
										cse4589_print_and_log("[%s:ERROR]\n", "IP");
										cse4589_print_and_log("[%s:END]\n", "IP");
									}
									break;
							case 3: 	if(strcmp(cmd,"LIST\n")==0){
										cse4589_print_and_log("[%s:SUCCESS]\n", "LIST");
											for(int i=0;i<=ipp_index && l[i].login==1;i++){
										cse4589_print_and_log("%-5d%-35s%-20s%-8d\n", l[i].list_id,l[i].hostname,l[i].ipaddr,l[i].port_no);
											}
										cse4589_print_and_log("[%s:END]\n", "LIST");
										}
										else{
											cse4589_print_and_log("[%s:ERROR]\n", "LIST");
											cse4589_print_and_log("[%s:END]\n", "LIST");
										}
										break;
							case 4: {
									if(strcmp(cmd,"EXIT")==0){
									cse4589_print_and_log("[%s:SUCCESS]\n", "EXIT");
									cse4589_print_and_log("[%s:END]\n", "EXIT");
									exit(0);
									}
									else{
										cse4589_print_and_log("[%s:ERROR]\n", "EXIT");
										cse4589_print_and_log("[%s:END]\n", "EXIT");
									}
									break;
									}
							case 5: {
									if(strcmp(args[0],"BLOCKED")==0){
									cse4589_print_and_log("[%s:SUCCESS]\n", "BLOCKED");
									cse4589_print_and_log("[%s:END]\n", "BLOCKED");
									// exit(0);
									}
									else{
										cse4589_print_and_log("[%s:ERROR]\n", "BLOCKED");
										cse4589_print_and_log("[%s:END]\n", "BLOCKED");
									}
									break;
									}
						}

						printf("\nI got: %s\n", cmd);
						
						//Process PA1 commands here ...
						
						free(cmd);
					}
						
					/* Check if new client is requesting connection */
					else if(sock_index == server_socket){
						caddr_len = sizeof(client_addr);
						fdaccept = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t*)&caddr_len);
						if(fdaccept < 0)
							perror("Accept failed.");
						
						printf("\nRemote Host connected!\n");
						al[al_index ++] = (struct sockaddr *)&client_addr;
						/* Add to watched socket list */
						printf("%d\n",fdaccept);
						FD_SET(fdaccept, &master_list);
						if(fdaccept > head_socket) head_socket = fdaccept;
					}
					/* Read from existing clients */
					else{
						char *buffer = (char*) malloc(sizeof(char)*10*BUFFER_SIZE);
						memset(buffer, '\0', 10*BUFFER_SIZE);
						/* Initialize buffer to receieve response */
					    if(recv(sock_index, buffer, 10*BUFFER_SIZE, 0) <= 0){
							printf("Remote Host terminated connection!\n");           
						// if(recv(sock_index, buffer, BUFFER_SIZE, 0) <= 0){ //changed sock_index to fdaccept
                        //     printf("%s\n",buffer);
						// 	if(strcmp(buffer,"LIST")==0){
						// 		 send(sock_index, l_buffer, sizeof(l_buffer), 0);//changed sock_index to fdaccept
						// 	}
							close(sock_index);
							ipp_index--;
							printf("Remote Host terminated connection!\n");
							// al[al_index ] = NULL;
							// al_index--;
							/* Remove from watched list */
							FD_CLR(sock_index, &master_list);
						}
						else {
							//Process incoming data from existing clients here ...
							
							printf("\nClient sent me: %s\n", buffer);
							printf("I am here");
							string s=buffer;
							char *msg = (char*) malloc(sizeof(char)*15*MSG_SIZE);
							memset(msg, '\0', 15*MSG_SIZE);
							std::size_t found = s.find("LOGIN");
  							if (found!=std::string::npos){
								  donotmove=0;
    						// {std::cout << "first 'node' found at: " << found << '\n';
							printf("Before add client :%d\n",ipp_index);
							printf("%s %s",l[0].hostname,l[1].hostname);
							add_client(buffer,ipp_index,l,sock_index);
							printf("After add client :%d\n",ipp_index);
							printf("%s %s",l[0].hostname,l[1].hostname);
							strcpy(msg,"LOGIN ");
							int n;
							for(int i=0;i<=ipp_index;i++){
										n=sprintf(buffer,"%-5d%-35s%-20s%-8d\n",l[i].list_id,l[i].hostname,l[i].ipaddr,l[i].port_no);
										strncat(msg,buffer,n);
										if(i<ipp_index)
										strncat(msg,",",1);
										}
							if(send(sock_index,msg, strlen(msg), 0) == strlen(msg))
								printf("\nList send to Client\n");
							}
							else{
								std::size_t found = s.find("SEND");
								if (found!=std::string::npos && donotmove==0){
									printf("Client Sent:%s\n",buffer);
									socklen_t len;
									struct sockaddr_storage addr;
									char ipstr[INET6_ADDRSTRLEN];
									int port;

									len = sizeof addr;
									getpeername(sock_index, (struct sockaddr*)&addr, &len);
									if (addr.ss_family == AF_INET) {
										struct sockaddr_in *s = (struct sockaddr_in *)&addr;
										port = ntohs(s->sin_port);
										inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
									}
									printf("Peer IP address: %s\n", ipstr);
									
										int dest;
										char *args[3];
										// printf("I am here");
										char *str=buffer;
										// printf("I am here 1");
										args[0]=strtok_r(str, " ", &str);
										printf("%s\n",args[0]);
											char *msg = (char*) malloc(sizeof(char)*15*MSG_SIZE);
											memset(msg, '\0', 15*MSG_SIZE);
										// printf("I am here 2");
										// if(strcmp(args[0],"LOGIN")==0){
											// printf("I am here 3");
											args[1]=strtok_r(NULL, " ",&str);
											printf("%s\n",args[1]);
											for(int i=0;i<=ipp_index;i++){
												if(strcmp(l[i].ipaddr,args[1])==0 && l[i].login==1){
													dest=l[i].fd;
												}

												// else if(!(strcmp(l[i].ipaddr,args[1])==0)&& l[i].login==0){
												// 	cse4589_print_and_log("[RELAYED:ERROR]\n");
												// 	cse4589_print_and_log("[RELAYED:END]\n");
												// }
											}
											// auto it = blocklist.begin();
		//Checking Blocklist Value against IP									// 						// Iterate through the map
											// 						while(it != blocklist.end())
											// 						{
											// 							if(blocklist.find([1]])){

											// 							}
											// 							it++;
											// 						}
											// printf("I am here 4");
											// printf("I am here 5");
											args[2]=strtok_r(NULL, " ",&str);
											printf("%s\n",args[2]);
											cse4589_print_and_log("[RELAYED:SUCCESS]\n");
											cse4589_print_and_log("msg from:%s, to:%s\n[msg]:%s\n",ipstr, args[1], args[2]);
											// LOG_PRINT("msg from:%s, to:%s\n[msg]:%s\n",ipstr, args[1], args[2]);
											strcpy(msg,"SEND ");
											strcat(msg,ipstr);
											strcat(msg," ");
											strcat(msg,args[2]);
											printf("%s\n%d\n",msg,dest);
											 for(int j = 0; j <= head_socket; j++) {
                            // send to everyone!
                            if (FD_ISSET(j, &master_list)) {
                                // except the listener and ourselves
                                if (j != server_socket && j != sock_index) {
												if(send(dest, msg, strlen(msg), 0) == strlen(msg))
												{	printf("Done!\n");break;}
												fflush(stdout);
								}
							}
											 }
											 cse4589_print_and_log("[RELAYED:END]\n");
								}
								else{
											std::size_t found = s.find("BROADCAST");
										if (found!=std::string::npos && donotmove==0){
											printf("Client Sent:%s\n",buffer);
											socklen_t len;
											struct sockaddr_storage addr;
											char ipstr[INET6_ADDRSTRLEN];
											int port;

											len = sizeof addr;
											getpeername(sock_index, (struct sockaddr*)&addr, &len);
											if (addr.ss_family == AF_INET) {
												struct sockaddr_in *s = (struct sockaddr_in *)&addr;
												port = ntohs(s->sin_port);
												inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
											}
											printf("Peer IP address: %s\n", ipstr);
											
												int dest;
												char *args[3];
												// printf("I am here");
												char *str=buffer;
												// printf("I am here 1");
												args[0]=strtok_r(str, " ", &str);
												printf("%s\n",args[0]);
													char *msg = (char*) malloc(sizeof(char)*15*MSG_SIZE);
													memset(msg, '\0', 15*MSG_SIZE);
													args[1]=strtok_r(NULL, " ",&str);
													printf("%s\n",args[1]);
													strcpy(msg,"BROADCAST ");
															strcat(msg,ipstr);
															strcat(msg," ");
															strcat(msg,args[1]);
													for(int i=0;i<=ipp_index;i++){
														if(!(strcmp(l[i].ipaddr,ipstr)==0) && l[i].login==1){
															dest=l[i].fd;
															printf("%s\n%d\n",msg,dest);
															for(int j = 0; j <= head_socket; j++) {
														// send to everyone!
														if (FD_ISSET(j, &master_list)) {
															// except the listener and ourselves
															if (j != server_socket && j != sock_index) {
																			if(send(dest, msg, strlen(msg), 0) == strlen(msg))
																			{	printf("Done!\n");break;}
																			fflush(stdout);
																		}

																	}
																}
																	}
																}
													// printf("I am here 4");
													// printf("I am here 5");
													cse4589_print_and_log("[RELAYED:SUCCESS]\n");
													cse4589_print_and_log("msg from:%s, to:%s\n[msg]:%s\n",ipstr,"255.255.255.255", args[1]);
													// LOG_PRINT("msg from:%s, to:%s\n[msg]:%s\n",ipstr, args[1], args[2]);
									// 				strcpy(msg,"BROADCAST ");
									// 				strcat(msg,ipstr);
									// 				strcat(msg," ");
									// 				strcat(msg,args[1]);
									// 				printf("%s\n%d\n",msg,dest);
									// 				for(int j = 0; j <= head_socket; j++) {
									// // send to everyone!
									// if (FD_ISSET(j, &master_list)) {
									// 	// except the listener and ourselves
									// 	if (j != server_socket && j != sock_index) {
									// 					if(send(dest, msg, strlen(msg), 0) == strlen(msg))
									// 					{	printf("Done!\n");break;}
									// 					fflush(stdout);
									// 	}
									// }
									// 				}
													cse4589_print_and_log("[RELAYED:END]\n");


											}//IF BROADCAST
											else{
							std::size_t found = s.find("REFRESH");
  							if (found!=std::string::npos && donotmove==0){
								  strcpy(msg,"REFRESH ");
							int n;
							for(int i=0;i<=ipp_index;i++){
										if(l[i].login == 1){
										n=sprintf(buffer,"%-5d%-35s%-20s%-8d\n",l[i].list_id,l[i].hostname,l[i].ipaddr,l[i].port_no);
										strncat(msg,buffer,n);
										if(i<ipp_index)
										strncat(msg,",",1);}
										}
							if(send(sock_index,msg, strlen(msg), 0) == strlen(msg))
								printf("\nList send to Client\n");
											}
											else{
												std::size_t found = s.find("LOGOUT");
  										if (found!=std::string::npos && donotmove==0){
								  			// strcpy(msg,"REFRESH ");
											  char *args[3];
												// printf("I am here");
												char *str=buffer;
												// printf("I am here 1");
												args[0]=strtok_r(str, " ", &str);
												args[1]=strtok_r(NULL, " ", &str);
												int n;
												struct lists t;
												printf("I am here");
												for(int i=0;i<=ipp_index;i++){
														if((strcmp(l[i].ipaddr,args[1])==0) && l[i].login==1){
															l[i].login=0;
															donotmove=1;
														}}

												}//IF LOGOUT
												else{
													std::size_t found = s.find("BLOCK");
  										if (found!=std::string::npos && donotmove==0){
											   char *args[3];
												// printf("I am here");
												char *str=buffer;
												// printf("I am here 1");
												args[0]=strtok_r(str, " ", &str);
												args[1]=strtok_r(NULL, " ", &str);
												socklen_t len;
											struct sockaddr_storage addr;
											char ipstr[INET6_ADDRSTRLEN];
											int port;

											len = sizeof addr;
											getpeername(sock_index, (struct sockaddr*)&addr, &len);
											if (addr.ss_family == AF_INET) {
												struct sockaddr_in *s = (struct sockaddr_in *)&addr;
												port = ntohs(s->sin_port);
												inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
											}
											printf("Peer IP address: %s\n", ipstr);
											blocklist[ipstr]=args[1];
												// for(int i=0;i<=ipp_index;i++){
												// 		if(!(strcmp(l[i].ipaddr,args[1])==0) && l[i].login==1){
												// 			b[0].blocker=ipstr;
												// 			b[0].index[0]=}
												// }
												
										  }//IF BLOCK
												}
											}
										}
								
							}
							// if(send(fdaccept, buffer , strlen(buffer), 0) == strlen(buffer))
							// 	printf("Done!\n");
							}
						}//received messages/data
						
						free(buffer);
					}
				}
			}
		}
	}
	close(sock_index);
	
	return 0;
}
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
						// cse4589_print_and_log("PORT:%s\n",arg);
						// cse4589_print_and_log("[%s:END]\n", "PORT");
						// }
						// else{
						// 	cse4589_print_and_log("[%s:ERROR]\n", "PORT");
						// 	cse4589_print_and_log("[%s:END]\n", "PORT");
						// }
						// //IP
						
						// // inet_ntop(AF_INET,h->h_addr_list[0],ip, sizeof(ip));
						// if(strcmp(cmd,"IP\n")==0){
                        // cse4589_print_and_log("[%s:SUCCESS]\n", "IP");
						// ip=findip();
						// cse4589_print_and_log("IP:%s\n",ip);
						// cse4589_print_and_log("[%s:END]\n", "IP");
						// }
						// else{
						// 	cse4589_print_and_log("[%s:ERROR]\n", "IP");
						// 	cse4589_print_and_log("[%s:END]\n", "IP");
						// }

						// LIST
						// int id=0;
						// char ipstr[INET_ADDRSTRLEN]; 
						// int s,port;
						// if(strcmp(cmd,"LIST\n")==0){
                        // cse4589_print_and_log("[%s:SUCCESS]\n", "LIST");
						// for(int i=0; i<=ipp_index; i+=1){
						// getpeername(s, al[i], (socklen_t*)sizeof(al[i]));
						// struct sockaddr_in *sc = (struct sockaddr_in *)al[i];
						// port = ntohs(sc->sin_port);
						// inet_ntop(AF_INET, &sc->sin_addr, ipstr, sizeof(ipstr));
						// hostent *h = gethostbyaddr((struct sockaddr*)ipstr,sizeof(struct in_addr),AF_INET);
						// string ip = ipstr;
						// l_buffer[id++] = (char *)h->h_name + ip + to_string(htons(port));
						// char *msg = (char*) malloc(sizeof(char)*MSG_SIZE);
						// memset(msg, '\0', MSG_SIZE);
						// if(fgets(msg, MSG_SIZE-1, stdin) == NULL) //Mind the newline character that will be written to msg
						// 	exit(-1);
						// hostent *he;
						// struct in_addr ipv4addr;
						// char *ip2 = ip_list[i];
						// inet_pton(AF_INET,ip2, &ipv4addr);
						// he = gethostbyaddr(&ipv4addr, sizeof(ipv4addr), AF_INET);
						// for(int i=0;i<=ipp_index;i++){
						// cse4589_print_and_log("%-5d%-35s%-20s%-8d\n", l[ipp_index].list_id,l[ipp_index].hostname,l[ipp_index].ipaddr,l[ipp_index].port_no);
						// 	}
						// cse4589_print_and_log("[%s:END]\n", "LIST");
						// }
						// else{
						// 	cse4589_print_and_log("[%s:ERROR]\n", "LIST");
						// 	cse4589_print_and_log("[%s:END]\n", "LIST");
						// }