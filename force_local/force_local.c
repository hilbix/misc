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

static int (*_bind)(int, const struct sockaddr *, socklen_t);
static struct sockaddr_in _saddr, _any;

static void *
dlwrap(const char *fn)
{
  const char *err;

  void *p = dlsym(RTLD_NEXT, fn);
  if ((err=dlerror())!=0)
    fprintf(stderr, "dlsym(RTLD_NEXT,'%s'): %s\r\n", fn, err);
  return p;
}

static unsigned
getport(const char *env)
{
  const char *s;
  char *end;
  unsigned long port;

  s = getenv(env);
  if (!s)
    return 0;
  port = strtoul(s, &end, 0);
  return !end || *end || port>65535u ? 0 : port;
}

void
_init(void)
{
  const char *addr;
  unsigned port;

  _bind	= dlwrap("bind");

  addr = getenv("FORCE_LOCAL_ADDR");
  if (!addr)
    addr = "127.0.0.1";
  _saddr.sin_addr.s_addr = inet_addr(addr);
  
  port = getport("FORCE_LOCAL_PORT");
  if (port)
    _saddr.sin_port = htons(port);

  addr = getenv("FORCE_LOCAL_FROM_ADDR");
  if (!addr || !*addr || *addr=='*')
    _any.sin_addr.s_addr = htonl(INADDR_ANY);
  else
    _any.sin_addr.s_addr = inet_addr(addr);

  port = getport("FORCE_LOCAL_FROM_PORT");
  if (port)
    _any.sin_port = htons(port);
}

#define SAIN(x) ((struct sockaddr_in *)(x))

int
bind(int fd, const struct sockaddr *ptr, socklen_t len)
{
  if (SAIN(ptr)->sin_family==AF_INET
      && SAIN(ptr)->sin_addr.s_addr==_any.sin_addr.s_addr
      && ( !_any.sin_port || SAIN(ptr)->sin_port == _any.sin_port )
     )
    {
      SAIN(ptr)->sin_addr.s_addr = _saddr.sin_addr.s_addr;
      if (_saddr.sin_port)
        SAIN(ptr)->sin_port = _saddr.sin_port;
    }
  return _bind(fd, ptr, len);
}

