# This Works is placed under the terms of the Copyright Less License,
# see file COPYRIGHT.CLL.  USE AT OWN RISK, ABSOLUTELY NO WARRANTY.

CFLAGS=-Wall -O3 -D_GNU_SOURCE -fPIC

SLIB=force_local.so force_bind.so
OBJS=$(SLIB:.so=.o)

.PHONY: all
all:	$(SLIB)

.SUFFIXES: .so

.o.so:
	gcc -o $@ -nostartfiles -shared $< -ldl

$(OBJS): Makefile

.PHONY: install
install: $(SLIB)
	for a in $(SLIB); do cmp -s "$$a" "/usr/local/lib/$$a" || cp -f "$$a" /usr/local/lib/; done

.PHONY: clean distclean
clean distclean:
	rm -f $(SLIB) $(OBJS) *~

