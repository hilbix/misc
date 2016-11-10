# This Works is placed under the terms of the Copyright Less License,
# see file COPYRIGHT.CLL.  USE AT OWN RISK, ABSOLUTELY NO WARRANTY.

CFLAGS=-Wall -O3 -D_GNU_SOURCE

PROGS=wrap
WRAPS=VBoxHeadless.wrap.dist
INSTPATH=/wrap

.PHONY: all
all:	$(PROGS)

.PHONY: install
install: $(PROGS)
	mkdir -p $(INSTPATH)
	strip -s $(PROGS)
	cp -f $(PROGS) $(WRAPS) $(INSTPATH)/

example: $(WRAPS)
	-echo n | ln -is VBoxHeadless.wrap.dist $(INSTPATH)/VBoxHeadless.wrap
	-echo n | cp -f wrap $(INSTPATH)/VBoxHeadless
	chown --reference /usr/lib/virtualbox/VBoxHeadless $(INSTPATH)/VBoxHeadless
	chmod --reference /usr/lib/virtualbox/VBoxHeadless $(INSTPATH)/VBoxHeadless

.PHONY: clean distclean
clean distclean:
	rm -f $(PROGS) *.o *~

