#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

static void
oops(const char *error)
{
  perror(error);
  exit(127);
}

#if 0
#define OK(X) ok(#X, X)

static void
ok(const char *cmd, int ret)
{
  if (ret)
    oops(cmd);
}
#endif

#define NOTNULL(X) notnull(#X, X)

static void *
notnull(const char *cmd, void *p)
{
  if (!p)
    oops(cmd);

  return p;
}

static void
strxcat(char *s, size_t max, ...)
{
  va_list list;
  const char *c;

  va_start(list, max);
  while ((c=va_arg(list, char *))!=0)
    {
      size_t l;

      l = strlen(c);
      if (l>=max)
        oops("string too long");
      strncpy(s, c, l);
      s += l;
      s[0] = 0;
      max -= l;
    }
}

static const char *
strxdup(const char *s)
{
  if (!s)
    oops("NULL pointer");
  s = strdup(s);
  if (!s)
    oops("OOM");
  return s;
}

#define PW(X) ((struct passwd *)(X))

int
main(int argc, char **argv)
{
  char buf[256];
  const char *LOGNAME;
  uid_t u;

  LOGNAME = NOTNULL(getenv("LOGNAME"));
  if (geteuid()!=getuid() && ((u=PW(NOTNULL(getpwnam(LOGNAME)))->pw_uid)!=getuid() && u!=geteuid()))
    {
      /* Somebody lied to us in a suexec situation */
      LOGNAME = strxdup(PW(NOTNULL(getpwuid(getuid())))->pw_name);
      /* getpwuid returns some reused buffer, so we need strdup() here */
    }
  strxcat(buf, sizeof buf, PW(NOTNULL(getpwuid(geteuid())))->pw_dir, "/.runsh/", LOGNAME, ".runsh", NULL);
  execl("/bin/sh", LOGNAME, "-lc", buf, NULL);
  oops(buf);
  return 127;
}

