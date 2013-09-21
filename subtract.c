/***************************************************************************
 *      Find words which make other words when a given string is subtracted *
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
  int len;
  char str[32];
  const char *s;
  int i;
  int slen, len1;
  if (argc != 2) {
    fprintf(stderr, "usage: %s string\n", argv[0]);
    exit(1);
  }else if ((slen = strlen(argv[1])) > 20) {
    fprintf(stderr, "String too long\n");
    exit(1);
  }
  strcpy(str, argv[1]);
  for (i=0; i<slen; i++) {
    if (! isalpha(str[i])) {
      fprintf(stderr, "Non-alphabetic character in string\n");
      exit(1);
    }
    str[i] = toupper(str[i]);
  }
  len1 = slen+2;
  if (len1 < 3) len1 = 3;
  
  for (len=len1; len<=MAXLEN; len++) {
    Dscan ds = Dscan_open(d, len);
    while ((word = Dscan_read(ds)) != NULL) {
      if ((s = strstr(word, str)) != NULL) {
        strcpy(new, word);
        strcpy(new+(s-word), s+slen);
        if (Dict_isword(d, new, len-slen)) {
          printf("%s %s\n", word, new);
        }
      }
    }
    Dscan_close(ds);
  }
  Dict_close(d);
  return 0;
}
 	  
