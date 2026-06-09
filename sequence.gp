
cover_or_bitvec(n:int, d:vec) = {
	\\my(bits = Vecsmall(vector(n+1, i, 0)));
	my(bits = vector(n+1, i, 0));
	for(i = 1, #d,
		forstep(bit=d[i], n+1, d[i], 
			\\print("n = ", n, ", bit = ", bit, ", #bits = ", (#bits));
			bits[bit] = 1;
		);
	);
	\\print("factors = ", d, ", ", vecsum(bits), "/", n, " = ", vecsum(bits)/n);
	\\vecsum(bits)/n;
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

cover_xor_bitvec(n:int, d:vec) = {
	\\my(bits = Vecsmall(vector(n+1, i, 0)));
	my(bits = vector(n+1, i, 0));
	for(i = 1, #d,
		forstep(bit=d[i], n+1, d[i], 
			\\print("n = ", n, ", bit = ", bit, ", #bits = ", (#bits));
			bits[bit] = 1 - bits[bit];
		);
	);
	\\print("factors = ", d, ", ", vecsum(bits), "/", n, " = ", vecsum(bits)/n);
	\\vecsum(bits)/n;
	(sum(i=1, #bits, bits[i])/n);
}

cover_xor_ratio(n:int, d:vec)=
{
    my(ilen=#d, S=0);
    for(mask=1, 2^ilen-1,
        my(L=1, bits=0);
        for(i=1, ilen,
            if(bittest(mask,i-1),
                L = lcm(L, d[i]);
                bits++;
            );
        );
        S += (-2)^(bits-1) / L;
    );
    S;
}

is_divisible(d:int, V:list)=
{
	for(j = 1, #V,
		if(d % V[j] == 0 || V[j] % d == 0,
			return(1)
		);
	);
	0;
}

is_divisible_set(S:list)=
{
	ok = 1;
	for(i = 1, #S,
		for(j = i+1, #S,
			if(S[i] % S[j] == 0 || S[j] % S[i] == 0,
				ok = 0;
				break(2);
			);
		);
	);
	ok;
}


/*
install("backtrack_reference", "vGGGGGG", "backtrack_reference", "./sequence.dll");
current_factors = List();
results = List();
backtrack_reference(12, divisors(12), ~results, 1, current_factors, 1); backtrack_reference(168, divisors(168), ~results, 1, current_factors, 1);
*/
backtrack_reference(N, D, ~results, start, current_factors, current_prod)=
{
	if(current_prod == N,
		listput(results, Vec(current_factors));
		return()
	);
	/*
	for(i = start, #D,
		my(d = D[i]);
		if(N % (current_prod*d), next);
		if(is_divisible(d, current_factors), next);
		listput(current_factors, d);
		backtrack_reference(N, D, ~results, i+1, current_factors, current_prod*d);
		listpop(current_factors);
	);
	*/
	my(factors = List(current_factors));
	for(i = start, #D,
		my(d = D[i]);
		if(N % (current_prod*d), next);
		if(is_divisible(d, factors), next);
		listput(factors, d);
		backtrack_reference(N, D, ~results, i+1, factors, current_prod*d);
		listpop(factors);
	);
}


/*
install("init_sequence", "v", "init_sequence", "./sequence.dll");
install("backtrack", "vGGGGG", "backtrack", "./sequence.dll");
current_factors = List();
init_sequence(); backtrack(12, divisors(12), 1, current_factors, 1); backtrack(168, divisors(168), 1, current_factors, 1);
*/
\\backtrack(N, D, ~results, start, current_factors, current_prod)=
backtrack(N:int, D:vec, start:int, current_factors:list, current_prod:int)=
{
	if(current_prod == N,
		listput(results, Vec(current_factors));
		return()
	);
	for(i = start, #D,
		my(d = D[i]);
		if(N % (current_prod*d), next);
		if(is_divisible(d, current_factors), next);
		listput(current_factors, d);
		\\backtrack(N, D, ~results, i+1, current_factors, current_prod*d);
		backtrack(N, D, i+1, current_factors, current_prod*d);
		listpop(current_factors);
	);
	/*
	my(factors = List(current_factors));
	for(i = start, #D,
		my(d = D[i]);
		if(N % (current_prod*d), next);
		if(is_divisible(d, factors), next);
		listput(factors, d);
		\\backtrack(N, D, ~results, i+1, factors, current_prod*d);
		backtrack(N, D, i+1, factors, current_prod*d);
		listpop(factors);
	);
	*/
}

\\global bool USE_MAP
USE_MAP = 0;
\\global bool USE_VEC
USE_VEC = 1;

search(min_n, max_n, n_step, i, j, prod, S, ~results)=
{
	if(j >= 2 && prod >= min_n && prod % n_step == 0 && is_divisible_set(S),
		if(USE_MAP, 
			my(tmp);
			if(!mapisdefined(results, prod),
				tmp = List(),
				tmp = mapget(results, prod)
			);
			listput(tmp, S);
			mapput(results, prod, tmp);
		);
		if(USE_VEC,
			\\print("#results = ", #results, ", (prod - min_n)\\n_step + 1 = ", (prod - min_n)\n_step + 1);
			listput(results[(prod - min_n)\n_step + 1], S);
		);
	);
	for(k = i, max_n\2,
		if(prod * k > max_n, break);
		\\listput(S, k);
		search(min_n, max_n, n_step, k + 1, j + 1, prod * k, concat(S, k), ~results);
		\\listpop(S);
	);
};

product_sets(min_n:int, max_n:int, n_step:int)=
{
	if(USE_MAP,
		results = Map(),
		results = vector((max_n - min_n)\n_step + 1, i, List());
	);
	\\print("product_sets(min_n = ", min_n, ", max_n = ", max_n, ", n_step = ", n_step, ", #results = ", #results);
    search(min_n, max_n, n_step, 2, 0, 1, [], ~results);
    \\vecsort(results, (a,b) -> a[1] > b[1]);
	results;
}

results = List();

/*
install("init_sequence", "v", "init_sequence", "./sequence.dll");
install("find_divisors", "G", "find_divisors", "./sequence.dll");
D = find_divisors(168);
*/
find_divisors(N:int)=
{
	my(D = select(d -> d > 1 && d < N, divisors(N)));
	my(myresults = List());
	\\results = List();
	my(current_factors = List());
	backtrack_reference(N, D, ~myresults, 1, current_factors, 1);
	\\backtrack(N, D, 1, current_factors, 1);
	(Vec(myresults));
}

\\global small OR_RATIO
OR_RATIO = 1;
\\global small OR_BITVEC
OR_BITVEC = 2;
\\global small XOR_RATIO
XOR_RATIO = 4;
\\global small XOR_BITVEC
XOR_BITVEC = 8;

/*
install("init_sequence", "v", "init_sequence", "./sequence.dll");
install("cover", "GGG", "cover", "./sequence.dll");
install("cover_or_ratio", "GG", "cover_or_ratio", "./sequence.dll");
init_sequence(); c = cover_or_ratio(12, [3, 4]);
init_sequence(); c = cover(12, [3, 4], 1);
*/
cover(n:int, v:vec, mode:int)=
{	
	/*
	OR_RATIO = 1;
	OR_BITVEC = 2;
	XOR_RATIO = 4;
	XOR_BITVEC = 8;
	*/
    if(mode==OR_RATIO, return(cover_or_ratio(n, v)));
    if(mode==OR_BITVEC, return(cover_or_bitvec(n, v)));
    if(mode==XOR_RATIO, return(cover_xor_ratio(n, v)));
    if(mode==XOR_BITVEC, return(cover_xor_bitvec(n, v)));
}

\\global int icount
icount = 0;
summary = Map();

/*
install("init_sequence", "v", "init_sequence", "./sequence.dll");
install("save_match", "vGG", "save_match", "./sequence.dll");
init_sequence(); save_match(1/2, 12); save_match(1/2, 168);
*/
save_match(c, n)=
{
	my(tmp);
	if(!mapisdefined(summary, c),
		tmp = List(),
		tmp = List(mapget(summary, c))
	);
	listput(tmp, n);
	mapput(summary, c, tmp);
}

/*
install("init_sequence", "v", "init_sequence", "./sequence.dll");
install("find_divisors", "G", "find_divisors", "./sequence.dll");
install("find_covers", "vGGG", "find_covers", "./sequence.dll");
init_sequence(); d = find_divisors(12);
init_sequence(); find_covers(12, [1/2], 1); find_covers(168, [1/2], 1);
*/
find_covers(N:int, targets:vec, mode:int)=
{
	if(icount == 0,
		print("find_covers()");
	);
	icount++;
	my(D = find_divisors(N));
	\\print("n=", N, ", divisors=", D);
	for(i = 1, #D,
		my(c = cover(N, D[i], mode));
		if(vecsearch(targets, c),
			save_match(c, N);
			print("n=", N, ", cover=", c, ", factors=", D[i]);
		);
	);
	\\kill(D);
}

find_covers_sets(min_n:int, max_n:int, istep:int, targets:vec, mode:small)=
{
	print("find_covers_sets()");
	default(parisizemax, 2^30);
	default(parisize, 2^26);
	my(n = min_n);
	\\my(istepsize = 65536);
	\\my(istepsize = 32768);
	my(istepsize = 16384);
	\\my(istepsize = 8192);
	while(n < max_n,
		my(S = product_sets(n, n + istepsize, istep));
		\\print("n = ", n, ", n + istepsize = ", n + istepsize, ", n_step = ", n_step, ", max_n = ", max_n, ", S[126]=", S[126]);
		if(USE_MAP,
			forstep(i = istep, max_n, istep,
				if(mapisdefined(S, i),
					Si = mapget(S, i);
					\\print("key = ", i, ", value = ", Si);
					for(j = 1, #Si,
						my(c = cover(i, Si[j], mode));
						if(vecsearch(targets, c),
							save_match(c, i);
							print("n=", i, ", cover=", c, ", factors=", Si[j]);
						);
					);			
				);
			);
		);
		if(USE_VEC,
			for(i = 1, #S,
				for(j = 1, #S[i],
					my(c = cover(n + i * istep - 1, S[i][j], mode));
					if(vecsearch(targets, c),
						save_match(c, n + i * istep - istep);
						print("n=", n + i * istep - istep, ", cover=", c, ", factors=", S[i][j]);
					);
				);
			);
		);
		n += istepsize;
	);
}

format_thousands(n) = {
  my(s = Vec(Str(n)), len = #s, res = []);
  forstep(i = len, 1, -3,
    my(start = max(1, i - 2), chunk = concat(s[start..i]));
    if (#res == 0, res = [chunk], res = concat([chunk, ","], res));
  );
  concat(res);
}

print_vars() = {
	my(results_mem = sizebyte(results));
	my(summary_mem = sizebyte(summary));
	if(results_mem >= 10485 || summary_mem >= 10485,
		printf("results\t%.2f MB\n", results_mem/1024.^2);
		printf("summary\t%.2f MB\n", summary_mem/1024.^2);
		,
		printf("results\t%s bytes\n", format_thousands(results_mem));
		printf("summary\t%s bytes\n", format_thousands(summary_mem));
	);
}

print_memory() = {
	my(stack_bytes = getstack());
	my(heap_info = getheap());
	my(stack_mb = stack_bytes / 1048576.0);
	my(heap_bytes = heap_info[2] * 8);
	my(heap_mb = heap_bytes / 1048576.0);
	if(stack_mb >= 0.01,
		printf("Stack Memory: %.2f MB\n", stack_mb);
	);
	printf("Heap Memory:  %.2f MB\n", heap_mb);
}

print_time(imin, imax, ms) = {
	printf("%s to %s in ", format_thousands(imin), format_thousands(imax));
	my(sec = ms / 1000.0, remaining_ms = ms % 1000);
	if (sec >= 60.0,
	my(min = sec \ 60, remaining_sec = sec % 60);
	printf("%d min %d sec\n", min, remaining_sec);
	,
	sec >= 1,
	printf("%.2f sec\n", sec);
	,
	printf("%d ms\n", ms);
	);
}

/*
install("init_sequence", "v", "init_sequence", "./sequence.dll");
install("print_results", "v", "print_results", "./sequence.dll");
init_sequence(); print_results();
*/
print_results()=
{
	printf("#results = %d\n", #results);
}

/*
install("init_sequence", "v", "init_sequence", "./sequence.dll");
install("save_match", "vGG", "save_match", "./sequence.dll");
install("print_summary", "vG", "print_summary", "./sequence.dll");
init_sequence(); save_match(1/2, 12); print_summary([1/2]);
*/
print_summary(targets:vec)=
{
	for(i = 1, #targets,
		if(mapisdefined(summary, targets[i]),
			tmp = mapget(summary, targets[i]);
			printf("%s: %s\n", targets[i], Vec(tmp));
		);
	);
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
/*
install("init_sequence", "v", "init_sequence", "./sequence.dll");
install("print_results", "v", "print_results", "./sequence.dll");
install("print_vars", "v", "print_vars", "./sequence.dll");
install("find_covers", "vGGG", "find_covers", "./sequence.dll");
install("sequence", "vGGGGG", "sequence", "./sequence.dll");
init_sequence(); find_covers(12, [1/2], 1); find_covers(168, [1/2], 1);
init_sequence(); sequence(2, 240, [1/2], 1, 0);
init_sequence(); sequence(2, 7000, [1/2], 1, 0);
init_sequence(); sequence(2, 65536, [1/2], 1, 0);
init_sequence(); sequence(2, 131072, [1/2], 1, 0);
init_sequence(); sequence(2, 2^20, [1/2], 1, 0);

\r sequence.gp
sequence(2, 65536, [1/2], 1, 0);
*/
sequence(imin:int, imax:int, targets:vec, mode:int, in_memory:int)=
{
	default(parisizemax, 2^28);
	default(parisize, 2^28);
	OR_RATIO = 1;
	OR_BITVEC = 2;
	XOR_RATIO = 4;
	XOR_BITVEC = 8;
	summary = Map();
	printf("sequence(%d, %d, %s, ", imin, imax, targets);
    if(mode==OR_RATIO, printf("OR_RATIO"));
    if(mode==OR_BITVEC, printf("OR_BITVEC"));
    if(mode==XOR_RATIO, printf("XOR_RATIO"));
    if(mode==XOR_BITVEC, printf("XOR_BITVEC"));
	printf(", %d);\n", in_memory);
    if(in_memory && USE_VEC, print("USE_VEC"));
    if(in_memory && USE_MAP, print("USE_MAP"));
	gettime();
	
	my(istep = 1);
	if(#targets == 1,
		istep = 1/targets[1]
	);
	if(in_memory,
		find_covers_sets(imin, imax, istep, targets, mode);
	);
	if(!in_memory, 
		forstep(i=imin, imax, istep,
			find_covers(i, targets, mode));
	);
	print_summary(targets);
	my(elapsed = gettime());
	print_memory();
	print_time(imin, imax, elapsed);
}
