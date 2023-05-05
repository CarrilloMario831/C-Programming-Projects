#include <stdio.h>
#include <stdlib.h>
#include "iterative.h"

int gcd_iterative(int m, int n){
	int tr = 1;
	int remainder = -1;
        int absM = abs(m);
        int absN = abs(n);
       	while(tr == 1){
            //while we have not reached a solution
            if (m == 0 && n != 0) {
                return absN;
            }
            //if m is 0 and n is not 0, then re turn the positive value of n
            else if (m != 0 && n == 0) {
                return absM;
            }
            //if m is not 0 and n is 0 then return the positive value of m
            while (m != 0 && n != 0) {
            //while m is not 0 and n is not 0
                if (absM == absN) {
                    return absM;
                }
                //if m is the same value as n, then we return whichever value since they are the same
                else if (absM > absN) {
                    //else if m is greater than n
                    while (remainder != 0) {
                        //while the remainder is not 0
                        remainder = absM % absN;
                        //set the remainder be the modulo of m and n
                        absM = absN;
                        //change the value of m to be n
                        absN = remainder;
                        //change the value of n to be the remainder
                        if (absN == 0) {
                            //if n is 0
                            return absM;
                            //return m
                        }
                    }
                }
                else if (absN > absM) {
                    //else if n is greater than m
                    while (remainder != 0) {
                        //while the remainder is not 0
                        remainder = absN % absM;
                        //set the remainder be the mod of n and m
                        absN = absM;
                        //change the value of n to be m
                        absM = remainder;
                        //change the value of m to be the remainder
                        if (absM == 0) {
                            //if m is 0
                            return absN;
                            //return n
                        }
                    }
                }
            }
        }
	return 0;
}
