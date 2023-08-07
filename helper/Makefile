#

.PHONY:	love
love:	all

.PHONY:	all
all:

.PHONY:	install
install:
	for a in *.sh; do cmp -s "$$HOME/bin/$$a" "$$a" || ln --force --backup=t -vs --relative "$$a" "$$HOME/bin/"; done

