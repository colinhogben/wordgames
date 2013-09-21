/*--------------------------------------------------------------*
 *	Find words whose even letters spell a word		*
 *--------------------------------------------------------------*/
#include "d2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
  Dict d = Dict_open(NULL);
  const char *word;
  char new[MAXLEN+1];
  int len, hlen, i;

  for (len=4; len<=MAXLEN; len++) {
    Dscan ds = Dscan_open(d, len);
    hlen = len/2;
    new[hlen] = '\0';
    while ((word = Dscan_read(ds)) != NULL) {
      for (i=0; i<hlen; i++) {
        new[i] = word[2*i+1];
      }
      if (Dict_isword(d, new, hlen)) {
        printf("%s \t%s\n", word, new);
      }
    }
    Dscan_close(ds);
  }
  Dict_close(d);
  exit(0);
}
 	  
