/*-*- compile-command: "/mingw64/bin/gcc -c -o sequence.gp.o -O3 -Wall -fno-strict-aliasing -I\"/usr/local/include\" sequence.gp.c && /mingw64/bin/gcc -o sequence.gp.so -shared -O3 -Wall -fno-strict-aliasing -Wl sequence.gp.o -Wl,-rpath,/usr/local/bin -L/usr/local/bin -lpari -Wl,--enable-auto-import"; -*-*/
#include <pari/pari.h>
/*
GP;install("init_sequence","v","init_sequence","./sequence.gp.so");
GP;install("cover_or_bitvec","GG","cover_or_bitvec","./sequence.gp.so");
GP;install("cover_or_ratio","GGp","cover_or_ratio","./sequence.gp.so");
GP;install("cover_xor_bitvec","GG","cover_xor_bitvec","./sequence.gp.so");
GP;install("cover_xor_ratio","GGp","cover_xor_ratio","./sequence.gp.so");
GP;install("is_divisible","lGG","is_divisible","./sequence.gp.so");
GP;install("is_divisible_set","G","is_divisible_set","./sequence.gp.so");
GP;install("backtrack","vGGGGGG","backtrack","./sequence.gp.so");
GP;install("search","vGGGGGGGG","search","./sequence.gp.so");
GP;install("product_sets","GGG","product_sets","./sequence.gp.so");
GP;install("find_divisors","G","find_divisors","./sequence.gp.so");
GP;install("cover","GGLp","cover","./sequence.gp.so");
GP;install("init","","init","./sequence.gp.so");
GP;install("save_match","vGG","save_match","./sequence.gp.so");
GP;install("find_covers","vGGLp","find_covers","./sequence.gp.so");
GP;install("find_covers_sets","vGGGGLp","find_covers_sets","./sequence.gp.so");
GP;install("format_thousands","G","format_thousands","./sequence.gp.so");
GP;install("print_memory","vp","print_memory","./sequence.gp.so");
GP;install("print_time","vGGGp","print_time","./sequence.gp.so");
GP;install("sequence","vGGGLLp","sequence","./sequence.gp.so");
*/
void init_sequence(void);
GEN cover_or_bitvec(GEN n, GEN d);
GEN cover_or_ratio(GEN n, GEN v, long prec);
GEN cover_xor_bitvec(GEN n, GEN d);
GEN cover_xor_ratio(GEN n, GEN d, long prec);
long is_divisible(GEN d, GEN V);
GEN is_divisible_set(GEN S);
void backtrack(GEN N, GEN D, GEN results, GEN start, GEN current_factors, GEN current_prod);
void search(GEN min_n, GEN max_n, GEN n_step, GEN i, GEN j, GEN prod, GEN S, GEN results);
GEN product_sets(GEN min_n, GEN max_n, GEN n_step);
GEN find_divisors(GEN N);
GEN cover(GEN n, GEN v, long mode, long prec);
GEN init(void);
void save_match(GEN c, GEN n);
void find_covers(GEN N, GEN targets, long mode, long prec);
void find_covers_sets(GEN min_n, GEN max_n, GEN istep, GEN targets, long mode, long prec);
GEN format_thousands(GEN n);
void print_memory(long prec);
void print_time(GEN imin, GEN imax, GEN ms, long prec);
void sequence(GEN imin, GEN imax, GEN targets, long mode, long in_memory, long prec);
/*End of prototype*/

static GEN /*global bool USE_MAP */
USE_MAP;
static GEN /*global bool USE_VEC */
USE_VEC;
static GEN /*global small OR_RATIO */
OR_RATIO;
static GEN /*global small OR_BITVEC */
OR_BITVEC;
static GEN /*global small XOR_RATIO */
XOR_RATIO;
static GEN /*global small XOR_BITVEC */
XOR_BITVEC;
static GEN /*global int icount */
icount;
/*End of global vars*/

