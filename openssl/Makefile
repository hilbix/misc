# Miscellaneous experiments

CFLAGS=-Wall -O3 -g
STATICFLAGS=-Wall

INSTALLS=B64 #AESctr
PROGS=$(INSTALLS)
LDLIBS=-lssl -lcrypto
STATICS=
INSTALLPREFIX=/usr/local

.PHONY: all
all:	$(PROGS) $(STATICS)

.PHONY: install
install:	$(INSTALLS)
	cp $(INSTALLS) $(INSTALLPREFIX)/bin/

.SUFFIXES: .static
.c.static:
	@echo ===== Please ignore the warning about printf =====
	diet -v -Os cc $(STATICFLAGS) $< -o $@
	cp $@ $@.debug
	strip -s $@

.PHONY: clean distclean
clean distclean:
	rm -f *.o *~ $(PROGS) $(STATICS) *.debug

