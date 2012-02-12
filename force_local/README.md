Force Local
===========

Force a program to listen to localhost.

Compile:
--------

```bash
make
make install
```

Example:
--------

```bash
LD_PRELOAD=/usr/local/lib/foce_local.so /usr/lib/virtualbox/VBoxHeadless --startvm "VM name" --vnc --vncport 2000 --vncpass "pw" --width 800 --height 600
```
The downside of this is that you must run VBoxHeadless as root to get this fixed as it is suid root.


License:
--------

This Works is placed under the terms of the Copyright Less License,
see file COPYRIGHT.CLL.  USE AT OWN RISK, ABSOLUTELY NO WARRANTY.
