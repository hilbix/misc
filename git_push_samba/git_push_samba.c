/* This Works is placed under the terms of the Copyright Less License,
 * see file COPYRIGHT.CLL.  USE AT OWN RISK, ABSOLUTELY NO WARRANTY.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#define	open	ignorethisopen
#define	open64	ignorethisopen64
#include <fcntl.h>
#undef open
#undef open64
#include <dlfcn.h>

static int (*___open)(const char *, int, mode_t);
static int (*___open64)(const char *, int, mode_t);

#if 0
#define	DP(X)	debugprintf X
#include <stdarg.h>
#include <unistd.h>
static void
debugprintf(const char *s, ...)
{
  va_list	list;
  char		buf[4096];

  va_start(list, s);
  vsnprintf(buf, sizeof buf, s, list);
  va_end(list);
  write(-1, buf, strlen(buf));	/* you can see this in strace	*/
}
#else
#define	DP(X)
#endif

static void *
dlwrap(const char *fn)
{
  const char *err;

  void *p = dlsym(RTLD_NEXT, fn);
  if ((err=dlerror())!=0)
    fprintf(stderr, "dlsym(RTLD_NEXT,'%s'): %s\r\n", fn, err);
  return p;
}

void
_init(void)
{
  DP(("open wrappe"));

  ___open	= dlwrap("open");
  ___open64	= dlwrap("open64");
}

int
open64(const char *file, int flags, mode_t mode)
{
  if (flags && O_CREAT)
    return ___open64(file, flags, mode|0200);
  return ___open64(file, flags, mode);
}

int
open(const char *file, int flags, mode_t mode)
{
  if (flags && O_CREAT)
    return ___open(file, flags, mode|0200);
  return ___open(file, flags, mode);
}

