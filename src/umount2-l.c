/* Unconditional unblocking call to umount2() in lazy mode
 *
 * This is for the case where
 *   /bin/umount -l PATH
 * hangs indefinitely because the path has become dead.
 * This is due "/bin/umount" calls "lstat()" before the umount
 * and "lstat() may block forever.
 *
 * In that case call `umount2l path..`
 */

#include <stdio.h>
#include <sys/mount.h>

int
main(int argc, char **argv)
{
  int i, flag=0;

  if (argc<1)
    {
      printf("Usage: %s path..\n\tnonblocking 'umount -l path..'\n", argv[0]);
      return 23;
    }
  for (i=0; ++i<argc; )
    flag |= umount2(argv[i], UMOUNT_NOFOLLOW|MNT_DETACH);
  return flag;
}

