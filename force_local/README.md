Force Local
===========

Force a program to listen to localhost.


Compile:
--------

```bash
make
make install
```


Environment:
------------

These are the default environment settings used:
```
LD_PRELOAD=/path/to/force_local.so
FORCE_LOCAL_ADDR=127.0.0.1
FORCE_LOCAL_PORT=0
FORCE_LOCAL_FROM_ADDR='*'
FORCE_LOCAL_FROM_PORT=0
```
- If `PORT`s are not 0, they are matched/set.
- If `FORCE_LOCAL_FROM_ADDR` is given (and does not start with a '*'), it is matched against the address to fix.


Example:
--------

**Make VBoxHeadless listen on localhost**

Set FORCE_LOCAL_ADDR if you want a different address than 127.0.0.1.

```bash
LD_PRELOAD=/usr/local/lib/force_local.so /usr/lib/virtualbox/VBoxHeadless --startvm "VM name" --vnc --vncport 2000 --vncpass "pw" --width 800 --height 600
```
The downside of this is that you must run VBoxHeadless as root to get this fixed as it is suid root.

**Fix SQUID listening on random port**

See also http://permalink.de/tino/squid

```bash
/etc/init.d/squid stop

echo "udp_outgoing_address 127.255.255.254" >> /etc/init.d/squid.conf

mv -i /usr/sbin/squid /usr/sbin/squid.orig

cat > /usr/sbin/squid <<"EOF"
export FORCE_LOCAL_FROM_ADDR=127.255.255.254
export FORCE_LOCAL_PORT=65535
export LD_PRELOAD=/path/to/force_local.so
/usr/sbin/squid.orig "$@"
EOF
chmod +x /usr/sbin/squid

/etc/init.d/squid start
```


Bugs:
-----

This is not yet IPv6 aware.  However I not yet came around the need to fix IPv6 applications, all those buggy apps which need fixing only use IPv4.

You can only fix one single specific address/port combination this way yet.  Currently I did not came around applications, which needed more.

This requires a `bind()` call.  If the program does not `bind`, it cannot be fixed with this method.  `strace` is your friend.


License:
--------

This Works is placed under the terms of the Copyright Less License,
see file COPYRIGHT.CLL.  USE AT OWN RISK, ABSOLUTELY NO WARRANTY.
