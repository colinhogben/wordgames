#include <stdio.h>
#include <stdlib.h>

static int cmp0(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;

  if ((diff = sa[0] - sb[0]) != 0) return diff;
  if ((diff = sa[1] - sb[1]) != 0) return diff;
  if ((diff = sa[2] - sb[2]) != 0) return diff;
  if ((diff = sa[3] - sb[3]) != 0) return diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;

  return 0;
}

static int cmp1(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[0] - sb[0]) != 0) return diff;

  if ((diff = sa[1] - sb[1]) != 0) return diff;
  if ((diff = sa[2] - sb[2]) != 0) return diff;
  if ((diff = sa[3] - sb[3]) != 0) return diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;

  return 0;
}

static int cmp2(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[1] - sb[1]) != 0) return diff;

  if ((diff = sa[0] - sb[0]) != 0) return diff;
  if ((diff = sa[2] - sb[2]) != 0) return diff;
  if ((diff = sa[3] - sb[3]) != 0) return diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;

  return 0;
}

static int cmp3(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[0] - sb[0]) != 0) return diff;
  if ((diff = sa[1] - sb[1]) != 0) return diff;

  if ((diff = sa[2] - sb[2]) != 0) return diff;
  if ((diff = sa[3] - sb[3]) != 0) return diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;

  return 0;
}

static int cmp4(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[2] - sb[2]) != 0) return diff;

  if ((diff = sa[0] - sb[0]) != 0) return diff;
  if ((diff = sa[1] - sb[1]) != 0) return diff;
  if ((diff = sa[3] - sb[3]) != 0) return diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;

  return 0;
}

static int cmp5(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[0] - sb[0]) != 0) return diff;
  if ((diff = sa[2] - sb[2]) != 0) return diff;

  if ((diff = sa[1] - sb[1]) != 0) return diff;
  if ((diff = sa[3] - sb[3]) != 0) return diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;

  return 0;
}

static int cmp6(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[1] - sb[1]) != 0) return diff;
  if ((diff = sa[2] - sb[2]) != 0) return diff;

  if ((diff = sa[0] - sb[0]) != 0) return diff;
  if ((diff = sa[3] - sb[3]) != 0) return diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;

  return 0;
}

static int cmp7(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[0] - sb[0]) != 0) return diff;
  if ((diff = sa[1] - sb[1]) != 0) return diff;
  if ((diff = sa[2] - sb[2]) != 0) return diff;

  if ((diff = sa[3] - sb[3]) != 0) return diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;

  return 0;
}

static int cmp8(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[3] - sb[3]) != 0) return diff;

  if ((diff = sa[0] - sb[0]) != 0) return diff;
  if ((diff = sa[1] - sb[1]) != 0) return diff;
  if ((diff = sa[2] - sb[2]) != 0) return diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;

  return 0;
}

static int cmp9(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[0] - sb[0]) != 0) return diff;
  if ((diff = sa[3] - sb[3]) != 0) return diff;

  if ((diff = sa[1] - sb[1]) != 0) return diff;
  if ((diff = sa[2] - sb[2]) != 0) return diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;

  return 0;
}

static int cmp10(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[1] - sb[1]) != 0) return diff;
  if ((diff = sa[3] - sb[3]) != 0) return diff;

  if ((diff = sa[0] - sb[0]) != 0) return diff;
  if ((diff = sa[2] - sb[2]) != 0) return diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;

  return 0;
}

static int cmp11(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[0] - sb[0]) != 0) return diff;
  if ((diff = sa[1] - sb[1]) != 0) return diff;
  if ((diff = sa[3] - sb[3]) != 0) return diff;

  if ((diff = sa[2] - sb[2]) != 0) return diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;

  return 0;
}

static int cmp12(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[2] - sb[2]) != 0) return diff;
  if ((diff = sa[3] - sb[3]) != 0) return diff;

  if ((diff = sa[0] - sb[0]) != 0) return diff;
  if ((diff = sa[1] - sb[1]) != 0) return diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;

  return 0;
}

