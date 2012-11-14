# Miscellaneous experiments

CFLAGS=-Wall -O3
STATICFLAGS=-Wall

PROGS=udp ipof  printresugid
STATICS=ipof.static
INSTALLS=ipof printresugid
INSTALLPREFIX=/usr/local

all:	$(PROGS) $(STATICS)

install:	$(INSTALLS)
	cp $(INSTALLS) $(INSTALLPREFIX)/bin/

.SUFFIXES: .static
.c.static:
	@echo ===== Please ignore the warning about printf =====
	diet -v -Os cc $(STATICFLAGS) $< -o $@
	strip -s $@

clean:
	rm -f *.o *~ $(PROGS) $(STATICS)
