/*-*- compile-command: "/mingw64/bin/gcc -c -o seqtest.gp.o -O3 -Wall -fno-strict-aliasing -I\"/usr/local/include\" seqtest.gp.c && /mingw64/bin/gcc -o seqtest.gp.so -shared -O3 -Wall -fno-strict-aliasing -Wl seqtest.gp.o -Wl,-rpath,/usr/local/bin -L/usr/local/bin -lpari -Wl,--enable-auto-import"; -*-*/
#include <pari/pari.h>
/*
GP;install("init_seqtest","v","init_seqtest","./seqtest.gp.so");
GP;install("addone","lL","addone","./seqtest.gp.so");
*/
void init_seqtest(void);
long addone(long x);
/*End of prototype*/

void
init_seqtest(void)	  /* void */
{
  return;
}

long
addone(long x)
{
  return x + 1;
}

