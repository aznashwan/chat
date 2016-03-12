FLAGS=-Wall
COMPILER=clang
# COMPILER=gcc -pthread

BASEFILES={socklib,debug,auth,termlib,sync}


all: server client

server:
	$(COMPILER) $(FLAGS) -o server server.c $(BASEFILES).c
client:
	$(COMPILER) $(FLAGS) -o client client.c $(BASEFILES).c

clean: clean
	rm -f *.o *~
	rm -f server client
