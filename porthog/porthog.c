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

#include <poll.h>
#include <fcntl.h>

static struct sockaddr_in	h4;
static struct sockaddr_in6	h6;
static int			maxfd;
static int			ipv4, ipv6;
static struct pollfd		*polls;
static int			npolls;
static int			forks;

static void
oops(const char *call)
{
  perror(call);
  exit(254);	/* resemble shell on fork() failure	*/
}

static void
myclose(int fd)
{
  if (fd>=0)
    while (close(fd) && errno==EINTR);
}

static void
closeall(void)
{
  while (maxfd>2)
    myclose(maxfd--);
}

static void __inline__ IGNORE_RETURN_VALUE(){}	/* make GCC happy	*/

static void
poller(void)
{
  int n, i;
  struct pollfd *p;

  if ((n=poll(polls, npolls, -1))<0)
    {
      if (errno==EINTR)
        return;
      oops("poll");
    }
  /* Just discard everything	*/
  for (p=polls, i=npolls; n && --i>=0; p->revents=0, p++)
    if (p->revents==POLLIN)
      {
        char buf[BUFSIZ];
  
        myclose(accept(p->fd, NULL, NULL));
        IGNORE_RETURN_VALUE(read(p->fd, buf, sizeof buf));
        n--;
      }
}

/* This also circumvent ulimit and the like.
 * Sorry, this routine is magic and intransparent.
 */
static void
forkme(void)
{
  pid_t child;
  int	status;

  if ((child=fork())==0)
    {
      closeall();	/* magic	*/
      npolls = 0;	/* intransparent	*/
      return;
    }
  if (child==(pid_t)-1)
    oops("fork()");	/* no retry	*/

  do
    {
      pid_t p;

      if (!forks)
	poller();	/* intransparent	*/

      p = waitpid((pid_t)-1, &status, forks /*magic*/ ? 0 : WNOHANG);
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
	  forkme();	/* circumvent ulimit	*/
	  s = socket(domain, type, 0);
	  if (s<0)
	    oops("socket() even after fork");
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
      if (!forks)
	{
          struct pollfd *p;

	  fcntl(s, F_SETFL, fcntl(s, F_GETFL, 0) | O_NONBLOCK);

#define	INCPOLLS	256	/* power of 2	*/
	  if (!polls)
	    polls	= malloc(INCPOLLS * sizeof *polls);
          else if (!(npolls&(INCPOLLS-1)))
	    polls	= realloc(polls, (npolls+INCPOLLS) * sizeof *polls);
	  if (!polls)
	    oops("OOM");
	  p		= &polls[npolls++];
	  p->fd		= s;
	  p->events	= POLLIN;
	  p->revents	= 0;
	}
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
sethost(const char *host, int fam)
{
  struct addrinfo *ai, *a, q;
 
  q.ai_flags = 0;
  q.ai_family = fam;
  q.ai_socktype = 0;
  q.ai_protocol = 0;
  q.ai_addrlen = 0;
  q.ai_addr = 0;
  q.ai_canonname = 0;
  q.ai_next = 0;

  if (getaddrinfo(host, "1", &q, &ai))
    {
      perror("getaddrinfo failed");
      exit(1);
    }
  for (a=ai; a; a=a->ai_next)
    {
      void *t;

      switch (a->ai_family)
	{
	default:	continue;
	case AF_INET:	t	= &h4; ipv4=1; break;
	case AF_INET6:	t	= &h6; ipv6=1; break;
	}
#if 0
      printf("%d %s %ld\n", a->ai_family, h, (long)a->ai_addrlen);
#endif
      memcpy(t, a->ai_addr, a->ai_addrlen);
    }
  freeaddrinfo(ai);
}

static void get_children_dummy_handler() {}

static void
get_children(void)
{
  struct sigaction sa;

  sa.sa_handler = &get_children_dummy_handler;
  sa.sa_flags = 0;
  sigfillset(&sa.sa_mask);
  if (sigaction(SIGCHLD, &sa, NULL))
    oops("sigaction");
}


int
main(int argc, char **argv)
{
  char	*range;
  int	tcp, udp;

  if (argc<3)
    {
      fprintf(stderr, "Usage: %s range[,range...] program [args...]\n"
		"\trange is [proto:]port[-port][@ip], proto defaults to 'tu46'\n"
		"\t\twhich stands for: tcp, udp, ipv4 and ipv6.\n"
		"\tIf program is '-' then it sits on the socktets until killed.\n"
		, argv[0]);
      return 2;		/* resemble shell	*/
    }

  get_children();

  forks	= argc>3 || strcmp(argv[2], "-");
  maxfd	= 0;
  tcp	= 1;
  udp	= 1;

  sethost(NULL, AF_UNSPEC);

  if (!ipv4)
    perror("ipv4 not available");
  if (!ipv6)
    perror("ipv6 not available");

  while ((range=strtok(argv[1], ","))!=0)
    {
      char *p;

      argv[1]=0;
      if ((p=strchr(range,'@'))!=0)
	{
	  *p = 0;
	  sethost(p+1, AF_UNSPEC);
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

  if (!forks)
    poller();

  forkme();

  execvp(argv[2], argv+2);

  perror(argv[2]);
  return 127;	/* resemble shell	*/
}

