#
# Dummy Makefile added

SUBS=	\
	force_local \
	porthog	\
	runsh	\
	src	\
	wrap	\

.PHONY: all clean

all clean:
	for a in $(SUBS); do make -C "$$a" $@ || { echo; echo "bug $$a"; echo; exit 1; }; done
