CC = gcc
CFLAGS = -Werror -Wall -Wpedantic -g3
OFLAGS = -O0 
LDFLAGS = -lm -lcheck

complexTests: complexTests.o complex.o
	$(CC) $(CFLAGS) $(OFLAGS) complexTests.o complex.o $(LDFLAGS) -o complexTests

complex.o: complex.c complex.h 
	$(CC) $(CFLAGS) $(OFLAGS) -c $< $(LDFLAGS) -o $@

complexTests.o: complexTests.c 
	$(CC) $(CFLAGS) $(OFLAGS) -c $< $(LDFLAGS) -o $@


clean: 
	rm -rf *.o
	rm -rf *.h.gch 
	rm -rf complexTests
