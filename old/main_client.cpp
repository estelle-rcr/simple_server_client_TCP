#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <string.h>
#include <iostream>

#define MYPORT 3490

int main() {

	int sockfd;
	struct sockaddr_in dest_addr;
	int ret;

	// INIT SOCKET FD
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		std::cout << "error in socket: " << std::strerror(errno) << std::endl;
		return (1);
	}

	// INIT STRUCT
	dest_addr.sin_family = AF_INET;						/* host byte order */
	dest_addr.sin_port = htons(3490);					/* short, network byte order */
	dest_addr.sin_addr.s_addr = inet_addr("127.0.0.1");	/* return in network byte order */
	if (dest_addr.sin_addr.s_addr == (in_addr_t) -1) {
		std::cout << "error in IP address: " << std::strerror(errno) << std::endl;
		return (1);
	}
	/*
	//	En mettant dest_addr.sin_port a zero, on demande a bind() de choisir le port pour nous. 
	//	De même, en affectant la valeur INADDR_ANY a dest_addr.sin_addr.s_addr, on demande que l'adresse IP de notre machine soit mise automatiquement.
		dest_addr.sin_port = 0; // choose an unused port at random 
		dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // use my IP adresse 
	*/
	bzero(&(dest_addr.sin_zero), 8);						/* bzero of sin_zero*/

	ret = connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr));
	if (ret == -1) {
		std::cout << "error in connect: " << std::strerror(errno) << std::endl;
		return (1);
	}

	//SEND
	std::string str = "I'm here!";
	char *msg = const_cast<char *>(str.c_str());
	int len, bytes_sent;
	while (1) {
		len = strlen(msg);
		std::cout << "client" << std::endl;
		bytes_sent = send(sockfd, msg, len, 0);
		// if (bytes_sent != len) {
		// 	std::cout << "Error: Use a buffer to send the rest of the data" << std::endl;
		// }
		// else if (bytes_sent == -1) {
		// 	std::cout << "error in send: " << std::strerror(errno) << std::endl;
		// 	return (1);			
		// }
	}
	return (0);
}