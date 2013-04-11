#include<xmmintrin.h>
#include<stdio.h>
#include<read.h>

#define WIDTH1 (35 * 4)
#define HEIGHT1 (44)

#define WIDTH2 (1280 * 4)
#define HEIGHT2 (960)

int * runsad(char* img1, int width1, int height1,
             char* img2, int width2, int height2);
int inner_sad(char* pattern, char* source, int w, int h, int source_w);

int main() {
  char * a;
  char * b;
  int success;
  success = load_file("black.raw", &a);
  success *= load_file("bb0001.raw", &b);
  // success *= load_file("black.raw", &b);
  if (!success) {
    return 1;
  }

  int * res = runsad(a, WIDTH1, HEIGHT1,
                     b, WIDTH2, HEIGHT2);

  success = write_file("sad.raw", res, (WIDTH2 - WIDTH1 + 1)*(HEIGHT2 - HEIGHT1 + 1));
  if (!success) {
    return 1;
  }

  free(a);
  free(b);

  return 0;

}

int inner_sad(char* pattern, char* source, int w, int h, int source_w) {
  int words = w/8;

  int sad = 0;

  int y;
  for (y = 0; y < h; ++y) {

    __m64* wpattern = (__m64*) (pattern + w*y);
    __m64* wsource = (__m64*) (source + source_w*y);

    int word;
    for(word = 0; word < words; ++word) {

      __m64 res = _mm_sad_pu8(wpattern[word], wsource[word]);

      /* the max diff of a byte is 255 (2^8 - 1),
       * there are 8 (2^3) bytes, so the total max diff
       * is 8 * 255 or < 2^11, which fits in a 32 bit int */

      sad += ((long long int) res) & 0xffff;
    }

    /* if we have 20 pixels, we'll do 16 the quick way and four this way */
    int extra;
    for(extra = words * 8; extra < w; ++extra) {
      char temp = pattern[w*y + extra] - source[source_w*y + extra];
      if(temp < 0)
        temp = -temp;
      sad += temp;
    }
  }

  return sad;
}

int * runsad(char* img1, int width1, int height1,
             char* img2, int width2, int height2) {

  int * sad
    = malloc(sizeof(long long int) *
             ((width2 - width1 + 1)/4*(height2 - height1 + 1)));


  /* the top left y-coord of the pattern within the source */
  int tly;
  for(tly = 0; tly < (height2 - height1 + 1); ++tly) {
    /* the top left x-coord of the pattern within the source */
    int tlx;
    for (tlx = 0; tlx < (width2 - width1 + 1); tlx += 4) {
      int tl_index = (tly * width2) + tlx;

      sad[tly * (width2-width1+1)/4 + tlx/4]
        += inner_sad(  img1
                     , (img2 + tl_index)
                     , width1
                     , height1
                     , width2);
    }
  }

  return sad;
}
