CC = gcc
CFLAGS = -W -Wall -O2
LDFLAGS = -lm

all : server client server2

server : tcp_server.c 
				$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
				
client : tcp_client.c
				$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
				
server2 : tcp_server2.c
				$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
				
clean: 
		@rm -rf *.o server client server2
