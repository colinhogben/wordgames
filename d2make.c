/*--------------------------------------------------------------*
 *	Make a dictionary from a file of words			*
 *	maked2 [-u] <len> <wordfile> <dictfile>			*
 *--------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "d2write.h"

/*--------------------------------------------------------------*
 *	main program entry point
 *--------------------------------------------------------------*/
int main(int argc, char *argv[])
{
  char line[256], w[256], *progname;
  int upper = 0;
  int len, i, wl, anylow;
  char c;
  Dictw dw;

  progname = argv[0];
  if (argc > 1 && strcmp(argv[1], "-u")==0) {
    upper = 1;
    argv++;
    argc--;
  }
  if (argc != 4) {
    fprintf(stderr, "Usage: %s [-u] len wordfile dictfile\n", progname);
    exit(1);
  }

  len = atoi(argv[1]);
  if (len < 1 || len > 100) {
    fprintf(stderr, "Invalid length %s\n", argv[1]);
    exit(1);
  }else if (! freopen(argv[2], "r", stdin)) {
    fprintf(stderr, "Cannot open %s\n", argv[2]);
    exit(1);
  }
  dw = Dictw_create(len);
  
  while (fgets(line, sizeof(line), stdin) != NULL) {
    if (strlen(line) < len) continue;	/* not long enough */
    wl = 0;
    anylow = 0;
    for (i=0; (c=line[i]) != 0; i++) {
      if (isalpha(c)) {
      	if (islower(c)) anylow = 1;	/* a lower case char */
      	w[wl++] = toupper(c);
      	if (wl > len) break;		/* too long */
      }else if (strchr("\n-'\373", c) == NULL) {
      	wl = 0;				/* funny character - ignore */
      	break;
      }
    }
    if (wl == len && (anylow || !upper)) { /* length ok, not acronym */
      Dictw_add(dw, w);
    }
  }

  Dictw_write(dw, argv[3]);
#ifdef __riscos
  sprintf(line, "SetType %s D2dict", argv[3]);
  system(line);
#endif

  return 0; 
}
