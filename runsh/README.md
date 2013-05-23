runsh, shell for running programs
=================================

`runsh` solves following:

You want to quickly invoke some command from your Linux' laptop login prompt.  This command needs root.  But you do not want to create a unix shell account for this which can be used or abused.


Usage:
------

```bash
sudo apt-get install dietlibc-dev

git clone https://github.com/hilbix/misc.git
cd misc/runsh
make
sudo make install

sudo su -

mkdir .runsh
cd .runsh

vim USER.runsh
chmod +x USER.runsh

vim /etc/passwd
pwconv
```

Duplicate `root` entry to USER, set it to no password and give it `/usr/local/sbin/runsh` as login shell.  Done.


Note:
-----

You probably don't want to use `root`, instead some more restricted user and sudo.  Do what you want, but don't blame me if it breaks.  You have been warned.

`runsh` supports SUID.  In that case it uses `$HOME/.runsh/` from the effective user (the one it is set to be SUID).  LOGNAME must be set to some user which maps to the user id or the effective user id, else it takes the (first found) name of the calling user id.


Rationale:
----------

It works.  Don't cosider this to be secure!


License:
--------

This Works is placed under the terms of the Copyright Less License,
see file COPYRIGHT.CLL.  USE AT OWN RISK, ABSOLUTELY NO WARRANTY.

