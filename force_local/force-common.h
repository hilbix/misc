/* This Works is placed under the terms of the Copyright Less License,
 * see file COPYRIGHT.CLL.  USE AT OWN RISK, ABSOLUTELY NO WARRANTY.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dlfcn.h>

#define SAIN(x) ((struct sockaddr_in *)(x))

static void *
dlwrap(const char *fn)
{
  const char *err;

  void *p = dlsym(RTLD_NEXT, fn);
  if ((err=dlerror())!=0)
    fprintf(stderr, "dlsym(RTLD_NEXT,'%s'): %s\r\n", fn, err);
  return p;
}

static int
get_u16(const char *env, int def)
{
  const char *s;
  char *end;
  unsigned long nr;

  s = getenv(env);
  if (!s)
    return def;
  nr = strtoul(s, &end, 0);
  return !end || *end || nr>65535u ? def : nr;
}

static void
get_addr(struct sockaddr_in *sa, const char *env, const char *def)
{
  const char *addr;

  addr = getenv(env);
  if (def && (!addr || !*addr))
    sa->sin_addr.s_addr = inet_addr(def);
  else if (!addr || !*addr || *addr=='*')
    sa->sin_addr.s_addr = htonl(INADDR_ANY);
  else
    sa->sin_addr.s_addr = inet_addr(addr);
}

static void
get_port(struct sockaddr_in *sa, const char *env)
{
  unsigned port;

  port = get_u16(env, 0);
  if (port)
    sa->sin_port = htons(port);
}

