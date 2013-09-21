/*--------------------------------------------------------------*
 *	Find words of the form <word><arg>			*
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
  int i, tlen, len, minlen, maxlen;

  if (argc != 2 && argc != 3) {
    fprintf(stderr, "Usage: %s tail [length]\n", argv[0]);
    exit(1);
  }
  tlen = strlen(argv[1]);
  for (i=0; i<tlen; i++) {
    if (! isalpha(argv[1][i])) {
      fprintf(stderr, "Non-alphabetic character in tail\n");
      exit(1);
    }
    argv[1][i] = toupper(argv[1][i]);
  }
  if (argc == 3) {
    minlen = maxlen = atoi(argv[2]);
    if (minlen < tlen+1 || minlen > MAXLEN) {
      fprintf(stderr, "Invalid length %d\n", minlen);
      exit(1);
    }
  } else {
    minlen = tlen+2;
    maxlen = MAXLEN;
  }
  if (minlen < 2) minlen = 2;

  for (len=minlen; len<=maxlen; len++) {
    Dscan ds = Dscan_open(d, len);
    while ((word = Dscan_read(ds)) != NULL) {
      if (strcmp(word+len-tlen, argv[1])==0) {
        if (Dict_isword(d, word, len-tlen)) {
          printf("%s\n", word);
        }
      }
    }
    Dscan_close(ds);
  }
  Dict_close(d);
  return 0;
}
 	  
