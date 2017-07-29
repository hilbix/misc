/* Wrap SUID root programs with LD_PRELOAD environment
 *
 * This Works is placed under the terms of the Copyright Less License,
 * see file COPYRIGHT.CLL.  USE AT OWN RISK, ABSOLUTELY NO WARRANTY.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static void
oops(const char *call)
{
  perror(call);
  exit(254);	/* resemble shell on fork() failure	*/
}

static int
myclose(int fd)
{
  while (close(fd))
    if (errno!=EINTR)
      return -1;
  return 0;
}

int
main(int argc, char **argv)
{
  char	buf[BUFSIZ], *ptr, *last;
  int	fd, len;

  /* locate the correct file in /wrap */
  ptr	= strrchr(argv[0], '/');
  if (!ptr)
    oops("missing / in command path");
  strcpy(buf, "/wrap");
  strncpy(buf+strlen(buf), ptr, (sizeof buf)-strlen(buf));
  if (buf[(sizeof buf)-1])
    oops("too long command name");
  strncpy(buf+strlen(buf), ".wrap", (sizeof buf)-strlen(buf));
  if (buf[(sizeof buf)-1])
    oops("too long command name");

  /* read the file	*/
  fd	= open(buf, O_RDONLY);
  if (fd<0)
    oops(buf);
  if ((len=read(fd, buf, sizeof buf))<0)
    oops("read");
  if (len>=sizeof buf)
    oops("too long environment");
  if (myclose(fd))
    oops("close");

  /* add the environment	*/
  last	= 0;
  for (ptr=buf; (ptr=strchr(ptr, '\n'))!=0; )
    {
      *ptr++ = 0;
      if (last && *last)
	{
#if 0
	  write(1, last, strlen(last)); write(1, "\r\n", 2);
#endif
          putenv(last);
        }
      last	= ptr;
    }

  /* Apply the effective uid/gid	*/
  setuid(geteuid());
  setgid(getegid());

  /* execute the wrapped binary	*/
  argv[0] = buf;
  execv(buf, argv);
  perror(buf);
  return 127;	/* resemble shell	*/
}

