/***************************************************************************
 *      Find words made from the given letters, maybe repeated		   *
 *	E.g. in The Times, Mindset puzzle of 2021-11-13
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
  
  for (len=strlen(s); len<=MAXLEN; len++) {
    Dscan ds = Dscan_open(d, len);
    int unused[26];
    while ((word = Dscan_read(ds)) != NULL) {
      int used = 0;
      memcpy(unused, allow, sizeof(unused));
      ok = 1;
      for (i=0; i<len; i++) {
        if (! allow[word[i]-'A']) {
          ok = 0;
          Dscan_skip(ds, i);
          break;
        }
	if (unused[word[i]-'A']) {
	  used ++;
	  unused[word[i]-'A'] = 0;
	}
      }
      if (ok && used == strlen(s)) {
        printf("%s\n", word);
      }
    }
    Dscan_close(ds);
  }
  Dict_close(d);
  return 0;
}
 	  
