/*--------------------------------------------------------------*
 *	Find words of four or more letters including the main
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
  char cmain;
  int i,clen,len,c;
  Dpool pool, wpool;
  Dscan ds;

  dpool_clear(&pool);

  if (argc != 2) {
    fprintf(stderr, "Usage: %s chars\n", argv[0]);
    exit(1);
  }
  clen = strlen(argv[1]);
  cmain = toupper(argv[1][0]);
  for (i=0; i<clen; i++) {
    char c = argv[1][i];
    if (isalpha(c)) {
      dpool_add_char(&pool, c);
    }else{
      fprintf(stderr, "Bad character\n");
      exit(1);
    }
  }

  for (len=4; len <= clen; len++) {
    ds = Dscan_open(d, len);
    while ((word = Dscan_read(ds)) != NULL) {
      if (strchr(word, cmain) != NULL) {
	int ok = 1;
	wpool = pool;
	for (i=0; i<len; i++) {
	  c = word[i];
	  if (dpool_sub_char(&wpool, c) < 0) {
	    ok = 0;
	    Dscan_skip(ds, i);
	    break;
	  }
	}
	if (ok) {
	  printf("%s\n", word);
	}
      }
    }
    Dscan_close(ds);
  }
  Dict_close(d);
  exit(0);
}
