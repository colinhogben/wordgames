/*======================================================================
 *	Test d2 dictionary
 *======================================================================*/
#include "d2.h"
#include <stdio.h>

int main(void)
{
  Dict d = Dict_open(NULL);
  int i;
  for (i=2; i<=MAXLEN; i++) {
    Dscan ds = Dscan_open(d, i);
    const char *w;
    while ((w = Dscan_read(ds)) != NULL) {
      printf("%s\n", w);
    }
    Dscan_close(ds);
  }
  Dict_close(d);
  return 0;
}
