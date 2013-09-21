/*--------------------------------------------------------------*
 *	Find palindromes					*
 *--------------------------------------------------------------*/
#include "d2.h"
#include <stdio.h>
#include <string.h>
                                                      
int main(void)
{
  Dict d = Dict_open(NULL);
  const char *word;
  int len, i;
  int palin;

  for (len=3; len<=MAXLEN; len++) {
    Dscan ds = Dscan_open(d, len);
    while ((word = Dscan_read(ds)) != NULL) {
      palin = 1;
      for (i=0; i<len/2; i++) {
        if (word[i] != word[len-1-i]) {
          palin = 0;
          break;
        }
      }
      if (palin) printf("%s\n", word);
    }
    Dscan_close(ds);
  }
  Dict_close(d);
  return 0;
}
 	  
