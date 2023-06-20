CC = gcc
CFLAGS = -O2 -std=c11 -pedantic -Wall -Wextra -Werror -Wno-unused-parameter -D_DEFAULT_SOURCE

CLIENT = client/main.c

SERVER = server/main.c

clnt: $(CLIENT) 
		$(CC) $(CFLAGS) -o $@ $^

srvr: $(SERVER)
		$(CC) $(CFLAGS) -o $@ $^
	
.PHONY: run
run:
	make clnt
	make srvr
.PHONY: clean

clean:
	rm -rf clnt srvr