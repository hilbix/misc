#include <stdio.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
  char	buf[BUFSIZ];

  return printf("%d: %.*s\n", gethostname(buf, sizeof buf), (int)(sizeof buf), buf);
  return 0;
}
