Quick'n'dirty sources
=====================

- udp.c is just a stub for an UDP sender, nothing spectacular, perhaps even too simple to be useful.

- ipof.c see below, with `make static` it currently is without IDN


IPOF
----

`dig` and `host` are too expensive or difficult to use?  Then try `ipof`.  It has less than 8K source and is prepeared for scripting usage.

Examples:

- `ipof -h -` interactive mode

- `producer | ipof -v -a -+ | consumer` resolve hostnames to "hostname IP..." lines, hostnames starting with - are not mistaken as options

- `ipof -q -t -- hostname` returns the status if hostname can be resolved, no output at all

