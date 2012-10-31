/* Allocate port ranges before executing a program.
 *
 * This Works is placed under the terms of the Copyright Less License,
 * see file COPYRIGHT.CLL.  USE AT OWN RISK, ABSOLUTELY NO WARRANTY.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <netdb.h>
#include <netinet/in.h>

static struct sockaddr_in	h4;
static struct sockaddr_in6	h6;
static int			maxfd;

static void
oops(const char *call)
{
  perror(call);
  exit(254);	/* resemble shell on fork() failure	*/
}

static void
myclose(int fd)
{
  while (close(fd) && errno==EINTR);
}

static void
closeall(void)
{
  while (maxfd>2)
    myclose(maxfd--);
}

static void
forkme(void)
{
  pid_t child;
  int	status;

  if ((child=fork())==0)
    {
      closeall();
      return;
    }
  if (child==(pid_t)-1)
    oops("fork()");	/* no retry	*/

  do
    {
      pid_t p;

      p = waitpid((pid_t)-1, &status, 0);
      if (p==(pid_t)-1)
	oops("waitpid()");

      if (p!=child)
	continue;

      if (WIFSIGNALED(status))
	exit(143);	/* resemble shell	*/

    } while (!WIFEXITED(status));

  exit(WEXITSTATUS(status));
}

static void
hog2(int domain, int type, unsigned long from, unsigned long to, struct sockaddr *sa, socklen_t len)
{
  int	i;

  if (!sa)
    return;

  sa->sa_family = domain;
  for (i=from; i<=to; i++)
    {
      int	s;

      s	= socket(domain, type, 0);
      if (s<0)
	{
	  if (errno!=EMFILE)
	    oops("socket()");
	  forkme();
	}
      ((struct sockaddr_in *)sa)->sin_port = htons(i);
      if (bind(s, sa, len))
	{
	  myclose(s);
	  continue;
	}
      listen(s,1);
      if (s>maxfd)
	maxfd	= s;
    }
}

static void
hog(int type, char *range, struct sockaddr_in *h4, struct sockaddr_in6 *h6)
{
  unsigned long from, to;
  char		*end;

  from	= strtoul(range, &end, 10);
  to	= from;
  if (end && *end=='-')
    to	= strtoul(end+1, &end, 10);
  if (end && *end)
    {
      perror(range);
      exit(1);
    }
  if (from<1 || from>to || to>65535)
    {
      errno	= ERANGE;
      perror(range);
      exit(1);
    }
  hog2(AF_INET,  type, from, to, (struct sockaddr *)h4, sizeof *h4);
  hog2(AF_INET6, type, from, to, (struct sockaddr *)h6, sizeof *h6);
}

static void
sethost(const char *h)
{
  struct addrinfo *ai, *a;

  if (getaddrinfo(h, NULL, NULL, &ai))
    {
      perror(h);
      exit(1);
    }
  for (a=ai; a; a=a->ai_next)
    {
      void *t;

      switch (a->ai_family)
	{
	default:	continue;
	case AF_INET:	t	= &h4; break;
	case AF_INET6:	t	= &h6; break;
	}
#if 0
      printf("%d %s %ld\n", a->ai_family, h, (long)a->ai_addrlen);
#endif
      memcpy(t, a->ai_addr, a->ai_addrlen);
    }
  freeaddrinfo(ai);
}

int
main(int argc, char **argv)
{
  char	*range;
  int	tcp, udp, ipv4, ipv6;

  if (argc<3)
    {
      fprintf(stderr, "Usage: %s range[,range...] program [args...]\n"
		"\trange is [proto:]port[-port][@ip], proto defaults to 'tu46'\n"
		"\t\twhich stands for: tcp, udp, ipv4 and ipv6\n"
		, argv[0]);
      return 2;		/* resemble shell	*/
    }

  maxfd	= 0;
  tcp	= 1;
  udp	= 1;
  ipv4	= 1;
  ipv6	= 1;

  sethost("127.0.0.1");
  sethost("::1");

  while ((range=strtok(argv[1], ","))!=0)
    {
      char *p;

      argv[1]=0;
      if ((p=strchr(range,'@'))!=0)
	{
	  *p = 0;
	  sethost(p+1);
	}
      if ((p=strchr(range,':'))!=0)
	{
	  *p++ = 0;
	  tcp = udp = ipv4 = ipv6 = 0;
	  for (;;)
	    {
	      switch (*range++)
		{
		default:	continue;
		case 't': tcp	= 1; continue;
		case 'u': udp	= 1; continue;
		case '4': ipv4	= 1; continue;
		case '6': ipv6	= 1; continue;
		case 0:
		  break;
		}
	      break;
	    }
	  if (!tcp && !udp)
	    tcp = udp = 1;
	  if (!ipv4 && !ipv6)
	    ipv4 = ipv6 = 1;
	  range = p;
	}
      if (udp)
	hog(SOCK_DGRAM,  range, ipv4 ? &h4 : NULL, ipv6 ? &h6 : NULL);
      if (tcp)
	hog(SOCK_STREAM, range, ipv4 ? &h4 : NULL, ipv6 ? &h6 : NULL);
    }

  forkme();

  execvp(argv[2], argv+2);

  perror(argv[2]);
  return 127;	/* resemble shell	*/
}
