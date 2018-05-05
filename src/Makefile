# Miscellaneous experiments

CFLAGS=-Wall -O3 -g
STATICFLAGS=-Wall

INSTALLS=ipof gethostname printresugid checknul umount2-l fs_ioc_fiemap
PROGS=udp $(INSTALLS)
LDLIBS=-lrt	# for checknul
STATICS=ipof.static
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