void
init_sequence(void)	  /* void */
{
  USE_MAP = pol_x(fetch_user_var("USE_MAP"));
  USE_VEC = pol_x(fetch_user_var("USE_VEC"));
  OR_RATIO = pol_x(fetch_user_var("OR_RATIO"));
  OR_BITVEC = pol_x(fetch_user_var("OR_BITVEC"));
  XOR_RATIO = pol_x(fetch_user_var("XOR_RATIO"));
  XOR_BITVEC = pol_x(fetch_user_var("XOR_BITVEC"));
  icount = pol_x(fetch_user_var("icount"));
  /*global bool USE_MAP */
  USE_MAP = gen_0;
  /*global bool USE_VEC */
  USE_VEC = gen_1;
  /*global small OR_RATIO */
  OR_RATIO = gen_1;
  /*global small OR_BITVEC */
  OR_BITVEC = gen_2;
  /*global small XOR_RATIO */
  XOR_RATIO = stoi(4);
  /*global small XOR_BITVEC */
  XOR_BITVEC = stoi(8);
  /*global int icount */
  icount = gen_0;
  return;
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
          gel(/*print("n = ", n, ", bit = ", bit, ", #bits = ", (#bits)); */
          bits, gtos(bit)) = gen_1;
      }
    }
  }
  /*print("factors = ", d, ", ", vecsum(bits), "/", n, " = ", vecsum(bits)/n); */
  /*vecsum(bits)/n; */
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

GEN
cover_xor_bitvec(GEN n, GEN d)
{
  GEN bits;
  GEN p1;	  /* int */
  GEN p2;	  /* vec */
  long l3;	  /* lg */
  long l4;
  GEN p5;
  if (typ(n) != t_INT)
    pari_err_TYPE("cover_xor_bitvec",n);
  if (!is_matvec_t(typ(d)))
    pari_err_TYPE("cover_xor_bitvec",d);
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
          gel(/*print("n = ", n, ", bit = ", bit, ", #bits = ", (#bits)); */
          bits, gtos(bit)) = gsubsg(1, gel(bits, gtos(bit)));
      }
    }
  }
  /*print("factors = ", d, ", ", vecsum(bits), "/", n, " = ", vecsum(bits)/n); */
  /*vecsum(bits)/n; */
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
cover_xor_ratio(GEN n, GEN d, long prec)
{
  GEN ilen, S = gen_0, p1;
  if (typ(n) != t_INT)
    pari_err_TYPE("cover_xor_ratio",n);
  if (!is_matvec_t(typ(d)))
    pari_err_TYPE("cover_xor_ratio",d);
  ilen = stoi(lg(d)-1);
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
            L = glcm(L, gel(d, gtos(i)));
            bits = gaddgs(bits, 1);
          }
        }
      }
      S = gadd(S, gdiv(gpow(gen_m2, gsubgs(bits, 1), prec), L));
    }
  }
  return S;
}

long
is_divisible(GEN d, GEN V)
{
  long l1;
  l1 = glength(V);
  {
    long j;
    for (j = 1; j <= l1; ++j)
    {
      if (gequal0(gmod(d, gel(V, j))) || gequal0(gmod(gel(V, j), d)))
        return 1;
    }
  }
  return 0;
}

GEN
is_divisible_set(GEN S)
{
  GEN ok = pol_x(fetch_user_var("ok"));
  long l1;
  ok = gen_1;
  l1 = glength(S);
  {
    long i;
    for (i = 1; i <= l1; ++i)
    {
      long l2, l3;
      l2 = i + 1;
      l3 = glength(S);
      {
        GEN j;
        for (j = stoi(l2); gcmpgs(j, l3) <= 0; j = gaddgs(j, 1))
        {
          if (gequal0(gmod(gel(S, i), gel(S, gtos(j)))) || gequal0(gmod(gel(S, gtos(j)), gel(S, i))))
          {
            ok = gen_0;
            goto label1;
          }
        }
      }
    }
    label1:;
  }
  return ok;
}

