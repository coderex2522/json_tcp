#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "json-c/json.h"

int main(int argc, char *argv[])
{
	struct sockaddr_in server_addr, client_addr;
	int server_sockfd, client_sockfd, err, cnt;
	socklen_t addrlen;
	char send_buff[BUFSIZ],recv_buff[BUFSIZ];

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=INADDR_ANY;
	server_addr.sin_port=htons(39400);

	server_sockfd=socket(AF_INET, SOCK_STREAM, 0);
	if(server_sockfd<0)
	{
		perror("socket call error.");
		return -1;
	}

	err=bind(server_sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(err<0)
	{
		perror("bind call error.");
		return -1;
	}

	err=listen(server_sockfd, 5);
	if(err<0)
	{
		perror("listen call error.");
		return -1;
	}

	while(1)
	{
		printf("waiting for connecting.\n");
		
		addrlen=sizeof(client_addr);
		client_sockfd=accept(server_sockfd, (struct sockaddr*)&client_addr, &addrlen);
		if(client_sockfd<0)
		{
			perror("connect error.\n");
			break;
		}

		
		cnt=1;
		while(1)
		{
			
			err=read(client_sockfd, recv_buff, BUFSIZ-1);
			if(err<=0)
			{
				printf("connection close or exist error.\n");
				break;
			}
			printf("%s\n", recv_buff);
			
			snprintf(send_buff, BUFSIZ,"server current cnt is %d",cnt);
			cnt+=2;
			
			err=write(client_sockfd, send_buff, strlen(send_buff)+2);
			if(err<=0)
			{
				printf("connection close or exist error.\n");
				break;
			}
		}
		
	}

	close(client_sockfd);
	close(server_sockfd);
	return 0;
}