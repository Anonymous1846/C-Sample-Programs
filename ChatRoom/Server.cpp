#include<iostream>
#include<vector>
#include<map>
#include<cstring>
#include<sys/select.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>


#define PORT 9090
#define MAX_CLIENTS 10

int main(){
	
	int server_fd,new_socket, max_sd,activity, valread;
	struct sockaddr_in address;

	int addr_len = sizeof(address);
	char buffer[1024];
	fd_set descriptor;
	if((server_fd=socket(AF_INET,SOCK_STREAM,0))==0){
	perror("Socket failed");
	exit(EXIT_FAILURE);
	}
	int opt =1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	if(bind(server_fd,(struct sockaddr *) &address, sizeof(address)) < 0){
		perror("Bind failed");
		exit(EXIT_FAILURE);
	}
	if(listen(server_fd,3)<0){
		perror("Listen failed");
		exit(EXIT_FAILURE);
	}
	std::cout << "Chat server started on port " << PORT << "....."<<std::endl;
	map<int, string> clients;
	vector<string> chatHistory;
	while(true){
		FD_ZERO(&readfds);
		FD_SET(server_fd,&readfds);
		max_sd= server_fd;
	}
	for(auto &p: clients){
		int sd = p.first;
		FD_SET(sd,&readfds);
		if(sd>max_sd) max_sd = sd;
	}

	activity = select(max_sd+1,&readfds,NULL,NULL,NULL);
	if((activity < 0 ) && (errno !=EINTR)){
		perror("select error");
	}

	if(FD_ISSET(server_fd,&readfds)){
	if((new_socket = accept(server_fd,(struct sockaddr * )&address,(socklen_t *)&adddrlen))<0){
	perror("Accept failed");
	exit(EXIT_FAILURE);
	}
	}
	return 0;
}
