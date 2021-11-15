/*--------------------------------------------------------------*
 *	Find embedded anagram plus one spare character
 *	E.g. FLYINGCIRCUS contains RING + C
 *--------------------------------------------------------------*/
#include "d2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc,
         char *argv[])
{
  Dict d = Dict_open(NULL);
  const char *word;
  int i,j,len,tlen,c;
  Dpool *pool, wpool;
  Dscan ds;

  if (argc != 3) {
    fprintf(stderr, "Usage: %s length chars\n", argv[0]);
    exit(1);
  }
  len = atoi(argv[1]);
  tlen = strlen(argv[2]);
  if (len > tlen-1) {
    fprintf(stderr, "Bad length\n");
    exit(1);
  }

  /* Create pools for each subword of length n+1
   * e.g. FLYIN, LYING, YINGC, INGCI, NGCIR, GCIRC, CIRCU, IRCUS
   */
  pool = calloc(tlen-len-1, sizeof(Dpool));
  if (pool == NULL) {fprintf(stderr, "Out of memory\n"); exit(1);}
  for (j=0; j<tlen-len-1; j++) {
    dpool_clear(&pool[j]);

    for (i=0; i<len+1; i++) {
      char c = argv[2][j+i];
      if (isalpha(c)) {
	dpool_add_char(&pool[j], c);
      }else{
        fprintf(stderr, "Bad character\n");
        exit(1);
      }
    }
  }

  ds = Dscan_open(d, len);
  while ((word = Dscan_read(ds)) != NULL) {
    int maxskip = 0;
    for (j=0; j<tlen-len-1; j++) {
      int ok = 1;
      wpool = pool[j];
      for (i=0; i<len; i++) {
        c = word[i];
	if (dpool_sub_char(&wpool, c) < 0) {
	  ok = 0;
	  if (i > maxskip) maxskip = i;
      	  break;
        }
      }
      if (ok) {
        /* Find extra character in jumble */
	char rest[MAXLEN];
	dpool_string(rest, &wpool, '.');
        printf("%s %s\n", word, rest);
        maxskip = len;
      }
    }
    Dscan_skip(ds, maxskip);
  }
  Dscan_close(ds);
  Dict_close(d);
  exit(0);
}
