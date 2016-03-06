FLAGS=-Wall
COMPILER=clang

BASEFILES=socklib.c debug.c


echo: echoserver echoclient

echoserver:
	$(COMPILER) $(FLAGS) -o echoserver echoserver.c $(BASEFILES)
echoclient:
	$(COMPILER) $(FLAGS) -o echoclient echoclient.c $(BASEFILES)

clean: clean
	rm -f *.o *~
	rm -f echoserver echoclient
