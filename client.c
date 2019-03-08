#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "json-c/json.h"

#define DHMP_SERVER_NODE_NUM 4

json_object *global_jobj;

void test_json()
{
	int64_t dram_free_size, dram_total_size, nvm_free_size, nvm_total_size;
	int i;
	char server_name[25];
	
	json_object *dram_free_size_arr[DHMP_SERVER_NODE_NUM],
				*dram_total_size_arr[DHMP_SERVER_NODE_NUM],
				*nvm_free_size_arr[DHMP_SERVER_NODE_NUM],
				*nvm_total_size_arr[DHMP_SERVER_NODE_NUM],
				*tmp;
	
	dram_free_size=100;
	dram_total_size=1100;
	nvm_free_size=200;
	nvm_total_size=1200;
	
	for(i=0; i<DHMP_SERVER_NODE_NUM; i++)
	{
		dram_free_size_arr[i]=json_object_new_int64(dram_free_size);
		dram_total_size_arr[i]=json_object_new_int64(dram_total_size);
		nvm_free_size_arr[i]=json_object_new_int64(nvm_free_size);
		nvm_total_size_arr[i]=json_object_new_int64(nvm_total_size);

		dram_free_size+=200;
		dram_total_size+=400;
		nvm_free_size+=200;
		nvm_total_size+=400;
	}

	global_jobj=json_object_new_object();
	
	for(i=0;i <DHMP_SERVER_NODE_NUM; i++)
	{
		snprintf(server_name, 24, "server%d",i+1);

		tmp=json_object_new_object();
		json_object_object_add(tmp, "dram free size", dram_free_size_arr[i]);
    	json_object_object_add(tmp, "dram total size", dram_total_size_arr[i]);
    	json_object_object_add(tmp, "nvm free size", nvm_free_size_arr[i]);
    	json_object_object_add(tmp, "nvm total size", nvm_total_size_arr[i]);

		json_object_object_add(global_jobj, server_name, tmp);
	}
	
    //json_object_to_file("new.json", global_jobj);

}

int main(int argc, char *argv[])
{
	struct sockaddr_in server_addr;
	int client_sockfd, err;
	char recv_buff[BUFSIZ];
	const char *data_ptr;
	
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=inet_addr("192.168.206.129");
	server_addr.sin_port=htons(39400);

	test_json();
	
	client_sockfd=socket(AF_INET, SOCK_STREAM, 0);
	if(client_sockfd<0)
	{
		perror("socket call error.");
		return -1;
	}

	
	err=connect(client_sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(err)
	{
		perror("connect call error.");
		return -1;
	}
	
	while(1)
	{
		data_ptr=json_object_to_json_string(global_jobj);
		err=write(client_sockfd, data_ptr, strlen(data_ptr)+5);
		if(err<=0)
		{
			printf("connection close or exist error.\n");
			break;
		}
		
		read(client_sockfd, recv_buff, BUFSIZ-1);
		if(err<=0)
		{
			printf("connection close or exist error.\n");
			break;
		}
		
		printf("%s\n",recv_buff);
		
		sleep(2);
	}

	close(client_sockfd);
	return 0;
}
