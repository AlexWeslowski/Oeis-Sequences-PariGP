
\\ cd "/c/Program Files (x86)/Pari64-2-17-3/"
\\ gp2c -S seqtest.gp > seqtest.c
\\ gcc -shared -O2 -I/usr/local/include -L/usr/local/lib -o seqtest.dll seqtest.c -lpari
\\ gcc -shared -O2 -I/usr/local/include -o seqtest.dll seqtest.c /usr/local/bin/libpari.dll.a
\\ install("cover_or_bitvec", "LG", "cover_or_bitvec", "./seqtest.dll");
\\ cover_or_bitvec(12, [3, 4]);

addone(n:int) = {
	n + 1;
}

cover_or_bitvec(n:int, d:vec) = {
	\\my(bits = Vecsmall(vector(n+1, i, 0)));
	my(bits = vector(n+1, i, 0));
	for(i = 1, #d,
		forstep(bit=d[i], n+1, d[i], 
			bits[bit] = 1;
		);
	);
	(sum(i=1, #bits, bits[i])/n);
}

cover_or_ratio(n:int, v:vec)=
{
	my(ilen=#v, c=0);
	for(mask=1, 2^ilen-1,
		my(L=1, bits=0);
		for(i=1, ilen,
			if(bittest(mask, i-1),
				L = lcm(L, v[i]);
				bits++;
			);
		);
		c += (-1)^(bits+1)/L;
	);
	c;
}

/*
install("is_divisible", "lGG", "is_divisible", "./seqtest.dll");
is_divisible(3, [2, 9]);
*/
is_divisible(d, V:list)=
{
	for(j = 1, #V,
		if(max(d, V[j]) % min(d, V[j]) == 0,
			return(1)
		);
	);
	0;
}

/*
install("init_seqtest", "v", "init_seqtest", "./seqtest.dll");
install("is_divisible", "lGG", "is_divisible", "./seqtest.dll");
install("backtrack", "vGGGGG", "backtrack", "./seqtest.dll");
current_factors = List();
init_seqtest(); backtrack(12, divisors(12), 1, current_factors, 1);
*/
backtrack(N:int, D:vec, start:int, current_factors:list, current_prod:int)=
{
	if(current_prod == N,
		listput(results, Vec(current_factors));
		return()
	);
	my(factors = List(current_factors));
	for(i = start, #D,
		my(d = D[i]);
		if(N % (current_prod*d) != 0, next);
		if(is_divisible(d, factors), next);
		listput(factors, d);
		backtrack(N, D, i+1, factors, current_prod*d);
		listpop(factors);
	);
}

results = List();

/*
install("init_seqtest", "v", "init_seqtest", "./seqtest.dll");
install("find_divisors", "G", "find_divisors", "./seqtest.dll");
init_seqtest(); d = find_divisors(12);
*/
find_divisors(N:int)=
{
	my(D = select(d -> d > 1 && d < N, divisors(N)));
	results = List();
	my(current_factors = List());
	backtrack(N, D, 1, current_factors, 1);
	(Vec(results));
}
