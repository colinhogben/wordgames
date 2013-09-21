/* eliminate each character from a word in turn */
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
  char *str=argv[1], *s;
  int n=strlen(str), i, c, t;

  for (i=0,s=str,c=*s++; i<n; ) {
    puts(str+1);
    while (i<n && (i++, (t=*s++)==c)) ;
    s[-1] = c;
    c = t;
  }
  return 0;
}
