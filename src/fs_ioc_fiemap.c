/* call ioctl(FS_IOC_FIEMAP) on files/paths
 * and dump information using a callback
 *
 * If you are puzzled, it basically lists the fragments of each file.
 * For example to prepare a call to "e4defrag".
 */

#include <stdio.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/ioctl.h>

#include <linux/fs.h>
#include <linux/fiemap.h>

static void
OOPS(const char *s, ...)
{
  int		e=errno;
  va_list	list;

  fprintf(stderr, "OOPS: ");
  va_start(list, s);
  vfprintf(stderr, s, list);
  va_end(list);
  errno = e;
  perror("");
  exit(23);
}

#define	FIEMAP_SIZE	100
struct fiemap_size
    {
      struct fiemap		m;
      struct fiemap_extent	ext[FIEMAP_SIZE];
    };

static int
fs_ioc_fiemap(const char *file, int xattr, int cb(const char *name, struct stat *, int nr, struct fiemap_extent *))
{
  int			fd, i, n, err;
  struct fiemap_size	map;
  struct stat		st;
  __u64			start;

  if ((fd=open(file, O_RDONLY|O_NOFOLLOW))<0)
    return -1;

  memset(&map, 0, sizeof map);
  start	= 0;
  err	= 0;

  if (fstat(fd, &st))
    err = -2;

  n = 0;
  while (!err)
    {
      struct fiemap_extent	*e;

      map.m.fm_start		= start;
      map.m.fm_length		= -1;
      map.m.fm_flags		= 0;
      map.m.fm_extent_count	= FIEMAP_SIZE;

      if (ioctl(fd, FS_IOC_FIEMAP, &map))
	{
	  err = -3;
	  break;
	}
      if (!map.m.fm_mapped_extents)
	break;
      for (e=map.m.fm_extents, i=0; i<map.m.fm_mapped_extents; i++, e++)
	{
	  __u64 tmp;

	  if (cb(file, &st, ++n, e))
	    goto out;
	  tmp = e->fe_logical + e->fe_length;
	  if (start<tmp)
	    start = tmp;
	}
    }

 out:
  while (close(fd))
    if (errno!=EINTR)
      return err ? err : -4;
  return err;
}

static int
print_extent_cb(const char *name, struct stat *st, int nr, struct fiemap_extent *e)
{
  static struct { const char name[7]; int flag; } flags[] =
    {
      { ",_last", FIEMAP_EXTENT_LAST		},
      { ",unkwn", FIEMAP_EXTENT_UNKNOWN		},
      { ",delet", FIEMAP_EXTENT_DELALLOC	},
      { ",obfsc", FIEMAP_EXTENT_ENCODED		},
      { ",crypd", FIEMAP_EXTENT_DATA_ENCRYPTED	},
      { ",nalgn", FIEMAP_EXTENT_NOT_ALIGNED	},
      { ",inlin", FIEMAP_EXTENT_DATA_INLINE	},
      { ",_tail", FIEMAP_EXTENT_DATA_TAIL	},
      { ",pndng", FIEMAP_EXTENT_UNWRITTEN	},
      { ",mergd", FIEMAP_EXTENT_MERGED		},
      { ",shred", FIEMAP_EXTENT_SHARED		},
    };
  char flag[sizeof(flags->name)*sizeof(flags)/sizeof(*flags)];
  int	i;

  flag[0] = 0;
  for (i=0; i<sizeof(flags)/sizeof(*flags); i++)
    if (e->fe_flags & flags[i].flag)
      strcat(flag, flags[i].name);
  printf("%04llx.%010llx %010llx %010llx-%010llx/%010llx %10llx %-11s %2d %s\n", (long long unsigned)st->st_dev, (long long unsigned)st->st_ino, e->fe_length, e->fe_logical, e->fe_logical+e->fe_length, (long long unsigned)st->st_size, e->fe_physical, *flag ? flag+1 : "-", nr, name);
  return 0;
}

int
main(int argc, char **argv)
{
  int i, err;

  if (argc<1)
    OOPS("Usage: %s file..", argv[0]);
  printf("%4s %10s %10s %10s %10s %10s %10s %-11s %2s %s\n", "dev", "inode", "0xlength", "0xfrom", "0xto", "0xsize", "0xdisk", "flags", "nr", "file");
  err = 0;
  for (i=0; ++i<argc; )
    if (fs_ioc_fiemap(argv[i], 0, print_extent_cb))
      {
	perror(argv[i]);
	err++;
      }
  return err>20 ? 20 : err;
}

