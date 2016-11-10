CFLAGS=-Wall
LDFLAGS=-static
PREFIX=/usr/local
SBINDIR=/sbin
BINS=runsh

# Compile it with dietlibc, such that it does not need dynamically loaded glibc wrappers
CC=diet -v -Os cc

.PHONY: all
all:	$(BINS)

.PHONY: install
install:
	strip -s $(BINS)
	cp $(BINS) $(PREFIX)$(SBINDIR)

.PHONY: clean distclean
clean distclean:
	rm -f *.o *~ $(BINS)
