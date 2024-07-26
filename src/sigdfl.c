/* Default all signal handlers to terminalt the executed command
 *
 * usage: ./sigdfl command args..
 *
 * This Works is placed under the terms of the Copyright Less License,
 * see file COPYRIGHT.CLL.  USE AT OWN RISK, ABSOLUTELY NO WARRANTY.
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>

static void
writes(const char *s, ...)
{
  va_list	list;

  va_start(list, s);
  for (; s; s=va_arg(list, char *))
    write(2, s, strlen(s));
  write(2, "\n", 1);
}

int
main(int argc, char **argv)
{
  if (argc<2)
    writes("Usage: ", argv[0], " cmd args..", NULL);
  for (int i=SIGRTMAX; i; i--)
    signal(i, SIG_DFL);
  execvp(argv[1], argv+1);
  writes(argv[1], ": ", strerror(errno), NULL);
  return 127;
}

