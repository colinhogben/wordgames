/*--------------------------------------------------------------*
 *	Find words of the form <arg><word>			*
 *--------------------------------------------------------------*/
#include "d2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
  Dict d = Dict_open(NULL);
  const char *word;
  int hlen, len, minlen, maxlen, i, ok;

  if (argc != 2 &&argc != 3) {
    fprintf(stderr, "Usage: %s head [length]\n", argv[0]);
    exit(1);
  }
  hlen = strlen(argv[1]);
  for (i=0; i<hlen; i++) {
    if (! isalpha(argv[1][i])) {
      fprintf(stderr, "Non-alphabetic character in head\n");
      exit(1);
    }
    argv[1][i] = toupper(argv[1][i]);
  }
  if (argc == 3) {
    minlen = maxlen = atoi(argv[2]);
    if (minlen < hlen+1 || minlen > MAXLEN) {
      fprintf(stderr, "Invalid length %d\n", minlen);
      exit(1);
    }
  } else {
    minlen = hlen-1;
    maxlen = MAXLEN;
  }
  if (minlen < 2) minlen = 2;

  for (len=minlen; len<=maxlen; len++) {
    Dscan ds = Dscan_open(d, len);
    while ((word = Dscan_read(ds)) != NULL) {
      ok = 1;
      for (i=0; i<hlen; i++) {
      	if (word[i] != argv[1][i]) {
      	  ok = 0;
      	  Dscan_skip(ds, i);
      	  break;
      	}
      }
      if (ok && Dict_isword(d, word+hlen, len-hlen)) {
        printf("%s\n", word);
      }
    }
    Dscan_close(ds);
  }
  Dict_close(d);
  return 0;
}
 	  
