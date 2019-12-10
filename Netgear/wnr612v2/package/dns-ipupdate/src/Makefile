#CC = gcc

CFLAG = -O2
APP_LIBS = -L. 
SO_LIBS = -L. -llink
INCPATH = -I.
LFLAGS = -shared
SRC = base64.c    generate.c  main.c  phglobal.c  phupdate.c \
	blowfish.c  log.c       md5.c   phsocket.c

all: updatednsip

clean:
	rm -f *.o
	rm -f updatednsip

updatednsip: $(SRC)
	$(CC) $(CFLAG) $(INCPATH) $(APP_LIBS) $(LDFLAGS) $(EXTRA_LDFLAGS) -lconfig -o updatednsip $(SRC)
