# GIT push Samba

This is a workaround for http://superuser.com/q/681196.  No need to patch the kernel nor Samba nor GIT.

Compile this and then invoke `LD_PRELOAD=path/to/git_push_samba.so git` instead of `git`.

Please note:

- `git` is correct.  POSIX allows to create a file readonly and then write to it.
- The kernel/smbmount drivers etc. are implementing this feature wrong, so they block write attemts, therefor violating POSIX.
- However this bug seems witespread.

The way to go is to just always add write permissions to the user in that case.
Exactly this hack is implemented here.  In a perfect world this would not be needed.


## Compile:

    make
    make install

This installs a `$HOME/bin/git` to wrap with this wrapper installed.


## Background:

When `git` tries to create an object, folloing happens (output of `strace`):

    open("./objects/98/tmp_obj_vVR5gb", O_RDWR|O_CREAT|O_EXCL|O_LARGEFILE, 0444) = -1 EACCES (Permission denied)

which translates to (output of `ltrace`):

    open64("./objects/98/tmp_obj_96Lm3b", 194, 0444) = -1

The interesting part here is that `./objects/98/` is writeable and everything else is without error.
It's just the filesystem's driver which violates POSIX and does not allow to use `O_CREAT` with `0444`.  Changing this to `0644` fixes this problem.

## License:

This Works is placed under the terms of the Copyright Less License,
see file COPYRIGHT.CLL.  USE AT OWN RISK, ABSOLUTELY NO WARRANTY.
