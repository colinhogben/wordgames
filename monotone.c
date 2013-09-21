/*--------------------------------------------------------------*
 *	Find words whose letters are in alphabetic order
 *--------------------------------------------------------------*/

#include "d2.h"
#include <stdio.h>
#include <string.h>
                                                      
int main(int argc, char **argv)
{
  Dict d = Dict_open(NULL);
  const char *word;
  int len, i;
  
  if (argc > 1) {                       /* increasing */
    for (len=3; len<=MAXLEN; len++) {
      Dscan ds = Dscan_open(d, len);
      while ((word = Dscan_read(ds)) != NULL) {
      	for (i=1; i<len; i++) {
      	  if (word[i] > word[i-1]) {
	    Dscan_skip(ds, i);
	    goto nextd;
	  }
	}
        printf("%s\n", word);
nextd:  ;
      }
      Dscan_close(ds);
    }
  }else{                                /* decreasing */
    for (len=3; len<=MAXLEN; len++) {
      Dscan ds = Dscan_open(d, len);
      while ((word = Dscan_read(ds)) != NULL) {
      	for (i=1; i<len; i++) {
      	  if (word[i] < word[i-1]) {
	    Dscan_skip(ds, i);
	    goto nexti;
	  }
	}
        printf("%s\n", word);
nexti:  ;
      }
      Dscan_close(ds);
    }
  }
  return 0;
}
 	  
