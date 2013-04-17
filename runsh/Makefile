CFLAGS=-Wall
LDFLAGS=-static
PREFIX=/usr/local
SBINDIR=/sbin
BINS=runsh

# Compile it with dietlibc, such that it does not need dynamically loaded glibc wrappers
CC=diet -v -Os cc

all:	$(BINS)

install:
	strip -s $(BINS)
	cp $(BINS) $(PREFIX)$(SBINDIR)

clean:
	rm -f *.o *~ $(BINS)
