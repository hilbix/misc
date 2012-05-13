/* This Works is placed under the terms of the Copyright Less License,
 * see file COPYRIGHT.CLL.  USE AT OWN RISK, ABSOLUTELY NO WARRANTY.
 */

#include "force-common.h"

static int (*_bind)(int, const struct sockaddr *, socklen_t);
static struct sockaddr_in _saddr, _any;

void
_init(void)
{
  _bind	= dlwrap("bind");

  get_addr(&_saddr, "FORCE_LOCAL_ADDR", "127.0.0.1");
  get_port(&_saddr, "FORCE_LOCAL_PORT");

  get_addr(&_any, "FORCE_LOCAL_FROM_ADDR", NULL);
  get_port(&_any, "FORCE_LOCAL_FROM_PORT");
}

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