static int cmp13(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[0] - sb[0]) != 0) return diff;
  if ((diff = sa[2] - sb[2]) != 0) return diff;
  if ((diff = sa[3] - sb[3]) != 0) return diff;

  if ((diff = sa[1] - sb[1]) != 0) return diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;

  return 0;
}

static int cmp14(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[1] - sb[1]) != 0) return diff;
  if ((diff = sa[2] - sb[2]) != 0) return diff;
  if ((diff = sa[3] - sb[3]) != 0) return diff;

  if ((diff = sa[0] - sb[0]) != 0) return diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;

  return 0;
}

static int cmp15(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[0] - sb[0]) != 0) return diff;
  if ((diff = sa[1] - sb[1]) != 0) return diff;
  if ((diff = sa[2] - sb[2]) != 0) return diff;
  if ((diff = sa[3] - sb[3]) != 0) return diff;

  if ((diff = sa[4] - sb[4]) != 0) return diff;

  return 0;
}

static int cmp16(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;

  if ((diff = sa[0] - sb[0]) != 0) return diff;
  if ((diff = sa[1] - sb[1]) != 0) return diff;
  if ((diff = sa[2] - sb[2]) != 0) return diff;
  if ((diff = sa[3] - sb[3]) != 0) return diff;

  return 0;
}

static int cmp17(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[0] - sb[0]) != 0) return diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;

  if ((diff = sa[1] - sb[1]) != 0) return diff;
  if ((diff = sa[2] - sb[2]) != 0) return diff;
  if ((diff = sa[3] - sb[3]) != 0) return diff;

  return 0;
}

static int cmp18(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[1] - sb[1]) != 0) return diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;

  if ((diff = sa[0] - sb[0]) != 0) return diff;
  if ((diff = sa[2] - sb[2]) != 0) return diff;
  if ((diff = sa[3] - sb[3]) != 0) return diff;

  return 0;
}

static int cmp19(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[0] - sb[0]) != 0) return diff;
  if ((diff = sa[1] - sb[1]) != 0) return diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;

  if ((diff = sa[2] - sb[2]) != 0) return diff;
  if ((diff = sa[3] - sb[3]) != 0) return diff;

  return 0;
}

static int cmp20(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[2] - sb[2]) != 0) return diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;

  if ((diff = sa[0] - sb[0]) != 0) return diff;
  if ((diff = sa[1] - sb[1]) != 0) return diff;
  if ((diff = sa[3] - sb[3]) != 0) return diff;

  return 0;
}

static int cmp21(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[0] - sb[0]) != 0) return diff;
  if ((diff = sa[2] - sb[2]) != 0) return diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;

  if ((diff = sa[1] - sb[1]) != 0) return diff;
  if ((diff = sa[3] - sb[3]) != 0) return diff;

  return 0;
}

static int cmp22(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[1] - sb[1]) != 0) return diff;
  if ((diff = sa[2] - sb[2]) != 0) return diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;

  if ((diff = sa[0] - sb[0]) != 0) return diff;
  if ((diff = sa[3] - sb[3]) != 0) return diff;

  return 0;
}

static int cmp23(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[0] - sb[0]) != 0) return diff;
  if ((diff = sa[1] - sb[1]) != 0) return diff;
  if ((diff = sa[2] - sb[2]) != 0) return diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;

  if ((diff = sa[3] - sb[3]) != 0) return diff;

  return 0;
}

static int cmp24(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[3] - sb[3]) != 0) return diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;

  if ((diff = sa[0] - sb[0]) != 0) return diff;
  if ((diff = sa[1] - sb[1]) != 0) return diff;
  if ((diff = sa[2] - sb[2]) != 0) return diff;

  return 0;
}

