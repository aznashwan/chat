FLAGS=-Wall
COMPILER=clang
# COMPILER=gcc -pthread

BASEFILES={socklib,debug,auth}


auth: cp authserver authclient

authserver:
	$(COMPILER) $(FLAGS) -o authserver authserver.c $(BASEFILES).c
authclient:
	$(COMPILER) $(FLAGS) -o authclient authclient.c $(BASEFILES).c

cp:
	cp ../../$(BASEFILES).{c,h} .

clean: clean
	rm -f *.o *~
	rm -f authserver authclient
	rm -f $(BASEFILES).{c,h}
