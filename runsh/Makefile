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
	for a in $(BINS); do cmp -s "$$a" "$(PREFIX)/bin/$$a" || cp --backup=t -f "$$a" "$(PREFIX)/bin/"; done

.PHONY: clean distclean
clean distclean:
	rm -f *.o *~ $(BINS)

