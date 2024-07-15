CFLAGS = -Werror -Wall -Wpedantic -g3
OFLAGS = -O0 
LDFLAGS = -lm -lcheck

complexTests: complexTests.c complex.o
	$(CC) $(CFLAGS) $(OFLAGS) $< $(LDFLAGS) -o complexTests

complex.o: complex.h complex.c
	$(CC) $(CFLAGS) $(OFLAGS) $< $(LDFLAGS) -c -o complex.o 


clean: 
	rm *.o
	rm *.h.gch 
	rm complexTests
