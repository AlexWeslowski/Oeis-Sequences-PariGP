/*-*- compile-command: "/mingw64/bin/gcc -c -o seqtest.gp.o -O3 -Wall -fno-strict-aliasing -I\"/usr/local/include\" seqtest.gp.c && /mingw64/bin/gcc -o seqtest.gp.so -shared -O3 -Wall -fno-strict-aliasing -Wl seqtest.gp.o -Wl,-rpath,/usr/local/bin -L/usr/local/bin -lpari -Wl,--enable-auto-import"; -*-*/
#include <pari/pari.h>
/*
GP;install("init_seqtest","v","init_seqtest","./seqtest.gp.so");
GP;install("addone","G","addone","./seqtest.gp.so");
GP;install("cover_or_bitvec","GG","cover_or_bitvec","./seqtest.gp.so");
GP;install("cover_or_ratio","GGp","cover_or_ratio","./seqtest.gp.so");
GP;install("is_divisible","lGG","is_divisible","./seqtest.gp.so");
GP;install("backtrack","vGGGGG","backtrack","./seqtest.gp.so");
GP;install("find_divisors","G","find_divisors","./seqtest.gp.so");
*/
void init_seqtest(void);
GEN addone(GEN n);
GEN cover_or_bitvec(GEN n, GEN d);
GEN cover_or_ratio(GEN n, GEN v, long prec);
long is_divisible(GEN d, GEN V);
void backtrack(GEN N, GEN D, GEN start, GEN current_factors, GEN current_prod);
GEN find_divisors(GEN N);
/*End of prototype*/

static GEN results;
/*End of global vars*/

void
init_seqtest(void)	  /* void */
{
  results = pol_x(fetch_user_var("results"));
  results = mklist();
  return;
}

/* gp2c -S seqtest.gp > seqtest.c */
/* gcc -shared -O2 -I/usr/local/include -L/usr/local/lib -o seqtest.dll seqtest.c -lpari */
/* gcc -shared -O2 -I/usr/local/include -o seqtest.dll seqtest.c /usr/local/bin/libpari.dll.a */
/* install("cover_or_bitvec", "LG", "cover_or_bitvec", "./seqtest.dll"); */
/* cover_or_bitvec(12, [3, 4]); */

GEN
addone(GEN n)	  /* int */
{
  if (typ(n) != t_INT)
    pari_err_TYPE("addone",n);
  return addis(n, 1);
}

GEN
cover_or_bitvec(GEN n, GEN d)
{
  GEN bits;
  GEN p1;	  /* int */
  GEN p2;	  /* vec */
  long l3;	  /* lg */
  long l4;
  GEN p5;
  if (typ(n) != t_INT)
    pari_err_TYPE("cover_or_bitvec",n);
  if (!is_matvec_t(typ(d)))
    pari_err_TYPE("cover_or_bitvec",d);
  /*my(bits = Vecsmall(vector(n+1, i, 0))); */
  p1 = addis(n, 1);
  {
    long i;
    p2 = cgetg(itos(p1)+1, t_VEC);
    for (i = 1; cmpsi(i, p1) <= 0; ++i)
      gel(p2, i) = gen_0;
  }
  bits = p2;
  l3 = lg(d);
  {
    long i;
    for (i = 1; i < l3; ++i)
    {
      GEN p6;
      GEN p7;	  /* int */
      GEN p8;
      long l9;	  /* bool */
      p6 = gcopy(gel(d, i));
      p7 = addis(n, 1);
      p8 = gcopy(gel(d, i));
      l9 = gcmpgs(p8, 0) > 0;
      {
        GEN bit;
        for (bit = p6; l9?gcmp(bit, p7) <= 0:gcmp(bit, p7) >= 0; bit = gadd(bit, p8))
          gel(bits, gtos(bit)) = gen_1;
      }
    }
  }
  l4 = glength(bits);
  {
    long i;
    p5 = gen_0;
    for (i = 1; i <= l4; ++i)
      p5 = gadd(p5, gel(bits, i));
  }
  return gdiv(p5, n);
}

