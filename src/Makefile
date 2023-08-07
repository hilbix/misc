# Miscellaneous experiments

CFLAGS=-Wall -O3 -g
STATICFLAGS=-Wall

SRCS=$(wildcard *.c)
PROGS=$(SRCS:.c=)

LDLIBS=-lrt	# for checknul
STATICS=ipof.static
INSTALLPREFIX=/usr/local

.PHONY: all
all:	$(PROGS) $(STATICS)

.PHONY: install
install:	$(PROGS)
	for a in $(PROGS); do cmp -s "$$a" "$(INSTALLPREFIX)/bin/$$a" || cp --backup=t -f "$$a" "$(INSTALLPREFIX)/bin/"; done

.SUFFIXES: .static
.c.static:
	@echo ===== Please ignore the warning about printf =====
	diet -v -Os cc $(STATICFLAGS) $< -o $@
	cp $@ $@.debug
	strip -s $@

.PHONY: clean distclean
clean distclean:
	rm -f *.o *~ $(PROGS) $(STATICS) *.debug
