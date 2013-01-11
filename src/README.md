Quick'n'dirty sources
=====================

- `udp.c` is just a stub for an UDP sender, nothing spectacular, perhaps even too simple to be useful.

- `ipof.c` see below, `ipof.static` currently is without IDN (International Domain Names) support

- `gethostname` calls gethostname(2), in contrast to `hostname` which uses `uname(2)`

- `printresugid.c` just prints the real, effective and saved UID and GID.  Can be used for SUID experiments.


IPOF
----

`dig` and `host` are too expensive or difficult to use?  Then try `ipof`.  It has less than 8K source and is prepeared for scripting usage.

Examples:

- `ipof -h -` interactive mode

- `producer | ipof -v -a -+ | consumer` resolve hostnames to "hostname IP..." lines, hostnames starting with - are not mistaken as options

- `ipof -q -t -- hostname` returns the status if hostname can be resolved, no output at all

- `ipof -v -f -l -a -- hostname` outputs all IP addresses of the given hostname

Notes
-----

There can be a bug in dietlibc, which makes `ipof.static` enter an endless loop in case `/etc/resolv.conf` contains irregular characters like `_` (underscore) in the `search` or `domain` string.  These irregular entries can happen in any `DHCP` setup easily.  The bug is in the routine `__dns_readstartfiles` of `dietlibc`.  This bug combined with some forged `DHCP` packets this gives us a DoS.  The nonstatic version is not affected.

