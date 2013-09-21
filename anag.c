/*--------------------------------------------------------------*
 *	Find anagrams
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
  int i,clen,len,c;
  struct {char nlet[26], nwild;} pool, wpool;
  Dscan ds;

  for (i=0; i<26; i++) {
    pool.nlet[i] = 0;
  }
  pool.nwild = 0;

  if (argc < 2 || argc > 3) {
    fprintf(stderr, "Usage: %s chars [length]\n", argv[0]);
    exit(1);
  }
  clen = strlen(argv[1]);
  for (i=0; i<clen; i++) {
    char c = argv[1][i];
    if (isalpha(c)) {
      pool.nlet[toupper(c)-'A']++;
    }else if (c=='#' || c=='.') {
      pool.nwild++;
    }else{
      fprintf(stderr, "Bad character\n");
      exit(1);
    }
  }
  if (argc == 3) {
    len = atoi(argv[2]);
    if (len > clen) {
      fprintf(stderr, "Length %d but only %d letters given\n", len, clen);
      exit(1);
    }
  } else {
    len = clen;
  }

  ds = Dscan_open(d, len);
  while ((word = Dscan_read(ds)) != NULL) {
    int ok = 1;
    wpool = pool;
    for (i=0; i<len; i++) {
      c = word[i];
      if (wpool.nlet[c-'A'] > 0) {
        wpool.nlet[c-'A']--;
      }else if (wpool.nwild > 0) {
      	wpool.nwild--;
      }else{
      	ok = 0;
      	Dscan_skip(ds, i);
      	break;
      }
    }
    if (ok) {
      printf("%s", word);
      if (len < clen) {
	printf(" ");
	for (c='A'; c<='Z'; c++) {
	  for (i=0; i<wpool.nlet[c-'A']; i++) {
	    printf("%c", c);
	  }
	}
	for (i=0; i<wpool.nwild; i++) {
	  printf(".");
	}
      }
      printf("\n");
    }
  }
  Dscan_close(ds);
  Dict_close(d);
  exit(0);
}
