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
	cp -f $(SLIB) /usr/local/lib/

.PHONY: clean distclean
clean distclean:
	rm -f $(SLIB) $(OBJS) *~

