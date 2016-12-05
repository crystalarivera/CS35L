#include "randlib.h"
#include<immintrin.h>

/* As you can find out in 'randall.c', the hardware implementation constructor 
and destructor are not doing anything, so it is ok to ignore them in your
'randlibhw.c'. */


/* Initialize the hardware rand64 implementation.  */
extern unsigned long long hardware_rand64 (void)
{
  unsigned long long int x;
  while (! _rdrand64_step (&x))
    continue;
  return x;
}
