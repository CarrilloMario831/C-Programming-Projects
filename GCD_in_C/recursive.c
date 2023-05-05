#include <stdio.h>
#include <stdlib.h>
#include "recursive.h"

int gcd_recursive(int m, int n){
	int res = m == 0 ? abs(n) :
		n == 0 ? abs(m) :
			gcd_recursive(n, m % n);
	return res;
}
