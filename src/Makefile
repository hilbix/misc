# Miscellaneous experiments

CFLAGS=-Wall -O3

PROGS=udp ipof

all:	$(PROGS)

install:	ipof
	cp $< /usr/local/bin/

clean:
	rm -f *.o *~ $(PROGS)
