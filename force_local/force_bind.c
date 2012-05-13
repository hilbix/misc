/* This Works is placed under the terms of the Copyright Less License,
 * see file COPYRIGHT.CLL.  USE AT OWN RISK, ABSOLUTELY NO WARRANTY.
 */

#include "force-common.h"

static int (*_socket)(int domain, int type, int protocol);
static struct sockaddr_in _saddr;
static struct { int domain; int type; int proto; } _sock;

void
_init(void)
{
  DP(("hello world"));

  _socket	= dlwrap("socket");

  _sock.domain	= get_u16("FORCE_BIND_SOCKDOM", AF_INET);
  _sock.type	= get_u16("FORCE_BIND_SOCKTYPE", SOCK_DGRAM);
  _sock.proto	= get_u16("FORCE_BIND_SOCKPROTO", IPPROTO_UDP);

  get_addr(&_saddr, "FORCE_BIND_ADDR", "127.0.0.1\0\0\0\0\0\0");
  get_port(&_saddr, "FORCE_BIND_PORT");
  _saddr.sin_family = _sock.domain;
}

int
socket(int domain, int type, int protocol)
{
  int fd;

  fd	= _socket(domain, type, protocol);
  DP(("socket(%d,%d,%d) = %d cmp %d %d %d\n", domain, type, protocol, fd, _sock.domain, _sock.type, _sock.proto));
  if (fd<0)
    return fd;
  if (_sock.domain == domain && _sock.type == type && _sock.proto == protocol)
    bind(fd, &_saddr, sizeof _saddr);
  return fd;
}

