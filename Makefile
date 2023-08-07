#
# Dummy Makefile added

SUBS=	\
	force_local \
	porthog	\
	runsh	\
	src	\
	wrap	\
	openssl	\

.PHONY: all clean distclean
all clean distclean install:
	for a in $(SUBS); do make -C "$$a" $@ || { echo; echo "bug $$a"; echo; exit 1; }; done
