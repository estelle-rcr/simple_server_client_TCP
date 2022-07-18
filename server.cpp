#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

# define MYPORT 3490    /* Le port où les utilisateurs se connecteront */
# define BACKLOG 10     /* Nombre maxi de connections acceptées en file */

int main()
{
	int sockfd, new_fd;  /* Ecouter sock_fd, nouvelle connection sur new_fd */
	struct sockaddr_in my_addr;    /* Adresse */
	struct sockaddr_in their_addr; /* Adresse du connecté  */
	int sin_size;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	my_addr.sin_family = AF_INET;         /* host byte order */
	my_addr.sin_port = htons(MYPORT);     /* short, network byte order */
	my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-remplissage avec mon IP */
	bzero(&(my_addr.sin_zero), 8);        /* zero pour le reste de struct */

	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
		perror("bind");
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	while(1) {  /* main accept() loop */
		sin_size = sizeof(struct sockaddr_in);
		if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, (socklen_t *)&sin_size)) == -1) {
			perror("accept");
			continue;
		}
		printf("serveur: Reçu connection de %s\n", inet_ntoa(their_addr.sin_addr));
		if (!fork()) { /* processus fils */
			if (send(new_fd, "Hello, world!\n", 14, 0) == -1)
				perror("send");
			close(new_fd);
			exit(0);
		}
		close(new_fd);  /* Le parent n'a pas besoin de cela */

		while(waitpid(-1,NULL,WNOHANG) > 0); /* Nettoyage des processus fils */
	}
	return (0);
}