OBJS = pipe.o

CFLAGS = -std=c11 -Wpedantic -Wall -O2 -pipe
LDFLAGS = -Wl,-O1,--sort-common,--as-needed,-z,relro,-z,now

pipe: ${OBJS}

.PHONY: clean
clean:
	rm -f ${OBJS} pipe
