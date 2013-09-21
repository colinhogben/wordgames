/*--------------------------------------------------------------*
 *	Find words of four or more letters including the main
 *--------------------------------------------------------------*/
#include "d2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc,
         char *argv[])
{
  Dict d = Dict_open(NULL);
  const char *word;
  char cmain;
  int i,clen,len,c;
  struct {char nlet[26];} pool, wpool;
  Dscan ds;

  for (i=0; i<26; i++) {
    pool.nlet[i] = 0;
  }

  if (argc != 2) {
    fprintf(stderr, "Usage: %s chars\n", argv[0]);
    exit(1);
  }
  clen = strlen(argv[1]);
  cmain = toupper(argv[1][0]);
  for (i=0; i<clen; i++) {
    char c = argv[1][i];
    if (isalpha(c)) {
      pool.nlet[toupper(c)-'A']++;
    }else{
      fprintf(stderr, "Bad character\n");
      exit(1);
    }
  }

  for (len=4; len <= clen; len++) {
    ds = Dscan_open(d, len);
    while ((word = Dscan_read(ds)) != NULL) {
      if (strchr(word, cmain) != NULL) {
	int ok = 1;
	wpool = pool;
	for (i=0; i<len; i++) {
	  c = word[i];
	  if (wpool.nlet[c-'A'] > 0) {
	    wpool.nlet[c-'A']--;
	  }else{
	    ok = 0;
	    Dscan_skip(ds, i);
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
  exit(0);
}
