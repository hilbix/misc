SUID root wrapper
=================

Environment mapper for SUID root programs.

For a bit more generic program, see [`suid`](https://github.com/hilbix/suid)


Example:
----------

`LD_PRELOAD` does not work with SUID root programs because it is too dangerous for such a thing.  Therefor to apply fixes like `../force_local/` to SUID root programs like VBoxManage you need a helper.  `wrap` does exactly this.

Instead invoking something like
```
LD_PRELOAD=/usr/local/lib/force_local.so /usr/lib/virtualbox/VBoxHeadless --startvm "VM name" --vnc --vncport 2000 --width 800 --height 600
```
you do following:

- Create directory `/wrap`
- Create a copy of `./wrap` to `/wrap/VBoxHeadless`
  - hardlinks work, too
- Set the permissions and owner of `/wrap/VBoxHeadless` to exactly the same as `/usr/lib/virtualbox/VBoxHeadless`
- Create a textfile named `/wrap/VBoxHeadless.wrap`
- The first line of `/wrap/VBoxHeadless.wrap` reads `/usr/lib/virtualbox/VBoxHeadless`
- The following lines of this files contain the environment to add to the called program.  In this case `LD_PRELOAD=/usr/local/lib/force_local.so`
  - The lines cannot contain NUL bytes, as NUL is considered EOF, too
  - The lines must be full lines including the `LF`
  - This is for safety.  If a file is truncated/damaged, NUL bytes might show up, which invalidates the current line

You then can call `/wrap/VBoxHeadless` instead of `LD_PRELOAD=/usr/local/lib/force_local.so /usr/lib/virtualbox/VBoxHeadless`, even from a normal user.
 

Usage:
------

```
make
sudo make install
sudo make example
cd ../force_local
make
sudo make install
```


Bugs:
-----

If the file contains CRLF sequences (like a DOS text file) or other funny charecters, these become part of the environemt or executed file.  Be sure the file contians just the lines, no additional spaces etc.


License:
--------

This Works is placed under the terms of the Copyright Less License,
see file COPYRIGHT.CLL.  USE AT OWN RISK, ABSOLUTELY NO WARRANTY.
