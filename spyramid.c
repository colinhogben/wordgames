/*--------------------------------------------------------------*
 *	Find sorted pyramid words
 *--------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "d2.h"

char *csort(char *s, int len)
{
  int i, j, k, c;
  for (i=0; i<len; i++) {
    c = s[k=i];
    for (j=i+1; j<len; j++)
      if (s[j] < c)
	c = s[k=j];
    s[k] = s[i];
    s[i] = c;
  }
  return s;
}

int main(int argc, char **argv)
{
  int len;
  Dict d, pd;
  Dictl pdl;
  Dscan ds;
  const char *word;
  char elim[MAXLEN+1], pyr[MAXLEN+1], *s;
  int i, c, t;

  if (argc != 2 ||
      (len = atoi(argv[1]), len < 2 || len>20)) {
    fprintf(stderr, "Usage: %s len\n", argv[0]);
    exit(1);
  }

  d = Dict_open(NULL);
  pd = Dict_open("Words:Spyr");
  pdl = Dictl_open(pd, len-1);
  for (ds=Dscan_open(d, len);
       (word = Dscan_read(ds)) != NULL; ) {
    int ok = 0;
    strcpy(elim, csort(strcpy(pyr, word), len));

    for (i=0,s=elim,c=*s++; i<len; ) {
      if (Dictl_contains(pdl, elim+1)) {
        ok = 1;
        pyr[i] = tolower(pyr[i]);
      }
      while (i<len && (i++, (t=*s++)==c)) ;
      s[-1] = c;
      c = t;
    }
    if (ok) puts(pyr);
  }
  return 0;
}
