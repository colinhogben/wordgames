/*--------------------------------------------------------------*
 *	Find words which are another word after substituting one
 *	subsequence for another
 *--------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "d2.h"

int main(int argc, char *argv[])
{
  Dict d = Dict_open(NULL);
  const char *word;
  char *seq1, *seq2, new[MAXLEN+1];
  int i, len1, len2, len, minlen, maxlen;

  if (argc != 3 && argc != 4) {
    fprintf(stderr, "Usage: %s seq1 seq2 [length]\n", argv[0]);
    exit(1);
  }
  len1 = strlen(seq1 = argv[1]);
  for (i=0; i<len1; i++) {
    if (! isalpha(seq1[i])) {
      fprintf(stderr, "Non-alphabetic character first sequence\n");
      exit(1);
    }
    seq1[i] = toupper(seq1[i]);
  }
  len2 = strlen(seq2 = argv[2]);
  for (i=0; i<len2; i++) {
    if (! isalpha(seq2[i])) {
      fprintf(stderr, "Non-alphabetic character first sequence\n");
      exit(1);
    }
    seq2[i] = toupper(seq2[i]);
  }
  if (argc == 4) {
    minlen = maxlen = atoi(argv[3]);
    if (minlen < len1+1 || minlen > MAXLEN || minlen+len2-len1 > MAXLEN) {
      fprintf(stderr, "Invalid length %d\n", minlen);
      exit(1);
    }
  } else {
    minlen = len1+1;
    maxlen = MAXLEN;
  }
  if (minlen < 2) minlen = 2;

  for (len=minlen; len<=maxlen && len-len1+len2<=MAXLEN; len++) {
    Dscan ds = Dscan_open(d, len);
    while ((word = Dscan_read(ds)) != NULL) {
      for (i=0; i < len-len1; i++) {
	if (word[i]==seq1[0] && strncmp(word+i, seq1, len1)==0) {
	  strncpy(new, word, i);
	  strncpy(new+i, seq2, len2);
	  strcpy(new+i+len2, word+i+len1);
	  if (Dict_contains(d, new, len-len1+len2)) {
	    printf("%s %s\n", word, new);
	  }
	}
      }
    }
    Dscan_close(ds);
  }
  Dict_close(d);
  return 0;
}
 	  
