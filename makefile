CC=gcc
CFLAGS=-W -Wall -g -fpic
LDFLAGS=
LDFLAGS_STATIC=-L. -lbfile
LDFLAGS_DYN=lbfile.so.1
EXEC=main_static main_dyn
LIBS=lbfile.a lbfile.so.1

all: $(LIBS) $(EXEC) 

main_dyn: main.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDFLAGS_DYN)

main_static: main.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDFLAGS_STATIC)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

lbfile.a: bfile.o format_in_out.o
	ar q lbfile.a bfile.o format_in_out.o

lbfile.so.1: bfile.o format_in_out.o
	gcc -shared -o lbfile.so.1 bfile.o format_in_out.o
	export LD_LIBRARY_PATH=.

$(LIBS): lbfile.a lbfile.so.1

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC) lbfile.so.1 lbfile.a 
