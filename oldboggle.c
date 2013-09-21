/*--------------------------------------------------------------*
 *	Find words in a two-dimensional array of letters moving	*
 *	up, down, left, right or diagonally.			*
 *--------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define XMAX 4
#define YMAX 4
#define MAXPOS (XMAX*YMAX)
#define NONE (-1)

typedef int bits;                       /* must be at least MAXPOS bits */
#define isset(m,b) ((m)&(1<<(b)))
#define adbit(m,b) ((m)|(1<<(b)))
#define stbit(m,b) (m)|=(1<<(b))

int first[26];                          /* first pos containing letter */
int next[MAXPOS];                       /* next pos containing same letter */
bits move[MAXPOS];                      /* isset(move[x],y) if can move x->y */

#define FAIL 0
#define REDO 1
#define EXIT 2

/*--------------------------------------------------------------*
 *	recursively look for a word				*
 *--------------------------------------------------------------*/
int boggle(char *word,                  /* word to look for */
           int pos,                     /* current position in array */
           bits used)                   /* positions visited already */
{
  int c, p;
  while (c = *word++, ! isalpha(c)) {   /* ignore punctuation */
    if (c == '\0') return EXIT;         /* end of string, ok! */
  }
  p = first[toupper(c) - 'A'];          /* first square containing letter */
  if (p == NONE) return FAIL;           /* letter not in square */
  for (; p != NONE; p=next[p]) {
    if (pos == NONE || isset(move[pos],p)) {    /* can move to square */
      if (! isset(used,p)) {            /* not already used */
        switch (boggle(word, p, adbit(used,p))) {   /* recurse on remainder */
        case FAIL: return FAIL;         /* later letter impossible */
        case EXIT: return EXIT;         /* succeeded */
        }
      }
    }
  }
  return REDO;                          /* no luck here */
}

void makemap(char *letters)
{
  int i,j,p,c;
  char *s = letters;
  for (i=0; i<26; i++) {
    first[i] = NONE;
  }
  for (i=0; i<MAXPOS; i++) {
    next[i] = NONE;
    move[i] = 0;
  }
  p = 0;
  for (j=0; j<YMAX; j++) {
    for (i=0; i<XMAX; i++,p++) {
      if (j>0) {
        if (i>0) stbit(move[p],p-XMAX-1);
        stbit(move[p],p-XMAX);
        if (i<XMAX-1) stbit(move[p],p-XMAX+1);
      }
      if (i>0) stbit(move[p],p-1);
      if (i<XMAX-1) stbit(move[p],p+1);
      if (j<YMAX-1) {
        if (i>0) stbit(move[p],p+XMAX-1);
        stbit(move[p],p+XMAX);
        if (i<XMAX-1) stbit(move[p],p+XMAX+1);
      }
    }
  }
  
  p = 0;
  for (j=0; j<YMAX; j++) {
    for (i=0; i<XMAX; i++,p++) {
      c = *s++;
      if (c == '\0') {
        fprintf(stderr, "Too few letters\n");
        exit(1);
      }else if (! isalpha(c)) {
        fprintf(stderr, "Non-alphabetic character %c\n", c);
        exit(1);
      }
      c = toupper(c) - 'A';
      
      next[p] = first[c];
      first[c] = p;
    }
  }
  if (*s != '\0') {
    fprintf(stderr, "Too many letter\n");
    exit(1);
  }
}	

#define MAXTEST 6
char *test[6] = {
"the","cat","sat","on","a","mat"};

/*--------------------------------------------------------------*
 *	Main program						*
 *--------------------------------------------------------------*/
int main(void)
{
  char line[256];
  char letters[256];
  char name[40];
  FILE *file;
  int i;

  printf("Boggle square size is %d lines of %d characters\n", YMAX, XMAX);
  strcpy(letters, "");
  for (i=0; i<YMAX; i++) {
    printf("Enter line %d :", i+1);
    gets(line);
    if (strlen(line) != XMAX) {
      fprintf(stderr, "Wrong length of line\n");
      exit(1);
    }
    strcat(letters, line);
  }
  makemap(letters);
  
  for (i=2; i<=18; i++) {
    sprintf(name, "<Data>.Words%d", i);
    file = fopen(name, "r");
    if (file == NULL) {
      fprintf(stderr, "Could not open %s\n", name);
      exit(1);
    }
    for (; fgets(line, 256, file); ) {
      if (boggle(line, NONE, 0) == EXIT) {
      	printf("-- %s", line);
      }
    }
    fclose(file);
  }

/*    
  for (i=0; i<MAXTEST; i++) {
    if (boggle(test[i], NONE, 0) == EXIT) {
      printf("-- %s\n", test[i]);
    }
  }
*/
  return 0;
} 
