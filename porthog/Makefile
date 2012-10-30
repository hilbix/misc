# This Works is placed under the terms of the Copyright Less License,
# see file COPYRIGHT.CLL.  USE AT OWN RISK, ABSOLUTELY NO WARRANTY.

CFLAGS=-Wall -O3 -D_GNU_SOURCE

PROGS=porthog

all:	$(PROGS)

install: $(PROGS)
	cp -f $(PROGS) /usr/local/sbin/

clean:
	rm -f $(PROGS) *.o *~