void
backtrack(GEN N, GEN D, GEN results, GEN start, GEN current_factors, GEN current_prod)	  /* void */
{
  long l1;
  current_factors = gcopy(current_factors);
  if (gequal(current_prod, N))
  {
    listput0(results, gtovec(current_factors), 0);
    return;
  }
  l1 = glength(D);
  {
    GEN i;
    for (i = gcopy(start); gcmpgs(i, l1) <= 0; i = gaddgs(i, 1))
    {
      GEN d;
      d = gcopy(gel(D, gtos(i)));
      if (!gequal0(gmod(N, gmul(current_prod, d))))
        continue;
      if (is_divisible(d, current_factors))
        continue;
      listput0(current_factors, d, 0);
      backtrack(N, D, results, gaddgs(i, 1), current_factors, gmul(current_prod, d));
      listpop0(current_factors, 0);
    }
  }
  return;
}

void
search(GEN min_n, GEN max_n, GEN n_step, GEN i, GEN j, GEN prod, GEN S, GEN results)	  /* void */
{
  GEN p1;
  if ((((gcmpgs(j, 2) >= 0) && (gcmp(prod, min_n) >= 0)) && gequal0(gmod(prod, n_step))) && !gequal0(is_divisible_set(S)))
  {
    if (!gequal0(USE_MAP))
    {
      GEN tmp = gen_0;
      if (!mapisdefined(results, prod, NULL))
        tmp = mklist();
      else
        tmp = mapget(results, prod);
      listput0(tmp, S, 0);
      mapput(results, prod, tmp);
    }
    if (!gequal0(USE_VEC))
      /*print("#results = ", #results, ", (prod - min_n)\\n_step + 1 = ", (prod - min_n)\n_step + 1); */
      listput0(gel(results, gtos(gaddgs(gdivent(gsub(prod, min_n), n_step), 1))), S, 0);
  }
  p1 = gdiventgs(max_n, 2);
  {
    GEN k;
    for (k = gcopy(i); gcmp(k, p1) <= 0; k = gaddgs(k, 1))
    {
      if (gcmp(gmul(prod, k), max_n) > 0)
        break;
      /*listput(S, k); */
      search(min_n, max_n, n_step, gaddgs(k, 1), gaddgs(j, 1), gmul(prod, k), gconcat(S, k), results);
    }
  }
  return;
}

/*listpop(S); */
GEN
product_sets(GEN min_n, GEN max_n, GEN n_step)
{
  GEN results = pol_x(fetch_user_var("results"));
  if (typ(min_n) != t_INT)
    pari_err_TYPE("product_sets",min_n);
  if (typ(max_n) != t_INT)
    pari_err_TYPE("product_sets",max_n);
  if (typ(n_step) != t_INT)
    pari_err_TYPE("product_sets",n_step);
  if (!gequal0(USE_MAP))
    results = mkmap();
  else
  {
    GEN p1;	  /* int */
    GEN p2;	  /* vec */
    p1 = addis(truedivii(subii(max_n, min_n), n_step), 1);
    {
      long i;
      p2 = cgetg(itos(p1)+1, t_VEC);
      for (i = 1; cmpsi(i, p1) <= 0; ++i)
        gel(p2, i) = mklist();
    }
    results = p2;
  }
  /*print("product_sets(min_n = ", min_n, ", max_n = ", max_n, ", n_step = ", n_step, ", #results = ", #results); */
  search(min_n, max_n, n_step, gen_2, gen_0, gen_1, cgetg(1, t_VEC), results);
  return /*vecsort(results, (a,b) -> a[1] > b[1]); */
  results;
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
  GEN D, results, current_factors;
  if (typ(N) != t_INT)
    pari_err_TYPE("find_divisors",N);
  D = genselect(mkvec(N), wrap_anon_0, divisors(N));
  results = mklist();
  current_factors = mklist();
  backtrack(N, D, results, gen_1, current_factors, gen_1);
  return gtovec(results);
}