GEN
cover_or_ratio(GEN n, GEN v, long prec)
{
  GEN ilen, c = gen_0, p1;
  if (typ(n) != t_INT)
    pari_err_TYPE("cover_or_ratio",n);
  if (!is_matvec_t(typ(v)))
    pari_err_TYPE("cover_or_ratio",v);
  ilen = stoi(lg(v)-1);
  p1 = gsubgs(gpow(gen_2, ilen, prec), 1);
  {
    GEN mask;
    for (mask = gen_1; gcmp(mask, p1) <= 0; mask = gaddgs(mask, 1))
    {
      GEN L = gen_1, bits = gen_0;
      {
        GEN i;
        for (i = gen_1; gcmp(i, ilen) <= 0; i = gaddgs(i, 1))
        {
          if (!gequal0(gbittest(mask, gtos(gsubgs(i, 1)))))
          {
            L = glcm(L, gel(v, gtos(i)));
            bits = gaddgs(bits, 1);
          }
        }
      }
      c = gadd(c, gdiv(gpow(gen_m1, gaddgs(bits, 1), prec), L));
    }
  }
  return c;
}

/*
install("is_divisible", "lGG", "is_divisible", "./seqtest.dll");
is_divisible(3, [2, 9]);
*/
long
is_divisible(GEN d, GEN V)
{
  long l1;
  l1 = glength(V);
  {
    long j;
    for (j = 1; j <= l1; ++j)
    {
      if (gequal0(gmod(gmax(d, gel(V, j)), gmin(d, gel(V, j)))))
        return 1;
    }
  }
  return 0;
}

/*
install("is_divisible", "lGG", "is_divisible", "./seqtest.dll");
install("backtrack", "vGGGGG", "backtrack", "./seqtest.dll");
install("init_seqtest", "v", "init_seqtest", "./seqtest.dll");
init_seqtest();
current_factors = List();
backtrack(12, divisors(12), 1, current_factors, 1);
*/
void
backtrack(GEN N, GEN D, GEN start, GEN current_factors, GEN current_prod)	  /* void */
{
  GEN factors;
  long l1;
  pari_printf("#results = %ld\n", glength(results));
  pari_printf("start = %Ps\n", start);
  pari_printf("current_factors = %Ps\n", current_factors);
  pari_printf("current_prod = %Ps\n", current_prod);
  /*
  if(current_prod == N,
  listput(results, Vec(current_factors));
  return()
  );
  */
  factors = listinit(gtolist(current_factors));
  pari_printf("factors = %Ps\n", factors);
  l1 = glength(D);
  {
    GEN i;
    for (i = gcopy(start); gcmpgs(i, l1) <= 0; i = gaddgs(i, 1))
    {
      GEN d;
      d = gcopy(gel(D, gtos(i)));
      if (!gequalgs(gmod(N, gmul(current_prod, d)), 0))
        continue;
      if (is_divisible(d, factors))
        continue;
      listput0(factors, d, 0);
      backtrack(N, D, gaddgs(i, 1), factors, gmul(current_prod, d));
      listpop0(factors, 0);
    }
  }
  return;
}

static long
anon_0(GEN d, GEN N)	  /* bool */
{
  return (gcmpgs(d, 1) > 0) && (gcmp(d, N) < 0);
}

static long
wrap_anon_0(void * _cargs, GEN d)
{
  GEN _args = (GEN) _cargs;
  long _res = anon_0(d, gel(_args,1));
  return _res;
}

GEN
find_divisors(GEN N)	  /* vec */
{
  GEN D, current_factors;
  if (typ(N) != t_INT)
    pari_err_TYPE("find_divisors",N);
  D = genselect(mkvec(N), wrap_anon_0, divisors(N));
  results = mklist();
  current_factors = mklist();
  backtrack(N, D, gen_1, current_factors, gen_1);
  return gtovec(results);
}

