CC =  /opt/csw/gcc3/bin/gcc
LDFLAGS =
CFLAGS =
CPPFLAGS =
MAKE = make

all: prob3

../lib/csim.a:
	(cd lib && $(MAKE))

prob3: prob3.c ../lib/csim.a
	$(CC) -Ilib $(LDFLAGS) $(CFLAGS) $(CPPFLAGS) -o prob3 prob3.c ../lib/csim.a -lm

clean:
	rm -f prob3