GEN
cover(GEN n, GEN v, long mode, long prec)
{
  if (typ(n) != t_INT)
    pari_err_TYPE("cover",n);
  if (!is_matvec_t(typ(v)))
    pari_err_TYPE("cover",v);
  if (gequalsg(mode, OR_RATIO))
    return cover_or_ratio(n, v, prec);
  if (gequalsg(mode, OR_BITVEC))
    return cover_or_bitvec(n, v);
  if (gequalsg(mode, XOR_RATIO))
    return cover_xor_ratio(n, v, prec);
  if (gequalsg(mode, XOR_BITVEC))
    return cover_xor_bitvec(n, v);
  return gen_0;
}

/*global GEN summary */
GEN
init(void)
{
  GEN summary = pol_x(fetch_user_var("summary"));
  return summary = mkmap();
}

void
save_match(GEN c, GEN n)	  /* void */
{
  GEN tmp = gen_0, summary = pol_x(fetch_user_var("summary"));
  if (!mapisdefined(summary, c, NULL))
    tmp = mklist();
  else
    tmp = mapget(summary, c);
  listput0(tmp, n, 0);
  mapput(summary, c, tmp);
  return;
}

void
find_covers(GEN N, GEN targets, long mode, long prec)	  /* void */
{
  GEN D;
  long l1;
  if (typ(N) != t_INT)
    pari_err_TYPE("find_covers",N);
  if (!is_matvec_t(typ(targets)))
    pari_err_TYPE("find_covers",targets);
  if (gequal0(icount))
    pari_printf("find_covers()\n");
  icount = gaddgs(icount, 1);
  D = find_divisors(N);
  /*print("n=", N, ", divisors=", D); */
  l1 = glength(D);
  {
    long i;
    for (i = 1; i <= l1; ++i)
    {
      GEN c;
      c = cover(N, gel(D, i), mode, prec);
      if (vecsearch(targets, c, NULL))
      {
        save_match(c, N);
        pari_printf("n=%Ps, cover=%Ps, factors=%Ps\n", N, c, gel(D, i));
      }
    }
  }
  return;
}

void
find_covers_sets(GEN min_n, GEN max_n, GEN istep, GEN targets, long mode, long prec)	  /* void */
{
  GEN n, istepsize = stoi(16384), Si = pol_x(fetch_user_var("Si"));
  if (typ(min_n) != t_INT)
    pari_err_TYPE("find_covers_sets",min_n);
  if (typ(max_n) != t_INT)
    pari_err_TYPE("find_covers_sets",max_n);
  if (typ(istep) != t_INT)
    pari_err_TYPE("find_covers_sets",istep);
  if (!is_matvec_t(typ(targets)))
    pari_err_TYPE("find_covers_sets",targets);
  pari_printf("find_covers_sets()\n");
  default0("parisizemax", GENtostr_unquoted(powis(gen_2, 30)));
  default0("parisize", GENtostr_unquoted(powis(gen_2, 26)));
  n = icopy(min_n);
  /*my(istepsize = 8192); */
  while (gcmp(n, max_n) < 0)
  {
    GEN S;
    S = product_sets(n, gadd(n, istepsize), istep);
    /*print("n = ", n, ", n + istepsize = ", n + istepsize, ", n_step = ", n_step, ", max_n = ", max_n, ", S[126]=", S[126]); */
    if (!gequal0(USE_MAP))
    {
      long l1;	  /* bool */
      l1 = cmpis(istep, 0) > 0;
      {
        GEN i;
        for (i = icopy(istep); l1?gcmp(i, max_n) <= 0:gcmp(i, max_n) >= 0; i = gadd(i, istep))
        {
          if (mapisdefined(S, i, NULL))
          {
            long l2;
            Si = mapget(S, i);
            /*print("key = ", i, ", value = ", Si); */
            l2 = glength(Si);
            {
              long j;
              for (j = 1; j <= l2; ++j)
              {
                GEN c;
                c = cover(i, gel(Si, j), mode, prec);
                if (vecsearch(targets, c, NULL))
                {
                  save_match(c, i);
                  pari_printf("n=%Ps, cover=%Ps, factors=%Ps\n", i, c, gel(Si, j));
                }
              }
            }
          }
        }
      }
    }
    if (!gequal0(USE_VEC))
    {
      long l3;
      l3 = glength(S);
      {
        long i;
        for (i = 1; i <= l3; ++i)
        {
          long l4;
          l4 = glength(gel(S, i));
          {
            long j;
            for (j = 1; j <= l4; ++j)
            {
              GEN c;
              c = cover(gsubgs(gadd(n, mulsi(i, istep)), 1), gel(gel(S, i), j), mode, prec);
              if (vecsearch(targets, c, NULL))
              {
                save_match(c, gsub(gadd(n, mulsi(i, istep)), istep));
                pari_printf("n=%Ps, cover=%Ps, factors=%Ps\n", gsub(gadd(n, mulsi(i, istep)), istep), c, gel(gel(S, i), j));
              }
            }
          }
        }
      }
    }
    n = gadd(n, istepsize);
  }
  return;
}

