/***************************************************************************
 *      Find words made only from the given letters			   *
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
  int allow[26];
  int len;
  const char *s;
  int i;
  int ok;

  if (argc != 2) {
    fprintf(stderr, "usage: %s string\n", argv[0]);
    exit(1);
  }
  for (i=0; i<26; i++) {
    allow[i] = 0;
  }
  s = argv[1];
  for (i=0; i<strlen(s); i++) {
    if (! isalpha(s[i])) {
      fprintf(stderr, "Non-alphabetic character in string\n");
      exit(1);
    }
    allow[toupper(s[i])-'A'] = 1;
  }
  
  for (len=2; len<=MAXLEN; len++) {
    Dscan ds = Dscan_open(d, len);
    while ((word = Dscan_read(ds)) != NULL) {
      ok = 1;
      for (i=0; i<len; i++) {
        if (! allow[word[i]-'A']) {
          ok = 0;
          Dscan_skip(ds, i);
          break;
        }
      }
      if (ok) {
        printf("%s\n", word);
      }
    }
    Dscan_close(ds);
  }
  Dict_close(d);
  return 0;
}
 	  
