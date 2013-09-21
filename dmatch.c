/*--------------------------------------------------------------*
 *	Find words matching pattern such as "p#tt*n"		*
 *--------------------------------------------------------------*/
#include "d2.h"
#include "match.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
  
int main(int argc,
         char *argv[])
{
  Dict d = Dict_open(NULL);
  const char *word;
  int i,len;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s pattern\n", argv[0]);
    exit(1);
  }
  for (i=0; argv[1][i]; i++) argv[1][i] = toupper(argv[1][i]);
  init_match(argv[1]);
  for (len=(match_min<1? 1: match_min); len<=match_max; len++) {
    Dscan ds = Dscan_open(d, len);
    while ((word = Dscan_read(ds)) != NULL) {
      if (match_word(word)) {
        printf("%s\n", word);
      }
    }
    Dscan_close(ds);
  }
  Dict_close(d);
  exit(0);
}
 	  
