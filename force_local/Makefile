# This Works is placed under the terms of the Copyright Less License,
# see file COPYRIGHT.CLL.  USE AT OWN RISK, ABSOLUTELY NO WARRANTY.

CFLAGS=-Wall -O3 -D_GNU_SOURCE -fPIC

SLIB=force_local

all:	$(SLIB).so

$(SLIB).so:	$(SLIB).o
	gcc -o $(SLIB).so -nostartfiles -shared $(SLIB).o -ldl

$(SLIB).o: Makefile

install: $(SLIB).so
	cp -f $(SLIB).so /usr/local/lib/

clean:
	rm -f *.so *.o *~

