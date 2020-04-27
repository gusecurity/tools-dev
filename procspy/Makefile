CC     := musl-gcc
CFLAGS := -std=c99 -D_DEFAULT_SOURCE -D_GNU_SOURCE -g

procspy: dynarr.o procspy.o
	$(CC) -static $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -f *.o procspy