static int cmp25(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[0] - sb[0]) != 0) return diff;
  if ((diff = sa[3] - sb[3]) != 0) return diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;

  if ((diff = sa[1] - sb[1]) != 0) return diff;
  if ((diff = sa[2] - sb[2]) != 0) return diff;

  return 0;
}

static int cmp26(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[1] - sb[1]) != 0) return diff;
  if ((diff = sa[3] - sb[3]) != 0) return diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;

  if ((diff = sa[0] - sb[0]) != 0) return diff;
  if ((diff = sa[2] - sb[2]) != 0) return diff;

  return 0;
}

static int cmp27(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[0] - sb[0]) != 0) return diff;
  if ((diff = sa[1] - sb[1]) != 0) return diff;
  if ((diff = sa[3] - sb[3]) != 0) return diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;

  if ((diff = sa[2] - sb[2]) != 0) return diff;

  return 0;
}

static int cmp28(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[2] - sb[2]) != 0) return diff;
  if ((diff = sa[3] - sb[3]) != 0) return diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;

  if ((diff = sa[0] - sb[0]) != 0) return diff;
  if ((diff = sa[1] - sb[1]) != 0) return diff;

  return 0;
}

static int cmp29(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[0] - sb[0]) != 0) return diff;
  if ((diff = sa[2] - sb[2]) != 0) return diff;
  if ((diff = sa[3] - sb[3]) != 0) return diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;

  if ((diff = sa[1] - sb[1]) != 0) return diff;

  return 0;
}

static int cmp30(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[1] - sb[1]) != 0) return diff;
  if ((diff = sa[2] - sb[2]) != 0) return diff;
  if ((diff = sa[3] - sb[3]) != 0) return diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;

  if ((diff = sa[0] - sb[0]) != 0) return diff;

  return 0;
}

static int cmp31(const void *a, const void *b)
{
  const char *sa = a;
  const char *sb = b;
  int diff;
  if ((diff = sa[0] - sb[0]) != 0) return diff;
  if ((diff = sa[1] - sb[1]) != 0) return diff;
  if ((diff = sa[2] - sb[2]) != 0) return diff;
  if ((diff = sa[3] - sb[3]) != 0) return diff;
  if ((diff = sa[4] - sb[4]) != 0) return diff;


  return 0;
}

typedef int (*cmp_func)(const void *, const void *);

static cmp_func cmpn[32] = {
  &cmp0,
  &cmp1,
  &cmp2,
  &cmp3,
  &cmp4,
  &cmp5,
  &cmp6,
  &cmp7,
  &cmp8,
  &cmp9,
  &cmp10,
  &cmp11,
  &cmp12,
  &cmp13,
  &cmp14,
  &cmp15,
  &cmp16,
  &cmp17,
  &cmp18,
  &cmp19,
  &cmp20,
  &cmp21,
  &cmp22,
  &cmp23,
  &cmp24,
  &cmp25,
  &cmp26,
  &cmp27,
  &cmp28,
  &cmp29,
  &cmp30,
  &cmp31,
};
#define MAXWORD 100000
static char words[MAXWORD][5];

int main(int argc, char **argv)
{
    int nword, mask, i;
    char line[40];
    for (nword=0; ! feof(stdin); nword++) {
	if (nword == MAXWORD) {
	    fprintf(stderr, "Too many words\n");
	    exit(2);
	}
	fgets(line, 40, stdin);
	memcpy(words[nword], line, 5);
    }
    for (mask=0; mask<32; mask++) {
	char filename[200];
	FILE *f;
	qsort(words, nword, 5, cmpn[mask]);
	sprintf(filename, "shuf%02d_%d", 5, mask);
	if ((f = fopen(filename, "w")) == NULL) {
	    fprintf(stderr, "Error opening %s\n", filename);
	}
	for (i=0; i<nword; i++) {
	    fprintf(f, "%.5s\n", words[i]);
	}
	fclose(f);
    }
    return 0;
}
