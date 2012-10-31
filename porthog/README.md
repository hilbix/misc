Force buggy services to a static port
=====================================

Here is another fix in the neverending story about how to make standard system services well behave, such that you can apply static firewall rules to them.  This time:

* rpcbind

In `libtirpc-0.2.2/src/bindresvport.c` there is missing an option to make it listen to a single static port (for example by looking at the environment).  This can be considered a bug, anyway, as this might make rpcbind listen on a reserved port number.

This time it is difficult to fix the syscall, as it could come in an unknown sequence with an unknown port.  No much room for a hack like `force_local` (which isn't IPv6 ready yet anyway).  So here is another method.


FIX: Allocate all ports which must not be used
----------------------------------------------

`porthog` allocates ranges of UDP ports on a given IP (by default 127.0.0.2) and forks a program.  Afterwards it closes the ports again.

Sounds silly and is silly of course.  But I did not get a better idea, except to replace the `bindresvport_sa()` routine of libirpc via `LD_PRELOAD`.


Usage:
------

`porthog range[,range...] program [args...]` where `range` is `[proto:]port[-port][@ip]` where `proto` is `tu46` by default, which means tcp4, udp4, tcp6 and udp6 alltogether.

If `porthog` runs out of file descriptors, it `fork()`s to raise the per-process limit.  However there is a slight chance that for some operations the library needs a file descriptor while all are used up.  In this case you have to re-arrange the `range` appropriately.


Bugs:
-----

`porthog` assumes that the program does not daemonize before it has allocated the ports.

If some other process tries to allocate ports while `porthog` is active, it may fail.  You can perhaps help by using different interfaces, in case you can make the fixed program listen on the same interface or on `*`.


Example:
--------

`sudo strace porthog 1-65535 netstat -natup 2>TRC >NST` tries to open all ports for TCP, UDP, IPv4 and IPv6.  **Warning!** Only try this if you are sure, your machine has enough resources.  This writes around 25MB in the files.

Fixing `rpcbind` involves editing `/etc/init.d/rcbind`, because you cannot wrap the name `/sbin/rpcbind` as `rpcbind` must run under this name which is hardcoded in the script.  The changes are:

* copy `/usr/local/sbin/porthog` to `/sbin/porthog` to make it available before `/usr` is mounted.
* alter one line as follows:

original:
```
    start-stop-daemon --start --quiet --oknodo --exec /sbin/rpcbind -- "$@"
```
to:
```
    start-stop-daemon --start --quiet --oknodo --exec /sbin/porthog -- u:500-800,802-1023 /sbin/rpcbind "$@"
```


License:
--------

This Works is placed under the terms of the Copyright Less License,
see file COPYRIGHT.CLL.  USE AT OWN RISK, ABSOLUTELY NO WARRANTY.
