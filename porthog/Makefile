# This Works is placed under the terms of the Copyright Less License,
# see file COPYRIGHT.CLL.  USE AT OWN RISK, ABSOLUTELY NO WARRANTY.

CFLAGS=-Wall -O3 -D_GNU_SOURCE

PROGS=porthog

.PHONY: all
all:	$(PROGS)

.PHONY: install
install: $(PROGS)
	for a in $(PROGS); do cmp -s "$$a" "/usr/local/bin/$$a" || cp --backup=t -f "$$a" /usr/local/bin/; done

.PHONY: clean distclean
clean distclean:
	rm -f $(PROGS) *.o *~

