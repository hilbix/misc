/* B64enc < BINARY > base64
 *
 * This Works is placed under the terms of the Copyright Less License,
 * see file COPYRIGHT.CLL.  USE AT OWN RISK, ABSOLUTELY NO WARRANTY.
 *
 * XXX TODO XXX this critically lacks all error processing
 */

#include <stdio.h>
#include <string.h>

#include <openssl/evp.h>

static void
bio_copy(BIO *i, BIO *o)
{
  char	buf[BUFSIZ];
  int	got;

  while ((got = BIO_read(i, buf, BUFSIZ)) > 0)
    BIO_write(o, buf, got);
  BIO_flush(o);
}

static int
choice(const char *s, ...)
{
  int		i;
  const char	*arg;
  va_list	list;

  va_start(list, s);
  for (i=0; (arg = va_arg(list, const char *))!=0; i++)
    if (!strcmp(s, arg))
      return i;
  return -1;
}

int
main(int argc, char **argv)
{
  BIO	*i, *o, *b64;
  int	decode;

  if (argc!=2 || (decode=choice(argv[1], "enc", "dec", NULL))<0)
    {
      fprintf(stderr, "Usage: %s enc|dec\n", argv[0]);
      return 42;
    }

  i	= BIO_new_fd(0, BIO_NOCLOSE);
  o	= BIO_new_fd(1, BIO_NOCLOSE);
  b64	= BIO_new(BIO_f_base64());

  if (decode)
    {
      BIO_push(b64, i);
      bio_copy(b64, o);
    }
  else
    {
      BIO_push(b64, o);
      bio_copy(i, b64);
    }

  BIO_free(b64);
  BIO_free(o);
  BIO_free(i);

  return 0;
}

