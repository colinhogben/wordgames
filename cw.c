/*======================================================================
 *	Doodlings to do with Scrabble(R)
 *======================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif
#include "dict.h"

char *progname;

/* functions to interface with the dictionary */
int is_word(const char *word, int len)
{
  char w[MAXLEN+1];
  strncpy(w, word, len);
  w[len] = '\0';
  return check_word(w);
}

static DictCtl dictctl;
void init_word(char *wbuf, int len)
{
  INITWORD(dictctl,len);
}
int get_word(char *wbuf, int pos)
{
  return NEXTWORD(dictctl,wbuf,pos);
}
