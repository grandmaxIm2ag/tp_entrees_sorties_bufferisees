CC=gcc
CFLAGS=-W -Wall -g
LDFLAGS=
EXEC=main

all: $(EXEC)

main: bfile.o format_in_out.o main.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
