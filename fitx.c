/*--------------------------------------------------------------*
 *	Fit words into a crossword grid
 *--------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BLANK ' '
#define STOP_H '-'
#define STOP_V '|'
#define STOP_HV '#'
#define is_letter(c) isalpha(c)

#define MAXLEN 15
#define MAXWORD 20
#define CON_SCORE 10

typedef struct {
  char word[MAXLEN+1];
  int len;
  int optional;
} Word;

typedef struct {
  char pos[MAXLEN+2][MAXLEN+2];
  int xmax, ymax;
  int score;
} Grid;

typedef struct {
  enum {INIT, HORIZCON, VERTCON, HORIZ, VERT, OMIT} state;
  int x, y;
  int xmax, ymax, len;
} Iter;

/*--------------------------------------------------------------*
 *	Implementation of Word
 *--------------------------------------------------------------*/
void Word_Word(Word *word, const char *line)
{
  word->optional = line[0]=='?';
  strcpy(word->word, line+word->optional);
  word->len = strlen(word->word);
}

/*--------------------------------------------------------------*
 *	Grid into which words are placed
 *--------------------------------------------------------------*/
void Grid_Grid(Grid *grid, int xmax, int ymax)
{
  int x, y;
  grid->xmax = xmax;
  grid->ymax = ymax;
  for (y=0; y<ymax+2; y++)
    for (x=0; x<xmax+2; x++)
      grid->pos[y][x] = BLANK;
  grid->score = 0;
}

void Grid_print(Grid *grid)
{
  int x, y, c;
  for (y=0; y<grid->ymax; y++) {
    for (x=0; x<grid->xmax; x++) {
      c = grid->pos[1+y][1+x];
      printf(" %c", is_letter(c) ? c : '.');
    }
    printf("\n");
  }
  printf("Score: %d\n\n", grid->score);
}

/*--------------------------------------------------------------*
 *	Iteration through possible positions
 *--------------------------------------------------------------*/
void Iter_Iter(Iter *iter, Grid *grid, Word *word)
{
  iter->xmax = grid->xmax;
  iter->ymax = grid->ymax;
  iter->len = word->len;
  iter->state = INIT;
}

int Iter_next(Iter *iter)
{
  switch (iter->state) {
  case INIT:
    iter->y = 0;
  hcy:
    if (iter->y == iter->ymax) goto ehcy;
    iter->x = 0;
  hcx:
    if (iter->x + iter->len > iter->xmax) goto ehcx;
    iter->state = HORIZCON;
    return 1;
  case HORIZCON:
    ++iter->x;
    goto hcx;
  ehcx:
    ++iter->y;
    goto hcy;
  ehcy:

    iter->x = 0;
  vcx:
    if (iter->x == iter->xmax) goto evcx;
    iter->y = 0;
  vcy:
    if (iter->y + iter->len > iter->ymax) goto evcy;
    iter->state = VERTCON;
    return 1;
  case VERTCON:
    ++iter->y;
    goto vcy;
  evcy:
    ++iter->x;
    goto vcx;
  evcx:

    iter->y = 0;
  hy:
    if (iter->y == iter->ymax) goto ehy;
    iter->x = 0;
  hx:
    if (iter->x + iter->len > iter->xmax) goto ehx;
    iter->state = HORIZ;
    return 1;
  case HORIZ:
    ++iter->x;
    goto hx;
  ehx:
    ++iter->y;
    goto hy;
  ehy:

    iter->x = 0;
  vx:
    if (iter->x == iter->xmax) goto evx;
    iter->y = 0;
  vy:
    if (iter->y + iter->len > iter->ymax) goto evy;
    iter->state = VERT;
    return 1;
  case VERT:
    ++iter->y;
    goto vy;
  evy:
    ++iter->x;
    goto vx;
  evx:

    iter->state = OMIT;
    return 1;
  case OMIT:
    ;
  }
  return 0;
}

/*--------------------------------------------------------------*
 *	See if a word will fit in a given position
 *--------------------------------------------------------------*/
int Grid_fitword_h(Grid *grid, Word *word, Iter *iter)
{
  int x=iter->x, y=iter->y, len=iter->len;
  int result, i;

  /*
   * First check the end positions
   */
  if (is_letter(grid->pos[1+y][1+x-1])) return 0;
  if (is_letter(grid->pos[1+y][1+x+len])) return 0;

  /*
   * Now check each letter in turn, noting if we connect
   */
  result = 1;
  for (i=0; i<len; i++) {
    int c = grid->pos[1+y][1+x+i];
    if (c==word->word[i]) {		/* connects with old word */
      result = 2;
    }else if (is_letter(c) ||
              c==STOP_H || c==STOP_HV) { /* blocked */
      return 0;
    }					/* BLANK or STOP_V are OK */
  }
  return result;
}

