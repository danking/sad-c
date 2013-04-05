#include<xmmintrin.h>
#include<stdio.h>

#define N 16

void runsad(void* a, int alen, void* b, int blen);

int main()

{
  /* unsigned char a[N] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07}; */
  /* unsigned char b[N] = {0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f}; */

  /* unsigned char a[N] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; */
  /* unsigned char b[N] = {0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; */

  /* __m64 a = (__m64)0x0000000000000000LL; */
  /* __m64 b = (__m64)0x0808000000000000LL; */

  /* __m64 res = _mm_sad_pu8(a, b); */

  /* int i; */
  /* for (i = 0; i < N; i=i+8) { */
  /*   __m64 newa = (__m64)((long long int)*(a + i)); */
  /*   __m64 newb = (__m64)((long long int)*(b + i)); */
  /*   __m64 res = _mm_sad_pu8((__m64)((long long int)*(a + i)), */
  /*                           (__m64)((long long int)*(b + i))); */

  /*   unsigned long long int to_print = (unsigned long long int) res; */

  /*   printf("%llx\n", to_print); */
  /* } */

  /* unsigned long long int to_print = (unsigned long long int) res; */

  /* printf("%llx\n", to_print); */

  unsigned char a[N/2] = {0x00, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  unsigned char b[N] = {0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  runsad((void*)a, N/2, (void*)b, N);

  return 0;

}

void runsad(void* a, int alen, void* b, int blen) {
  __m64* x = (__m64*)a;
  __m64* y = (__m64*)b;
  int xlen = alen/8;
  int ylen = blen/8;

  long long int sad = 0;

  int j;
  for (j = 0; j < ylen - xlen + 1; ++j) {

    int i;
    for (i = 0; i < xlen; ++i) {
      __m64 res = _mm_sad_pu8(x[i], y[i+j]);

      unsigned long long int to_print = (unsigned long long int) res;

      printf("%d,%d : %llx\n", j, i, to_print);

      sad += to_print;
    }

    printf("starting at %d, total : %llx\n", j, sad);
    sad = 0;
  }
}
