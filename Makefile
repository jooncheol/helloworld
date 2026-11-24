hello: hello.o
	gcc -o $@ $<

hello.o: hello.c
	gcc -o $@ -c $<

all: hello

clean:
	rm -f hello *.o a.out
