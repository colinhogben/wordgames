/***************************************************************************
 *      Find words which make other words when rotated                     *
 ***************************************************************************/

#include "d2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
                                                      
int main(int argc, char *argv[])
{
  Dict d = Dict_open(NULL);
  const char *word;
  char new[32];
  int len, i;
  int dist;
  if (argc==1) {
    dist = 1;
  }else if (argc > 2) {
    fprintf(stderr, "usage: %s [count]\n", argv[0]);
    exit(1);
  }else{
    dist = atoi(argv[1]);
    if (dist < 1) {
      fprintf(stderr, "Bad count\n");
      exit(1);
    }
  }

  for (len=(2*dist>3? 2*dist: 3); len<=MAXLEN; len++) {
    Dscan ds = Dscan_open(d, len);
    new[len] = '\0';
    while ((word = Dscan_read(ds)) != NULL) {
      for (i=0; i<len-dist; i++) {
        new[i] = word[i+dist];
      }
      for (i=len-dist; i<len; i++) {
        new[i] = word[i+dist-len];
      }
      if (Dict_isword(d, new, len)) {
        printf("%s %s\n", word, new);
      }
    }
    Dscan_close(ds);
  }
  Dict_close(d);
  return 0;
}
 	  
