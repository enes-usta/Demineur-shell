CC=gcc
CFLAGS=-Wall -std=c99
EXEC=demineur

all : $(EXEC)

demineur : demineur.o Profil.o main.o
	$(CC) $(CFLAGS) -o $@ $^

main.o : demineur.h Profil.h

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean :
	rm -rf *.o

mrproper : clean
	rm -rf $(EXEC)
