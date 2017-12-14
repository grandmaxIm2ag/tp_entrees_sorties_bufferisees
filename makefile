CC=gcc
CFLAGS=-W -Wall -g -fpic
LDFLAGS=
LDFLAGS_STATIC=lbfile.a
LDFLAGS_DYN=lbfile.so.1
EXEC=main_dyn main_static main generator test_format

all: $(LIBS) $(EXEC) 

main:main.o bfile.o format_in_out.o
	$(CC) -o $@ $^ $(LDFLAGS)

test_format:test_format.o bfile.o format_in_out.o
	$(CC) -o $@ $^ $(LDFLAGS)

generator: generator.o 
	$(CC) -o $@ $^ $(LDFLAGS)

main_dyn: main.o lbfile.so.1
	$(CC) -o $@ $^ $(LDFLAGS) $(LDFLAGS_DYN)
	export LD_LIBRARY_PATH=.

main_static: main.o lbfile.a
	$(CC) -o $@ $^ $(LDFLAGS) $(LDFLAGS_STATIC)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

lbfile.a: bfile.o format_in_out.o
	ar rcs lbfile.a bfile.o format_in_out.o

lbfile.so.1: bfile.o format_in_out.o
	gcc -shared -o lbfile.so.1 bfile.o format_in_out.o

libs: lbfile.a lbfile.so.1

.PHONY:libs

tests: all
	./script_test.sh
clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC) lbfile.so.1 lbfile.a 
