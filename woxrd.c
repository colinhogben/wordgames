/*--------------------------------------------------------------*
 *	Find words of the form <start of word><arg><rest of word>*
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
  int i, tlen, len, minlen, maxlen;

  if (argc != 2 && argc != 3) {
    fprintf(stderr, "Usage: %s middle [length]\n", argv[0]);
    exit(1);
  }
  tlen = strlen(argv[1]);
  for (i=0; i<tlen; i++) {
    if (! isalpha(argv[1][i])) {
      fprintf(stderr, "Non-alphabetic character in middle\n");
      exit(1);
    }
    argv[1][i] = toupper(argv[1][i]);
  }
  if (argc == 3) {
    minlen = maxlen = atoi(argv[2]);
    if (minlen < tlen+2 || minlen > MAXLEN) {
      fprintf(stderr, "Invalid length %d\n", minlen);
      exit(1);
    }
  } else {
    minlen = tlen+2;
    maxlen = MAXLEN;
  }

  for (len=minlen; len<=maxlen; len++) {
    Dscan ds = Dscan_open(d, len);
    new[len-tlen] = '\0';
    while ((word = Dscan_read(ds)) != NULL) {
      int pos;
      for (pos=1; pos<=len-tlen-1; pos++) {
      	if (strncmp(word+pos, argv[1], tlen) == 0) { /* Contains 'x' */
      	  strncpy(new, word, pos);
      	  strncpy(new+pos, word+pos+tlen, len-tlen-pos); /* Word with 'x' removed */
	  if (Dict_isword(d, new, len-tlen)) {
            printf("%s\n", word);
          }
        }
      }
    }
    Dscan_close(ds);
  }
  Dict_close(d);
  return 0;
}

