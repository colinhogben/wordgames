/*--------------------------------------------------------------*
 *	Compare two dictionaries
 *--------------------------------------------------------------*/
#include "d2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
  
int main(int argc,
         char *argv[])
{
  Dict d1, d2;
  const char *word;
  int len;

  if (argc != 3) {
    fprintf(stderr, "Usage: %s dir1 dir2\n", argv[0]);
    exit(1);
  }

  d1 = Dict_open(argv[1]);
  d2 = Dict_open(argv[2]);
  for (len=2; len<=MAXLEN; len++) {
    Dscan ds = Dscan_open(d1, len);
    while ((word = Dscan_read(ds)) != NULL) {
      if (Dict_isword(d2, word, len)) {
      	;
      }else if (len > 2 && word[len-1]=='S' &&
		Dict_isword(d2, word, len-1)) {
	;
      }else{
      	printf("%s\n", word);
      }
    }
    Dscan_close(ds);
  }
  Dict_close(d2);
  Dict_close(d1);
  return 0;
}
