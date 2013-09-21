/*--------------------------------------------------------------*
 *	Find embedded anagram plus one spare character
 *--------------------------------------------------------------*/
#include "d2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {unsigned char nlet[26], nwild;} CPool;

int main(int argc,
         char *argv[])
{
  Dict d = Dict_open(NULL);
  const char *word;
  int i,j,len,tlen,c;
  CPool *pool, wpool;
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

  pool = calloc(tlen-len-1, sizeof(CPool));
  if (pool == NULL) {fprintf(stderr, "Out of memory\n"); exit(1);}
  for (j=0; j<tlen-len-1; j++) {
    for (i=0; i<26; i++) {
      pool[j].nlet[i] = 0;
    }
    pool[j].nwild = 0;

    for (i=0; i<len+1; i++) {
      char c = argv[2][j+i];
      if (isalpha(c)) {
        pool[j].nlet[toupper(c)-'A']++;
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
        if (wpool.nlet[c-'A'] > 0) {
          wpool.nlet[c-'A']--;
        }else{
	  ok = 0;
	  if (i > maxskip) maxskip = i;
      	  break;
        }
      }
      if (ok) {
        /* Find extra character in jumble */
        for (c=0; c<26; c++) {
          if (wpool.nlet[c] != 0) break;
        }
        printf("%s %c\n", word, c+'A');
        maxskip = len;
      }
    }
    Dscan_skip(ds, maxskip);
  }
  Dscan_close(ds);
  Dict_close(d);
  exit(0);
}