int Grid_fitword_v(Grid *grid, Word *word, Iter *iter)
{
  int x=iter->x, y=iter->y, len=iter->len;
  int result, i;

  /*
   * First check the end positions
   */
  if (is_letter(grid->pos[1+y-1][1+x])) return 0;
  if (is_letter(grid->pos[1+y+len][1+x])) return 0;

  /*
   * Now check each letter in turn, noting if we connect
   */
  result = 1;
  for (i=0; i<len; i++) {
    int c = grid->pos[1+y+i][1+x];
    if (c==word->word[i]) {		/* connects with old word */
      result = 2;
    }else if (is_letter(c) ||
              c==STOP_V || c==STOP_HV) { /* blocked */
      return 0;
    }					/* BLANK or STOP_V are OK */
  }
  return result;
}

int Grid_fitword(Grid *grid, Word *word, Iter *iter)
{
  switch (iter->state) {
  case INIT:
    break;
  case HORIZCON:
    return Grid_fitword_h(grid, word, iter) == 2;
  case VERTCON:
    return Grid_fitword_v(grid, word, iter) == 2;
  case HORIZ:
    return Grid_fitword_h(grid, word, iter) == 1;
  case VERT:
    return Grid_fitword_v(grid, word, iter) == 1;
  case OMIT:
    return word->optional;
  }
  return 0;
}
    
void Grid_addword(Grid *new, const Grid *old,
		  const Word *word, const Iter *iter)
{
  int x=iter->x, y=iter->y, len=word->len;
  int i;
  char *c;

  *new = *old;
  switch (iter->state) {
  case INIT:
    break;
  case HORIZCON:
    new->score += CON_SCORE;
  case HORIZ:
    new->score += len;
    new->pos[1+y][1+x-1] = STOP_HV;	/* guard start */
    new->pos[1+y][1+x+len] = STOP_HV;	/* guard end */
    for (i=0; i<len; i++) {
      new->pos[1+y][1+x+i] = word->word[i]; /* insert char of word */

      c = &new->pos[1+y-1][1+x+i];	/* guard top side */
      if (*c==BLANK) *c = STOP_H;
      else if (*c==STOP_V) *c = STOP_HV;
      c = &new->pos[1+y+1][1+x+i];	/* guard bottom side */
      if (*c==BLANK) *c = STOP_H;
      else if (*c==STOP_V) *c = STOP_HV;
    }
    break;
  case VERTCON:
    new->score += CON_SCORE;
  case VERT:
    new->score += len;
    new->pos[1+y-1][1+x] = STOP_HV;	/* guard start */
    new->pos[1+y+len][1+x] = STOP_HV;	/* guard end */
    for (i=0; i<len; i++) {
      new->pos[1+y+i][1+x] = word->word[i]; /* insert char of word */

      c = &new->pos[1+y+i][1+x-1];	/* guard left side */
      if (*c==BLANK) *c = STOP_V;
      else if (*c==STOP_H) *c = STOP_HV;
      c = &new->pos[1+y+i][1+x+1];	/* guard right side */
      if (*c==BLANK) *c = STOP_V;
      else if (*c==STOP_H) *c = STOP_HV;
    }
    break;
  case OMIT:
    break;
  }
}

Grid grids[MAXWORD+1];
Word words[MAXWORD+1];
Iter iters[MAXWORD+1];

void findall(void (*use)(Grid *))
{
  Grid *grid = grids;
  Word *word = words;
  Iter *iter = iters;
  enum {CALL,REDO,EXIT,FAIL} state = CALL;

  while (1) {
    switch (state) {
    case CALL:
      if (word->len == 0) {
      	state = EXIT;
      	break;
      }
      Iter_Iter(iter, grid, word);
    case REDO:
      if (Iter_next(iter)) {
        if (Grid_fitword(grid, word, iter)) {
          Grid_addword(grid+1, grid, word, iter);
          grid++;
          word++;
          iter++;
          state = CALL;
        }else{
          state = REDO;
        }
      }else{
        state = FAIL;
      }
      break;
    case FAIL:
      if (grid == grids) return;
      grid--;
      word--;
      iter--;
      state = REDO;
      break;
    case EXIT:
      use(grid);			/* call function passed */
      state = FAIL;
      break;
    }
  }
}

#define NBMAX 10
#define NBTYPE Grid
#define NBBETTER(a,b) ((a)->score > (b)->score)
#include "nbest.h"

void print_add(Grid *g)
{
  Grid_print(g);
  nb_add(g);
}

int main()
{
  int i;
#if 1
  Word_Word(&words[0], "?colin");
  Word_Word(&words[1], "?trudy");
  Word_Word(&words[2], "?stuart");
  Word_Word(&words[3], "?helen");
  Word_Word(&words[4], "?gordon");
  Word_Word(&words[5], "");		/* empty to terminate */
  Grid_Grid(&grids[0], 8, 6);
#elif 0
  Word_Word(&words[0], "?pig");
  Word_Word(&words[1], "?dog");
  Word_Word(&words[2], "?cow");
  Word_Word(&words[3], "");		/* empty to terminate */
  Grid_Grid(&grids[0], 4, 3);
#endif

  findall(nb_add);
  nb_sort();
  for (i=0; i<nb_count; i++) {
    Grid_print(nb_sorted[i]);
  }
  return 0;
}

