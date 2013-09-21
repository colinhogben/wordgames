/*--------------------------------------------------------------*
 *	Match a string, wildcards # and *			*
 *--------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "match.h"

typedef struct {
  int code;				/* >0 = len(text), <0 = len(##), 0=* */
  const char *text;
} Segment;

Segment segment[32];
int numseg=0;
int match_min=0;
int match_max=0;
int numstar=0;

static void dump_match(void)
{
  int i,j,code;
  for (i=0; i<numseg; i++) {
    code = segment[i].code;
    printf("%d\t", code);
    if (code < 0) {
      for (j=0; j<(-code); j++) {
        printf("#");
      }
    }else if (code > 0) {
      printf("%*s", code, segment[i].text);
    }else{
      printf("*");
    }
    printf("\n");
  }
}

/*--------------------------------------------------------------*
 *	Compile a match pattern					*
 *--------------------------------------------------------------*/
void init_match(const char *pat)
{
  const char *lit = NULL;
  char c;
  
  numseg = 0;
  match_min = 0;
  numstar = 0;
  for (; (c=*pat) != '\0'; pat++) {
    switch (c) {
    case '*':
      if (lit != NULL) {
        segment[numseg++].code = pat-lit;
        lit = NULL;
      }
      segment[numseg++].code = 0;
      numstar++;
      break;
    case '#':
    case '.':
      match_min++;
      if (lit != NULL) {
        segment[numseg++].code = pat-lit;
        lit = NULL;
      }
      segment[numseg].code = -1;
      for (; pat[1] == '#'; pat++) {
        match_min++;
      	segment[numseg].code--;
      }
      numseg++;
      break;
    default:
      match_min++;
      if (lit == NULL) {
        lit = pat;
        segment[numseg].text = lit;
      }
    }
  }
  if (lit != NULL) {
    segment[numseg++].code = pat-lit;
  }
  match_max = numstar? 20: match_min;
}

static int match_tail(const char *word,
                      Segment seg[],
                      int nseg,
                      int nstar,
                      int spare)
{
  int code;
  for (; nseg; seg++,nseg--) {
    code = seg[0].code;
    if (code < 0) {                     /* (-code) single wildcards "#" */
      word += (-code);
    }else if (seg[0].code > 0) {        /* literal text */
      if (strncmp(word, seg[0].text, code) != 0) {
        return 0;
      }
      word += code;
    }else{                              /* multiple wildcard "*" */
      if (nstar) {
        for (; spare >= 0; word++,spare--) {
          if (match_tail(word, seg+1, nseg-1, nstar-1, spare)) {
            return 1;                   /* tail matched */
          }
        }
        return 0;
      }else{
        return match_tail(word+spare, seg+1, nseg-1, 0, 0);
      }
    }
  }
  return (word[0]=='\0');
}

int match_word(const char *word)
{
  int spare = strlen(word) - match_min;
  if (spare < 0) return 0;
  return match_tail(word, segment, numseg, numstar, spare);
}
