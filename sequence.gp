
cover_or_bitvec(n:int, d:vec) = {
	my(bits = Vecsmall(vector(n+1, i, 0)));
	\\my(bits = vector(n+1, i, 0));
	for(i = 1, #d,
		forstep(bit=d[i], n+1, d[i], 
			bits[bit] = 1;
		);
	);
	vecsum(bits)/n;
	\\(sum(i=1, #bits, bits[i])/n);
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
	my(bits = Vecsmall(vector(n+1, i, 0)));
	\\my(bits = vector(n+1, i, 0));
	for(i = 1, #d,
		forstep(bit=d[i], n+1, d[i], 
			bits[bit] = 1 - bits[bit];
		);
	);
	vecsum(bits)/n;
	\\(sum(i=1, #bits, bits[i])/n);
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


backtrack_reference(N, D, ~results, start, current_factors, current_prod)=
{
	if(current_prod == N,
		listput(results, Vec(current_factors));
		return()
	);
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
}


search(min_n, max_n, n_step, i, j, prod, S, ~results)=
{
	if(j >= 2 && prod >= min_n && prod % n_step == 0 && is_divisible_set(S),
		\\print("#results = ", #results, ", (prod - min_n)\\n_step + 1 = ", (prod - min_n)\n_step + 1);
		listput(results[(prod - min_n)\n_step + 1], S);
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
	results = vector((max_n - min_n)\n_step + 1, i, List());
	\\print("product_sets(min_n = ", min_n, ", max_n = ", max_n, ", n_step = ", n_step, ", #results = ", #results);
    search(min_n, max_n, n_step, 2, 0, 1, [], ~results);
    \\vecsort(results, (a,b) -> a[1] > b[1]);
	results;
}

results = List();

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

OR_RATIO = 1;
OR_BITVEC = 2;
XOR_RATIO = 4;
XOR_BITVEC = 8;

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

icount = 0;
summary = Map();

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

find_covers(N:int, targets:vec, mode:int)=
{
	if(icount == 0,
		print("find_covers()");
	);
	icount++;
	my(D = find_divisors(N));
	for(i = 1, #D,
		my(c = cover(N, D[i], mode));
		if(vecsearch(targets, c),
			save_match(c, N);
			print(c, "\t", format_thousands(N), "\t", D[i]);
		);
	);
}

find_covers_sets(min_n:int, max_n:int, istep:int, targets:vec, mode:small)=
{
	print("find_covers_sets()");
	my(n = min_n);
	\\8192 16384 32768 65536
	my(istepsize = 16384);
	while(n < max_n,
		my(S = product_sets(n, n + istepsize, istep));
		for(i = 1, #S,
			for(j = 1, #S[i],
				my(c = cover(n + i * istep - 1, S[i][j], mode));
				if(vecsearch(targets, c),
					save_match(c, n + i * istep - istep);
					print(c, "\t", format_thousands(n + i * istep - istep), "\t", S[i][j]);
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

print_results()=
{
	printf("#results = %d\n", #results);
}

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
Heap Memory:  34.46-66.57 MB
2 to 131,072 in 21.81-29.00 sec

sequence(2, 131072, [1/2], OR_RATIO, 0);
Heap Memory:  34.59 MB
2 to 131,072 in 1 min 9 sec

sequence(2, 1048576, [1/2], OR_RATIO, 0);
Heap Memory:  37.25 MB
2 to 1,048,576 in 15 min 14 sec

sequence(2, 1048576, [1/2], OR_RATIO, 1);
Heap Memory:  102.68 MB
2 to 1,048,576 in 10 min 42 sec

sequence(2, 8388608, [1/2], OR_RATIO, 0);


curl -O https://pari.math.u-bordeaux.fr/pub/pari/unix/pari-2.17.3.tar.gz
tar -xvf pari-2.17.3.tar.gz

pacman -S git make gcc autoconf automake libtool perl diffutils
cd pari-2.17.3
./configure
make gp
make install

*/
/*

\r H:\PariGP\Sequence\sequence.gp

default(parisizemax, 2^28);
default(parisize, 2^28);
sequence(2, 65536, [1/2], 1, 0);

default(parisizemax, 2^30);
default(parisize, 2^28);
sequence(2, 65536, [1/2], 1, 1);

*/
sequence(imin:int, imax:int, targets:vec, mode:int, in_memory:int)=
{
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
