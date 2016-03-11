FLAGS=-Wall
COMPILER=clang

BASEFILES=socklib.c auth.c debug.c


auth: authserver authclient

authserver:
	$(COMPILER) $(FLAGS) -o authserver authserver.c $(BASEFILES)
authclient:
	$(COMPILER) $(FLAGS) -o authclient authclient.c $(BASEFILES)

clean: clean
	rm -f *.o *~
	rm -f authserver authclient
