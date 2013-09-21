/***************************************************************************
 *      Find sub-words from dictionary                                     *
 *      A126    Add partition finding                                      *
 *      A127    Split into three words                                     *
 ***************************************************************************/
#include "d2.h"
#include <stdlib.h>
#include <stdio.h>                     
#include <string.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0

Dict dict;

/*-------------------------------------------------------------------------*
 *      types used                                                         *
 *-------------------------------------------------------------------------*/
typedef struct {unsigned char count[26];} Pool;
#define Pool_contains(p,c) ((p).count[(c)-'A']>0)
#define Pool_add(p,c) ((p).count[(c)-'A']++)
#define Pool_remove(p,c) ((p).count[(c)-'A']--)
#define Pool_count(p,c) ((p).count[(c)-'A'])
#define Pool_clear(p) {int i; for (i=0; i<26; i++) (p).count[i]=0;}

/*-------------------------------------------------------------------------*
 *      Make a pool out of a word                                          *
 *-------------------------------------------------------------------------*/
int make_pool(const char *word, Pool *p)
{
  int c, n=0;

  Pool_clear(*p);			/* clear the counts */

  while ((c = *word++) != 0) {
    if (isalpha(c)) {
      Pool_add(*p, toupper(c));
      n++;
    }
  }
  return n;
}

/*-------------------------------------------------------------------------*
 *      Process all sub-words of the given length made from the pool       *
 *-------------------------------------------------------------------------*/
void find_subwords(Pool *wordpool, int sublen,
                   void process(const char *sw, Pool *rem))
{
  if (sublen >= 2 && sublen <= MAXLEN) {
    Dscan ds = Dscan_open(dict, sublen);

    /* scan dictionary for each initial pair of letters */
    const char *subword;

    while ((subword = Dscan_read(ds)) != NULL) {
      Pool remd = *wordpool;
      int i, c, ok=TRUE;

      for (i=0; i<sublen; i++) {
      	c = subword[i];
      	if (Pool_contains(remd, c)) {
      	  Pool_remove(remd, c);
      	}else{
      	  ok = FALSE;
      	  Dscan_skip(ds, i);
      	  break;
      	}
      }
      if (ok) {
        process(subword, &remd);
      }
    }
    Dscan_close(ds);
  }
}

char word1[20], word2[20];
int lenall, len1, len2, len3;

void print_all(const char *word3, Pool *remd)
{
  remd = remd;
  printf("%s %s %s\n", word1, word2, word3);
}

void find_third_and_print(const char *word, Pool *remd)
{
  strcpy(word2, word);
  find_subwords(remd, len3, print_all);
}

void find_second_and_third(const char *word, Pool *remd)
{
  int rest = lenall-len1,
      start = rest < len1 ? rest : len1; 
  strcpy(word1, word);
  for (len2=start; len2>=(rest+1)/2; len2--) {
    len3 = rest-len2;
    find_subwords(remd, len2, find_third_and_print);
  }
}

int main(void)
{
  char word[100];
  Pool wordpool;

  dict = Dict_open(NULL);

  while (printf("word> "), fflush(stdout),
	 fgets(word, sizeof(word), stdin),
         (lenall = make_pool(word, &wordpool)) != 0) {
    for (len1=lenall; len1>=(lenall+2)/3; len1--) {
      find_subwords(&wordpool, len1, find_second_and_third);
    }
  }

  Dict_close(dict);
  return 0;
}
