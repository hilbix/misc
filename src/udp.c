#define _GNU_SOURCE

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

void
oops(const char *s)
{
  perror(s);
  exit(1);
}

int
main(int argc, char **argv)
{
  int			fd;
  struct addrinfo	*ret, hint;
  const char		*host, *port, *text;
  size_t		len;

  if ((fd = socket(AF_INET, SOCK_DGRAM, 0))<0)
    oops("no socket");

  host	= argc>1 ? argv[1] : "localhost";
  port	= argc>2 ? argv[2] : "1111";
  text	= argc>3 ? argv[3] : "hello world";
  len	= strlen(text);

  memset(&hint, 0, sizeof hint);
  hint.ai_family	= AF_UNSPEC;
  hint.ai_flags		= AI_IDN;
  if (getaddrinfo(host, port, &hint, &ret))
    oops("cannot resolve");

  for (;;)
    {
      if (sendto(fd, text, len, 0, ret->ai_addr, ret->ai_addrlen)!=len)
	oops("send error");
      write(1, ".", 1);
      usleep(100000);
    }
}
