/***************************************************************************
 *      Find words which make other words when Caesar-rotated              *
 ***************************************************************************/

#include "d2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
                                                      
int main(int argc, char **argv)
{
  Dict d = Dict_open(NULL);
  const char *word;
  char new[32];
  int len, i;
  int rot;

  if (argc > 1) {
    rot = atoi(argv[1]);
  }else{
    rot = 13;
  }
  for (len=2; len<=MAXLEN; len++) {
    Dscan ds = Dscan_open(d, len);
    new[len] = '\0';
    while ((word = Dscan_read(ds)) != NULL) {
      if (rot == 13 && word[0] > 'M') break;   /* ignore N..Z (symmetric) */
      for (i=0; i<len; i++) {
        new[i] = word[i]+rot;
        if (new[i] > 'Z') new[i]-=26;
      }
      if (Dict_contains(d, new, len)) {
        printf("%s %s\n", word, new);
      }
    }
    Dscan_close(ds);
  }
  Dict_close(d);
  exit(0);
}
