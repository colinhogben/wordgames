/*--------------------------------------------------------------*
 *	Count words/chars in dict				*
 *--------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
  char last[80] = "";
  char line[80];
  int lines=0, chars=0, match=0;
  int i;
  
  while (gets(line)) {
    for (i=0; ; i++) {
      if (line[i] != last[i]) break;    /* mismatch found */
    }
    lines += 1;
    chars += strlen(line)+1;
    match += i;
    strcpy(last, line);
  }
  printf("%d lines, %d chars, %d matched chars\n", lines, chars, match);
  exit(0);
}