GEN
format_thousands(GEN n)
{
  GEN s, len, res;
  s = gtovec(GENtoGENstr(n));
  len = stoi(glength(s));
  res = cgetg(1, t_VEC);
  {
    GEN i;
    for (i = len; gcmpgs(i, 1) >= 0; i = gaddgs(i, -3))
    {
      GEN start, chunk;
      start = gmaxsg(1, gsubgs(i, 2));
      chunk = gconcat1(vecslice0(s, gtos(start), gtos(i)));
      if (glength(res) == 0)
      {
        GEN p1;	  /* vec */
        p1 = cgetg(2, t_VEC);
        gel(p1, 1) = gcopy(chunk);
        res = p1;
      }
      else
      {
        GEN p2;	  /* vec */
        p2 = cgetg(3, t_VEC);
        gel(p2, 1) = gcopy(chunk);
        gel(p2, 2) = strtoGENstr(",");
        res = gconcat(p2, res);
      }
    }
  }
  return gconcat1(res);
}

void
print_memory(long prec)	  /* void */
{
  GEN stack_bytes, heap_info, stack_mb, heap_bytes, heap_mb;
  stack_bytes = stoi(getstack());
  heap_info = getheap();
  stack_mb = gdiv(stack_bytes, stor(1048576, prec));
  heap_bytes = gmulgs(gel(heap_info, 2), 8);
  heap_mb = gdiv(heap_bytes, stor(1048576, prec));
  if (gcmp(stack_mb, strtor("0.01", prec)) >= 0)
    printf0("Stack Memory: %.2f MB\n", mkvec(stack_mb));
  printf0("Heap Memory:  %.2f MB\n", mkvec(heap_mb));
  return;
}

void
print_time(GEN imin, GEN imax, GEN ms, long prec)	  /* void */
{
  GEN sec, remaining_ms;
  printf0("%s to %s in ", mkvec2(format_thousands(imin), format_thousands(imax)));
  sec = gdiv(ms, stor(1000, prec));
  remaining_ms = gmodgs(ms, 1000);
  if (gcmp(sec, stor(60, prec)) >= 0)
  {
    GEN min, remaining_sec;
    min = gdiventgs(sec, 60);
    remaining_sec = gmodgs(sec, 60);
    printf0("%d min %d sec\n", mkvec2(min, remaining_sec));
  }
  else
  {
    if (gcmpgs(sec, 1) >= 0)
      printf0("%.2f sec\n", mkvec(sec));
    else
      printf0("%d ms\n", mkvec(ms));
  }
  return;
}

