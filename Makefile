.PHONY:clean

CC :=gcc
CFLAGS := -Wall -g 
LDFLAGS	:= ${LDFLAGS} -ljson-c

APPS := client server

all:${APPS}
	
client:client.o
	gcc -Wall -g $^ -o $@ ${LDFLAGS}

server:server.o
	gcc -Wall -g $^ -o $@ ${LDFLAGS}
	
clean:
	rm -rf *.o client server