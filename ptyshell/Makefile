CC     := musl-gcc
CFLAGS := -static

ptyshell: ptyshell.c
	$(CC) $(CFLAGS) -o $@ $^
	strip $@
