/*--------------------------------------------------------------*
 *	Find words of the form <reversed word><arg>		*
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
  char new[MAXLEN+1];
  int tlen, len, i;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s tail\n", argv[0]);
    exit(1);
  }
  tlen = strlen(argv[1]);
  
  for (len=2; len+tlen<=MAXLEN; len++) {
    Dscan ds = Dscan_open(d, len+tlen);
    new[len] = '\0';
    while ((word = Dscan_read(ds)) != NULL) {
      if (strcmp(word+len, argv[1])==0) {
        for (i=0; i<len; i++) {
          new[i] = word[len-1-i];
        }
        if (Dict_isword(d, new, len)) {
          printf("%s\n", word);
        }
      }
    }
    Dscan_close(ds);
  }
  Dict_close(d);
  return 0;
}
 	  
