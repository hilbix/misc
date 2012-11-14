#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
  uid_t ru, eu, su;
  gid_t rg, eg, sg;

  ru = eu = su = -1;
  rg = eg = sg = -1;
  if (getresuid(&ru, &eu, &su))
    perror("getresuid");
  if (getresgid(&rg, &eg, &sg))
    perror("getresgid");
  printf("uid real=%5ld effective=%5ld saved=%5ld\n", (long)ru, (long)eu, (long)su);
  printf("gid real=%5ld effective=%5ld saved=%5ld\n", (long)rg, (long)eg, (long)sg);
  return 0;
}
