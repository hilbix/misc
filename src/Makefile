# Miscellaneous experiments

CFLAGS=-Wall -O3 -g
STATICFLAGS=-Wall

PROGS=udp ipof gethostname printresugid checknul
LDLIBS=-lrt	# for checknul
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
	cp $@ $@.debug
	strip -s $@

clean:
	rm -f *.o *~ $(PROGS) $(STATICS) *.debug
