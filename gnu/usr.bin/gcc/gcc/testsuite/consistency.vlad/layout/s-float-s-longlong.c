#include <stdio.h>

static struct sss{
  float f;
  struct {long long m;} snd;
} sss;

#define _offsetof(st,f) ((char *)&((st *) 16)->f - (char *) 16)

int main (void) {
  printf ("+++Struct longlong inside struct starting with float:\n");
  printf ("size=%d,align=%d\n", sizeof (sss), __alignof__ (sss));
  printf ("offset-float=%d,offset-sss-longlong=%d,\nalign-float=%d,align-sss-longlong=%d\n",
          _offsetof (struct sss, f), _offsetof (struct sss, snd),
          __alignof__ (sss.f), __alignof__ (sss.snd));
  return 0;
}
