C = gcc
LDFLAGS =
CFLAGS =
CPPFLAGS =
MAKE = make

all: q11 q12 q2 q3 

lib/csim.a:
	(cd lib && $(MAKE))

q11: q11.c lib/csim.a
	$(CC) -Ilib $(LDFLAGS) $(CFLAGS) $(CPPFLAGS) -o q11 q11.c lib/csim.a -lm
q12: q12.c lib/csim.a
	$(CC) -Ilib $(LDFLAGS) $(CFLAGS) $(CPPFLAGS) -o q12 q12.c lib/csim.a -lm
q2: q2.c lib/csim.a
	$(CC) -Ilib $(LDFLAGS) $(CFLAGS) $(CPPFLAGS) -o q2 q2.c lib/csim.a -lm
q3: q3.c lib/csim.a
	$(CC) -Ilib $(LDFLAGS) $(CFLAGS) $(CPPFLAGS) -o q3 q3.c lib/csim.a -lm
clean:
	rm -f q11 q2 q3 q12
