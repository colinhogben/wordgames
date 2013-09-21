/*--------------------------------------------------------------*
 *	Find words of the form <arg><palindrome>		*
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
  int hlen, len, i;
  int ok;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s head\n", argv[0]);
    exit(1);
  }
  hlen = strlen(argv[1]);
  for (i=0; i<hlen; i++) {
    if (! isalpha(argv[1][i])) {
      fprintf(stderr, "Non-alphabetic character in head\n");
      exit(1);
    }
    argv[1][i] = toupper(argv[1][i]);
  }

  for (len=2; hlen+len<=MAXLEN; len++) {
    Dscan ds = Dscan_open(d, hlen+len);
    while ((word = Dscan_read(ds)) != NULL) {
      ok = 1;
      for (i=0; i<hlen; i++) {
      	if (word[i] != argv[1][i]) {
      	  ok = 0;
      	  Dscan_skip(ds, i);
      	  break;
      	}
      }
      if (ok) {
        for (i=0; i<len/2; i++) {
          if (word[hlen+i] != word[hlen+len-1-i]) {
            ok = 0;
            break;
          }
        }
        if (ok) {
          printf("%s\n", word);
        }
      }
    }
    Dscan_close(ds);
  }
  Dict_close(d);
  return 0;
}
 	  
