/***************************************************************************
 *      Find words which make other words when reversed                    *
 ***************************************************************************/

#include "d2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
                                                      
int main(void)
{
  Dict d = Dict_open(NULL);
  const char *word;
  char new[32];
  int len, i;

  for (len=3; len<=MAXLEN; len++) {
    Dscan ds = Dscan_open(d, len);
    new[len] = '\0';
    while ((word = Dscan_read(ds)) != NULL) {
      for (i=0; i<len; i++) {
        new[i] = word[len-1-i];
      }
      if (Dict_isword(d, new, len)) {
        printf("%s\n", word);
      }
    }
    Dscan_close(ds);
  }
  Dict_close(d);
  return 0;
}
 	  
