# This Works is placed under the terms of the Copyright Less License,
# see file COPYRIGHT.CLL.  USE AT OWN RISK, ABSOLUTELY NO WARRANTY.

# This is for users on broken systems.  Hence we must assume this:
# - Home is on a really borken thing like Samba, so do not rely on softlinks
# - Non-root users, so you cannot install anything outside $HOME
# - Your git is on $HOME/bin/git.orig, /usr/bin/git or /usr/local/bin/git
# - If git is on $HOME/bin/git, then it will be renamed $HOME/bin/git.orig
INSTALLPREFIX=$(HOME)

CFLAGS=-Wall -O3 -D_GNU_SOURCE -fPIC

SLIB=git_push_samba.so
OBJS=$(SLIB:.so=.o)

all:	$(SLIB)

.SUFFIXES: .so

.o.so:
	gcc -o $@ -nostartfiles -shared $< -ldl

$(OBJS): Makefile

install: $(SLIB)
	mkdir -p '$(INSTALLPREFIX)/lib' && cp -p $(SLIB) '$(INSTALLPREFIX)/lib/'
	[ -x '$(INSTALLPREFIX)/bin/git.orig' ] || rm -f '$(INSTALLPREFIX)/bin/git.orig'
	test ".`grep ^#MAGIC# git`" = ".`grep ^#MAGIC# '$(INSTALLPREFIX)/bin/git' 2>/dev/null`" || mv -i --backup=t '$(INSTALLPREFIX)/bin/git' '$(INSTALLPREFIX)/bin/git.orig' || :
	rm -f '$(INSTALLPREFIX)/bin/git'
	sed "s#''#'$(INSTALLPREFIX)/lib'#g" git > '$(INSTALLPREFIX)/bin/git'
	chmod 755 '$(INSTALLPREFIX)/bin/git'

clean:
	rm -f $(OBJS) *~

