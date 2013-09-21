/*--------------------------------------------------------------*
 *	Find words also word when quarter reversed		*
 *--------------------------------------------------------------*/
#include "d2.h"
#include <stdio.h>
#include <string.h>
                                                      
int main(void)
{
  char qb[MAXLEN+1];
  Dict d = Dict_open(NULL);
  const char *word;
  int len, i, q;

  for (len=8; len<=MAXLEN; len+=4) {
    int l4 = len/4;
    Dscan ds = Dscan_open(d, len);
    while ((word = Dscan_read(ds)) != NULL) {
      for (q=0; q<4; q++) {
	strcpy(qb, word);
	for (i=0; i<l4; i++) {
	  qb[q*l4 + i] = word[q*l4 + l4-i-1];
	}
	if (strcmp(qb, word) != 0) { /* Ignore if trivial */
	  if (Dict_isword(d, qb, len)) {
	    printf("%s %s\n", word, qb);
	  }
	}
      }
    }
    Dscan_close(ds);
  }
  Dict_close(d);
  return 0;
}
