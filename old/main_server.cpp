#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <string.h>
#include <iostream>

#define MYPORT 3490
//
// Une autre chose, fait attention quand vous appelez bind(): ne mettez pas n'importe 
// quel numéro de port. Tous les port en dessous de 1024 sont réservés. Vous pouvez 
// utiliser n'importe quel port au dessus de 1024 jusqu'à 65535 ( prenez un port qui 
// n'est pas utilisé par un autre programme).
//

#define BACKLOG 10
//
// Maximum number of connection entries waiting
//

#define	IN		0
#define	OUT		1
#define	INOUT	2

struct sockadddr {
	unsigned short	sa_family;
	char			sa_data[14];
};

// struct in_addr {
// 	unsigned long	s_addr;					/* network byte order */
// };


// struct sockaddr_in {
// 	short int			sin_family;
// 	unsigned short int	sin_port;			/* network byte order */
// 	struct in_addr		sin_addr;
// 	unsigned char		sin_zero[8];
// };

int main (void) {
	
	int sockfd;
	struct sockaddr_in my_addr;
	int ret;

	// INIT SOCKET FD
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		std::cout << "error in socket: " << std::strerror(errno) << std::endl;
		return (1);
	}

	// INIT STRUCT
	my_addr.sin_family = AF_INET;						/* host byte order */
	my_addr.sin_port = htons(MYPORT);					/* short, network byte order */
	my_addr.sin_addr.s_addr =  inet_addr("127.0.0.1");	/* return in network byte order */
	if (my_addr.sin_addr.s_addr == (in_addr_t) -1) {
		std::cout << "error in IP address: " << std::strerror(errno) << std::endl;
		return (1);
	}
	/*
	//	En mettant my_addr.sin_port a zero, on demande a bind() de choisir le port pour nous. 
	//	De même, en affectant la valeur INADDR_ANY a my_addr.sin_addr.s_addr, on demande que l'adresse IP de notre machine soit mise automatiquement.
		my_addr.sin_port = 0; // choose an unused port at random 
		my_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // use my IP adresse 
	*/
	bzero(&(my_addr.sin_zero), 8);						/* bzero of sin_zero*/


	// BIND
	ret = bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr));		/* Error management to be done */
	if (ret == -1) {
		std::cout << "error in bind: " << std::strerror(errno) << std::endl;
		return (1);
	}

	// ALTERNATIVE WITH CONNECT (FOR DISTANT HOSTS)
	//connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr));

	// LISTEN
	ret = listen(sockfd, BACKLOG);
	if (ret == -1) {
		std::cout << "error in listen: " << std::strerror(errno) << std::endl;
		return (1);
	}

	// ACCEPT
	int new_fd;							// client fd
	while (1) {
		struct sockaddr_in client_addr;
		int sin_size = sizeof(client_addr);
		std::cout << "server: " << std::endl;
		new_fd = accept(sockfd, (struct sockaddr *)&client_addr, (socklen_t *)&sin_size);
		if (new_fd == -1) {
			std::cout << "error in accept: " << std::strerror(errno) << std::endl;
			return (1);
		}
		// SEND
		// char *msg = "I'm here!";
		// int len, bytes_sent;
		// len = strlen(msg);
		// bytes_sent = send(sockfd, msg, len, 0);
		// if (bytes_sent != len) {
		// 	std::cout << "Error: Use a buffer to send the rest of the data" << std::endl;
		// }
		// else if (bytes_sent == -1) {
		// 	std::cout << "error in send: " << std::strerror(errno) << std::endl;
		// 	return (1);			
		// }

		// RECV
		int bytes_received;
		void *buff = nullptr;
		int sizeof_buff = sizeof(buff);
		bytes_received = recv(new_fd, buff, sizeof_buff, 0);
		if (bytes_received == -1) {
			std::cout << "error in recv: " << std::strerror(errno) << std::endl;
			return (1);			
		}
		std::cout << "received bytes: " << bytes_received << std::endl;
	}
	//	close(sockfd);
	ret = shutdown(sockfd, INOUT);
	if (ret == -1) {
		std::cout << "error in shutdown: " << std::strerror(errno) << std::endl;
		return (1);			
	}
	return (0);
}