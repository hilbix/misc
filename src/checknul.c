/* Output offset of NUL sector spans on disk/partition/file
 *
 * This uses an AIO recipe to speed up reading,
 * so "processing" can take place while data is read into the buffers.
 *
 * usage: ./checknul device_or_file
 *
 * This Works is placed under the terms of the Copyright Less License,
 * see file COPYRIGHT.CLL.  USE AT OWN RISK, ABSOLUTELY NO WARRANTY.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <malloc.h>
#include <aio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define	SECTOR	512
#define	SECTORS	40960
#define	BUFFERLEN	(SECTOR*SECTORS)

static void
oops(const char *s)
{
  perror(s);
  exit(1);
}

static void *
my_memalign(size_t len)
{
  void		*ptr;
  static size_t	pagesize;

  if (!pagesize)
    pagesize = sysconf(_SC_PAGESIZE);
  if (len%pagesize)
    oops("alignment?");
  ptr = memalign(pagesize, len);
  if (!ptr)
    oops("OOM");
  return ptr;
}

static struct aiocb aio;

static void
my_aio_read(void *buf)
{
  int	ret;

  aio.aio_buf = buf;
  ret = aio_read(&aio);
  if (ret<0)
    oops("aio_read");
}

static int
my_aio_wait(void)
{
  const struct aiocb	*cb;
  int			ret;

  cb = &aio;
  ret = aio_suspend(&cb, 1, NULL);
  if (ret<0)
    oops("aio_suspend");
  if (aio_error(&aio))
    return -1;
  return aio_return(&aio);
}

static unsigned long long	nul_last;
static int			nul_was;

static void
fin(void)
{
  if (!nul_was)
    return;
  printf("%010llx\n", nul_last);
  fflush(stdout);
  nul_was	= 0;
}

static void
checknul(unsigned long long pos, unsigned char *buf, int len)
{
  static unsigned char	nullblock[SECTOR];
  int			off;

  for (off=0; off<len; off+=SECTOR)
    if (memcmp(nullblock, buf+off, SECTOR))
      fin();
    else
      {
	if (!nul_was)
	  {
	    printf("%010llx-", pos+off);
	    fflush(stdout);
	    nul_was	= 1;
	  }
        nul_last	= pos+off+SECTOR-1;
      }
}

int
main(int argc, char **argv)
{
  unsigned char	*buf[2];
  int		fd;
  int		io, got;

  buf[0] = my_memalign(BUFFERLEN);
  buf[1] = my_memalign(BUFFERLEN);

  if (argc!=2)
    oops("Usage: checknul file");
  if ((fd=open(argv[1], O_RDONLY))<0)
    oops(argv[1]);
  
  aio.aio_nbytes	= BUFFERLEN;
  aio.aio_fildes	= fd;
  aio.aio_offset	= 0;

  io = 0;
  my_aio_read(buf[io]);
  while ((got=my_aio_wait())>0)
    {
      unsigned long long	pos;

      pos	= aio.aio_offset;

      aio.aio_offset += got;
      my_aio_read(buf[1-io]);

      checknul(pos, buf[io], got);

      io	= 1-io;
    }
  if (got<0)
    oops("read error");
  printf("eof\n");
  close(fd);
  return 0;
}