/*
sequence(2, 131072, [1/2], OR_RATIO, 1);
USE_MAP
Heap Memory:  72.63 MB
2 to 131,072 in 54.00 sec

sequence(2, 131072, [1/2], OR_RATIO, 1);
USE_VEC
Heap Memory:  34.46-66.57 MB
2 to 131,072 in 21.81-29.00 sec

sequence(2, 131072, [1/2], OR_RATIO, 0);
Heap Memory:  34.59 MB
2 to 131,072 in 1 min 9 sec

sequence(2, 1048576, [1/2], OR_RATIO, 0);
Heap Memory:  37.25 MB
2 to 1,048,576 in 15 min 14 sec

sequence(2, 1048576, [1/2], OR_RATIO, 1);
USE_VEC
Heap Memory:  102.68 MB
2 to 1,048,576 in 10 min 42 sec

curl -O https://pari.math.u-bordeaux.fr/pub/pari/unix/pari-2.17.3.tar.gz
tar -xvf pari-2.17.3.tar.gz

curl -O https://pari.math.u-bordeaux.fr/pub/pari/GP2C/gp2c-0.0.14.tar.gz
curl -O https://pari.math.u-bordeaux.fr/pub/pari/GP2C/gp2c-0.0.14pl1.tar.gz
tar -xvf gp2c-0.0.14pl1.tar.gz

pacman -S git make gcc autoconf automake libtool perl diffutils
cd pari-2.17.3
./configure
make gp
make install

cd gp2c-0.0.14pl1
./configure
make
make install

cd "/c/Program Files (x86)/Pari64-2-17-3"
gp2c sequence.gp > sequence.c
gcc -shared -O2 -I/usr/local/include -o sequence.dll sequence.c /usr/local/bin/libpari.dll.a
install("sequence", "VLLGLL", "sequence", "./sequence.dll");
sequence(2, 300, [1/2], OR_RATIO, 0);

I suggest to use the Windows subsystem for Linux.
https://pari.math.u-bordeaux.fr/PDF/PARIwithWindows.pdf
sudo apt install pari-gp
sudo apt install pari-gp2c

*/
void
sequence(GEN imin, GEN imax, GEN targets, long mode, long in_memory, long prec)	  /* void */
{
  GEN istep = gen_1;
  long l1;	  /* lg */
  GEN summary = pol_x(fetch_user_var("summary")), tmp = pol_x(fetch_user_var("tmp")), elapsed;
  if (typ(imin) != t_INT)
    pari_err_TYPE("sequence",imin);
  if (typ(imax) != t_INT)
    pari_err_TYPE("sequence",imax);
  if (!is_matvec_t(typ(targets)))
    pari_err_TYPE("sequence",targets);
  init();
  printf0("sequence(%d, %d, %s, ", mkvec3(imin, imax, targets));
  if (gequalsg(mode, OR_RATIO))
    printf0("OR_RATIO", cgetg(1, t_VEC));
  if (gequalsg(mode, OR_BITVEC))
    printf0("OR_BITVEC", cgetg(1, t_VEC));
  if (gequalsg(mode, XOR_RATIO))
    printf0("XOR_RATIO", cgetg(1, t_VEC));
  if (gequalsg(mode, XOR_BITVEC))
    printf0("XOR_BITVEC", cgetg(1, t_VEC));
  printf0(", %d);\n", mkvec(stoi(in_memory)));
  if (in_memory && !gequal0(USE_VEC))
    pari_printf("USE_VEC\n");
  if (in_memory && !gequal0(USE_MAP))
    pari_printf("USE_MAP\n");
  gettime();
  if ((lg(targets)-1) == 1)
    istep = ginv(gel(targets, 1));
  if (in_memory)
    find_covers_sets(imin, imax, istep, targets, mode, prec);
  if (!(in_memory))
  {
    long l2;	  /* bool */
    l2 = gcmpgs(istep, 0) > 0;
    {
      GEN i;
      for (i = icopy(imin); l2?gcmp(i, imax) <= 0:gcmp(i, imax) >= 0; i = gadd(i, istep))
        find_covers(i, targets, mode, prec);
    }
  }
  l1 = lg(targets);
  {
    long i;
    for (i = 1; i < l1; ++i)
    {
      if (mapisdefined(summary, gel(targets, i), NULL))
      {
        tmp = mapget(summary, gel(targets, i));
        printf0("%s: %s\n", mkvec2(gel(targets, i), gtovec(tmp)));
      }
    }
  }
  elapsed = stoi(gettime());
  print_memory(prec);
  print_time(imin, imax, elapsed, prec);
  return;
}

