/*--------------------------------------------------------------*
 *	Find wordsquares matching given pattern
 *--------------------------------------------------------------*/
#include "d2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXSQUARE 8

typedef struct {
  int len;
  int all;
  Dict dict;
  int todo;				/* Bitmap */
  const char *row[MAXSQUARE];
} Square;

static void fit_one(Square *sq, int r);
static void fit_any(Square *sq);
static int word_misfit(Square *sq, int r, const char *word);
static void result(Square *sq);
static void dump(Square *sq, int indent);
static void indent(int n);

static int verbose;

int main(int argc,
         char *argv[])
{
  Dict d = Dict_open(NULL);
  int i,len;
  Square sq;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s size\n", argv[0]);
    exit(1);
  }
  len = atoi(argv[1]);
  if (len < 2 || len > MAXSQUARE) {
    fprintf(stderr, "%s: Bad size - must be 2..%d\n",
	    argv[0], MAXSQUARE);
    exit(2);
  }

  sq.len = len;
  sq.all = 1;
  sq.dict = d;
  sq.todo = (1 << len) - 1;		/* Bitmap - all to do */
  for (i=0; i<len; i++) {
    sq.row[i] = "................";
  }

  if (verbose) dump(&sq, 0);
  fit_one(&sq, 0);

  exit(0);
}

/*-----------------------------------------------------------------------
 *	Try to fit a word in the given row
 *-----------------------------------------------------------------------*/
static void fit_one(Square *sq, int r)
{
  Dscan ds;
  const char *word;
  ds = Dscan_open(sq->dict, sq->len);
  while ((word = Dscan_read(ds)) != NULL) {
    /*--- See if word fits */
    int m = word_misfit(sq, r, word);
    if (m >= 0) {
      Dscan_skip(ds, m);
    } else {
      const char *save_row = sq->row[r];
      sq->row[r] = word;
      sq->todo &= ~(1 << r);

      /*--- Word fits */
      if (verbose) printf("Row %d, using %s\n", r, word);
      if (sq->todo == 0) {
	/*--- Completed */
	result(sq);
      } else {
	/*--- Recurse */
	fit_any(sq);
      }
      sq->todo |= (1 << r);
      sq->row[r] = save_row;
    }
  }
  Dscan_close(ds);
}

/*-----------------------------------------------------------------------
 *	Find a row to try to fit a word in
 *-----------------------------------------------------------------------*/
static void fit_any(Square *sq)
{
  int hardest = -1;
  int hardness = 100000000;
  int r;
  if (verbose) dump(sq, 2);
  for (r=0; r<sq->len; r++) {
    if (sq->todo & (1 << r)) {
      Dscan ds;
      const char *word;
      int fits = 0;

      /*--- Count how many words fit */
      ds = Dscan_open(sq->dict, sq->len);
      while ((word = Dscan_read(ds)) != NULL) {
	int m = word_misfit(sq, r, word);
	if (m >= 0) {
	  Dscan_skip(ds, m);
	} else {
	  fits += 1;
	}
      }
      Dscan_close(ds);

      /*--- See if this has fewest possibilities, or even none */
      if (fits == 0) {
	return;				/* >>>>> No point continuing */
      } else if (fits < hardness) {
	hardest = r;
	hardness = fits;
      }
    }
  }

  /*--- Do next step of recursion */
  fit_one(sq, hardest);
}

/*-----------------------------------------------------------------------
 *	Return index of mismatch, or -1 if word matches
 *-----------------------------------------------------------------------*/
static int word_misfit(Square *sq, int r, const char *word)
{
  int i;
  for (i=0; i<sq->len; i++) {
    char c = sq->row[i][r];
    if (! (c == '.' || c == word[i])) {
      return i;
    }
  }
  return -1;
}

/*-----------------------------------------------------------------------
 *	Handle result
 *-----------------------------------------------------------------------*/
static void result(Square *sq)
{
  dump(sq, 0);
  if (sq->all) {
    printf("\n");
  } else {
    exit(0);
  }
}

static void dump(Square *sq, int lvl)
{
  int r;
  for (r=0; r<sq->len; r++) {
    indent(lvl); printf("%.*s\n", sq->len, sq->row[r]);
  }
}

static void indent(int n)
{
  while (n-- > 0) {
    printf(" ");
  }
}
