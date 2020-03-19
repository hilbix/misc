# OpenSSL examples

Documentation on OpenSSL sucks.  You need to find it out, yourself, the hard way.

Here is what I came up with.  But beware, I am no cryptonerd.

So if something is insecure, please open an issue on GitHub.  Thanks!

## About OpenSSL

OpenSSL has 3 types of routines, you can use:

- Simple
- Normal
- Complex

Simple is so simple, that you will fail to adapt it correctly to your needs.

Normal uses the nonaccelerated implementation, which, well I do not know why this exists at all.

So always choose the most complex way with OpenSSL.

However the complex routines are barely documented, so it is very likely that you fail due to this.
But there is the slight chance not to fail, if you are Bruce Schneier.

