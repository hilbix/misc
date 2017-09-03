Miscellaneous experiments

If not noted otherwise in the sources, it is [CLL](http://permalink.de/tino/cll), see [COPYRIGHT.CLL](COPYRIGHT.CLL).

# Overview

- `force_local/` is a recipe to use `LD_PRELOAD` to patch programs to listen or bind to fixed ports, such that you can apply static firewall rules to them.

- `git_push_samba/` is a workaround for http://superuser.com/q/681196 (`git` fails on certain POSIX violating filesystems).

- `helper/` are some shell helpers
  - Most need other tools from https://github.com/hilbix/src to be installed

- `porthog/` allocates IP port ranges and keeps them open while another command runs.  This prevents the running program from binding to that ports.

- `runsh/` allows to run scripts directly from the login prompt.  Convenient for laptops to switch different access profiles quickly.  Risky for machines with `ssh` access, use `PermitEmptyPasswords` wisely or try `rssh`.  `runsh` supports SUID, too.

- `src/ipof` is a easy to use, highly configurable, fast and extremly small IP resolver for batch and scripting purpose and works for IPv4 and IPv6.  It's source is less than 8K (and relies on the system resolver, of course).  Other tools in `src/`:
  - `checknul` demonstrates `AIO` by doing async reads to checks files for `NUL` spans (it does not skip over holes).
  - `fs_ioc_fiemap` shows fragments of files.  For example to find possible targets of `e4defrag`.
  - `gethostname` just calls `gethostname()`
  - `udp` uses `sendto()` to send UDP packets (just to demonstrate the minimum)
  - `umount2-l` just runs `umount2(dir, UMOUNT_NOFOLLOW|MNT_DETACH)` (still can detach mounts when `umount -l` hangs)

- `wrap/` is a wrapper to alter the environment of SUID programms and installs to `/wrap/` - it can be used to apply `force_local/` fixes to SUID programs.
  - [`suid`](https://github.com/hilbix/suid/) is a far better successor to this.

