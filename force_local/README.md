Force Local
===========

Force a program to listen to localhost (force_locel).  Force a program to bin to a port (force_bind).

Note that these wrappers are not meant to be perfect.  They are meant to be adjusted to your needs.
Use the source, it's free, as in free speech, free beer and free men.


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
LD_PRELOAD=/usr/local/lib/force_local.so
FORCE_LOCAL_ADDR=127.0.0.1
FORCE_LOCAL_PORT=0
FORCE_LOCAL_FROM_ADDR='*'
FORCE_LOCAL_FROM_PORT=0
```
- If `PORT`s are not 0, they are matched/set.
- If `FORCE_LOCAL_FROM_ADDR` is given (and does not start with a '*'), it is matched against the address to fix.


```
LD_PRELOAD=/usr/local/lib/force_bind.so
FORCE_BIND_ADDR=127.0.0.1
FORCE_BIND_PORT=0
FORCE_BIND_SOCKDOM=2		# PF_INET/AF_INET
FORCE_BIND_SOCKTYPE=2		# SOCK_DGRAM
FORCE_BIND_SOCKPROTO=17		# IPPROTO_UDP, see /etc/protocols
```
- You can find the constants via `grep IPPROTO_UDP /usr/include/*/*.h`


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
mkdir -p /usr/sbin/orig
cat > /usr/sbin/orig/squid.wrapper <<"EOF"
export FORCE_LOCAL_FROM_ADDR=127.255.255.254
export FORCE_LOCAL_PORT=65535
export LD_PRELOAD=/usr/local/lib/force_local.so
exec /usr/sbin/orig/squid "$@"
EOF
chmod +x /usr/sbin/orig/squid.wrapper

/etc/init.d/squid stop

echo "udp_outgoing_address 127.255.255.254" >> /etc/init.d/squid.conf

mv -i /usr/sbin/squid /usr/sbin/orig/
ln -s orig/squid.wrapper /usr/sbin/squid

/etc/init.d/squid start
```

**Secure rsyslogd to listen on a fixed UDP port**

Rsyslogd does not bind to a specific UDP port if talking to another syslogd.  This is bad in a firewalled environment, as you want to have a static communication matrix such that you can add a static ACL on the dedicated firewall.  This here fixes it:

```bash
mkdir -p /usr/sbin/orig
cat > /usr/sbin/orig/rsyslogd.wrapper <<"EOF"
export FORCE_BIND_ADDR=\*
export FORCE_BIND_PORT=65534
export LD_PRELOAD=/usr/local/lib/force_bind.so
exec /usr/sbin/orig/rsyslogd "$@"
EOF
chmod +x /usr/sbin/orig/rsyslogd.wrapper

/etc/init.d/rsyslogd stop

mv -i /usr/sbin/rsyslogd /usr/sbin/orig/
ln -s orig/rsyslogd.wrapper /usr/sbin/rsyslogd

/etc/init.d/rsyslogd start
```

Bugs:
-----

This only works on appications using standard library calls for creating the socket.  If it does direct kernel calls, you are out of luck.

This is not yet IPv6 aware.  However I never had the need to fix IPv6 applications yet, all those buggy apps which need fixing are IPv4 only.

You can only fix one single specific address/port combination this way yet.  Currently I did not came around applications, which needed more.
However you have the source and it's free to use.  Feel free to implement your own variant.

`strace` is your friend to find out if you need force_local (fixing the `bind()` call) or force_bind (if there is no `bind()` call at all).


License:
--------

This Works is placed under the terms of the Copyright Less License,
see file COPYRIGHT.CLL.  USE AT OWN RISK, ABSOLUTELY NO WARRANTY.
