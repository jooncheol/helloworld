bomber: bomber.o
	gcc -o $@ $<

bomber.o: bomber.c
	gcc -o $@ -c $<

all: bomber

clean:
	rm -f bomber *.o a.out
