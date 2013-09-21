/*--------------------------------------------------------------*
 *	Find words of the form <palindrome><arg>		*
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
  int tlen, len, i;
  int palin;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s tail\n", argv[0]);
    exit(1);
  }
  tlen = strlen(argv[1]);
  for (i=0; i<tlen; i++) {
    if (! isalpha(argv[1][i])) {
      fprintf(stderr, "Non-alphabetic character in tead\n");
      exit(1);
    }
    argv[1][i] = toupper(argv[1][i]);
  }
  
  for (len=2; len+tlen<=MAXLEN; len++) {
    Dscan ds = Dscan_open(d, len+tlen);
    while ((word = Dscan_read(ds)) != NULL) {
      if (strcmp(word+len, argv[1])==0) {
        palin = 1;
        for (i=0; i<len/2; i++) {
          if (word[i] != word[len-1-i]) {
            palin = 0;
            break;
          }
        }
        if (palin) {
          printf("%s\n", word);
        }
      }
    }
    Dscan_close(ds);
  }
  Dict_close(d);
  return 0;
}
 	  
