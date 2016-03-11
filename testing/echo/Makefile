FLAGS=-Wall
COMPILER=clang

BASEFILES={socklib,debug}


echo: cp echoserver echoclient

echoserver:
	$(COMPILER) $(FLAGS) -o echoserver echoserver.c $(BASEFILES).c
echoclient:
	$(COMPILER) $(FLAGS) -o echoclient echoclient.c $(BASEFILES).c

cp:
	cp ../../$(BASEFILES).{c,h} .

clean: clean
	rm -f *.o *~
	rm -f echoserver echoclient
	rm -f $(BASEFILES).{c,h}
