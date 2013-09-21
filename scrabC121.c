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

DictCtl dictctl;
void init_word(char *wbuf, int len)
{
  INITWORD(dictctl,len);
}
int get_word(char *wbuf, int pos)
{
  return NEXTWORD(dictctl,wbuf,pos);
}
 
/*----------------------------------------------------------------------
 *	Definitions of constants, types and macros for manipulating them
 *----------------------------------------------------------------------*/
#define EMPTY ' '			/* empty square */
static char empty_line[15+1] = "               ";
#define BLANK '*'
#define MAXBLANK 2
static const int value_table[] = {	/* values of each letter */
/* A  B  C  D  E  F  G  H  I  J  K  L  M */  
   1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3,
/* N  O  P  Q   R  S  T  U  V  W  X  Y  Z */
   1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10,
};
#define letter_value(l) ((l)==EMPTY? 0: value_table[(l)-'A'])

static const int bonus[15][15] = {
#define o 0
#define L2 2
#define L3 3
#define W2 -2
#define W3 -3
  W3, o, o,L2, o, o, o,W3, o, o, o,L2, o, o,W3,
   o,W2, o, o, o,L3, o, o, o,L3, o, o, o,W2, o,
   o, o,W2, o, o, o,L2, o,L2, o, o, o,W2, o, o,
  L2, o, o,W2, o, o, o,L2, o, o, o,W2, o, o,L2,
   o, o, o, o,W2, o, o, o, o, o,W2, o, o, o, o,
   o,L3, o, o, o,L3, o, o, o,L3, o, o, o,L3, o,
   o, o,L2, o, o, o,L2, o,L2, o, o, o,L2, o, o,
  W3, o, o,L2, o, o, o,W2, o, o, o,L2, o, o,W3,
   o, o,L2, o, o, o,L2, o,L2, o, o, o,L2, o, o,
   o,L3, o, o, o,L3, o, o, o,L3, o, o, o,L3, o,
   o, o, o, o,W2, o, o, o, o, o,W2, o, o, o, o,
  L2, o, o,W2, o, o, o,L2, o, o, o,W2, o, o,L2,
   o, o,W2, o, o, o,L2, o,L2, o, o, o,W2, o, o,
   o,W2, o, o, o,L3, o, o, o,L3, o, o, o,W2, o,
  W3, o, o,L2, o, o, o,W3, o, o, o,L2, o, o,W3,
#undef o
#undef L2
#undef L3
#undef W2
#undef W3
};

typedef long int Map;			/* bitmap (or set) of letters */
#define empty_map 0L
#define allset_map ((1L<<26)-(1L<<0))
#define add_map(m, c) ((m) |= (1L<<((c)-'A')))
#define in_map(m, c) (((m) & (1L<<((c)-'A'))) != 0)

typedef struct {
  char tile[7+1];			/* tiles available (except blanks) */
  int nblank;				/* number of blank tiles */
} Rack;

Map rack_map(Rack rack)
{
  Map map;
  if (rack.nblank != 0) {
    map = allset_map;
  }else{
    char *s;
    map = empty_map;
    for (s=rack.tile; *s != '\0'; s++) {
      add_map(map, *s);
    }
  }
  return map;
}

/*----------------------------------------------------------------------
 *	Current state of the board and tiles available
 *----------------------------------------------------------------------*/
char horiz_board[15][15];		/* current state of board */
int horiz_value[15][15];		/* value [0 for blank] */
char vert_board[15][15];		/* the same, transposed */
int vert_value[15][15];			/* value [0 for blank] */

enum dirn {ACROSS,DOWN} dirn;		/* current direction of word */
char (*board)[15];
int (*value)[15];
char (*xboard)[15];
int (*xvalue)[15];
Rack tile_rack = {"ACDINRU",0};		/* tiles available */

void print_map(Map map)
{
  int c;
  for (c='A'; c<='Z'; c++) {
    if (in_map(map,c)) printf("%c", c);
  }
}

/*----------------------------------------------------------------------
 *	Print the current board on standard output
 *	Show blanks as '*' with assumed value in margin
 *----------------------------------------------------------------------*/
void print_board(void)
{
  int r,c;
  char l;
  char blanks[4*MAXBLANK+1];
  for (r=0; r<15; r++) {
    blanks[0] = '\0';			/* no blank values */
    for (c=0; c<15; c++) {
      l = horiz_board[r][c];
      if (l == EMPTY) {
	l = bonus[r][c]==0? '.':':';	/* uncovered board */
      }else{
	if (horiz_value[r][c] == 0) {	/* blank */
	  sprintf(blanks+strlen(blanks), " *=%c", l);
	  l = '*';
	}
      }
      printf(c==0?"%c":" %c", l);
    }
    printf("%s\n", blanks);		/* output marginal notes */
  }
}

/*----------------------------------------------------------------------
 *	Read a scrabble board from standard input
 *----------------------------------------------------------------------*/
static void read_board(void)
{
  int row, col, bl;
  char fline[256];
  char *tok, ch, dumch;
  int blcol[MAXBLANK], nblank;
  int totblank = 0;
  for (row=0; row<15; row++) {
    if (fgets(fline, sizeof(fline), stdin) == NULL) {
      if (feof(stdin)) {
	fprintf(stderr, "Too few input lines\n");
      }else{
	perror(NULL);
      }
      exit(1);
    }else if (fline[strlen(fline)-1] != '\n') {
      fprintf(stderr, "Line %d too long\n", row+1);
      exit(1);
    }

/* Read in the line and note any blank positions */
    nblank = 0;
    for (col=0; col<15; col++) {
      tok = strtok(col==0? fline:NULL, " \t\n");
      if (tok == NULL) {
	fprintf(stderr, "Not enough items in line %d\n", row+1);
	exit(1);
      }else if (strlen(tok) > 1) {
	fprintf(stderr, "Item %d on line %d is too wide\n", col+1, row+1);
	exit(1);
      }
      ch = tok[0];
      if (isalpha(ch)) {		/* letter tile */
	if (islower(ch)) ch = toupper(ch);
	horiz_board[row][col] = ch;
	horiz_value[row][col] = letter_value(ch);
      }else if (ch=='*') {		/* blank tile */
	if (++totblank > MAXBLANK) {
	  fprintf(stderr, "Too many blanks (max %d)\n", MAXBLANK);
	  exit(1);
	}
	blcol[nblank++] = col;		/* remember its position */
	horiz_board[row][col] = '*';
	horiz_value[row][col] = 0;
      }else{				/* empty square */
	horiz_board[row][col] = EMPTY;
	horiz_value[row][col] = 0;
      }
    }

/* If there were any blanks, find out what their assumed values */
    for (bl=0; bl<nblank; bl++) {
      tok = strtok(NULL, " \t\n");
      if (tok == NULL) {
	fprintf(stderr, "Missing blank value on line %d\n", row+1);
	exit(1);
      }else if (sscanf(tok, "*=%c%c", &ch, &dumch) != 1 ||
		!isalpha(ch)) {
	fprintf(stderr, "Bad blank value on line %d\n", row+1);
	exit(1);
      }
      if (islower(ch)) ch = toupper(ch);
      horiz_board[row][blcol[bl]] = ch;
    }

/* Fill in the vertical version of the board too */
    for (col=0; col<15; col++) {
      vert_board[col][row] = horiz_board[row][col];
      vert_value[col][row] = horiz_value[row][col];
    }
  }
}

/*----------------------------------------------------------------------
 *	Try inserting every available character into a gap and see
 *	which of them make words.
 *----------------------------------------------------------------------*/
static Map try_gap(char *word, int len,	/* word containing a hole */
		   int pos,		/* position of hole */
		   Map available)	/* which letters the player has */
{
  int c;
  Map map;
  
  map = empty_map;
  for (c='A'; c<='Z'; c++) {
    if (in_map(available, c)) {		/* only try those available */
      word[pos] = c;
      if (is_word(word, len)) {
	add_map(map, c);		/* record word possible */
      }
    }
  }
  return map;
}

/*----------------------------------------------------------------------
 *	Check along a line to see which single characters may be added
 *	in each position
 *----------------------------------------------------------------------*/
void find_allowed(const char *line,	/* row or column of the board */
		  Map available,	/* which letters the player has */
		  Map allow[])		/* which letters may be placed */
{
  int k,i,j;
  Map map;
  char lline[15];			/* local copy of line */

  strncpy(lline, line, 15);		/* take local copy */
  for (k=0; k<15; k++) {
    if (line[k] != EMPTY) {		/* already a tile there */
      map = empty_map;
      add_map(map, line[k]);		/* allow just that letter */
    }else{
      i = k;
      while (i>0 && line[i-1] != EMPTY) {
	i--;				/* find first preceding letter */
      }
      j = k;
      while (j<15-1 && line[j+1] != EMPTY) {
	j++;				/* find last following letter */
      }
      if (i==j) {			/* none adjacent */
	map = available;		/* so any letter will do */
      }else{
	map = try_gap(lline+i, j+1-i, k-i, available); /* try each letter */
      }
    }
    allow[k] = map;
    printf("("); print_map(allow[k]); printf(") ");
  }
  printf("\n");
}

/*----------------------------------------------------------------------
 *	Calculate the score of a new word, counting bonuses
 *----------------------------------------------------------------------*/
int word_score(const char *word,	/* the new word */
	       int len,			/* its length */
	       int row, int col)	/* where to place it */
{
  int sum = 0, mult = 1;
  int i, lval;
  char *dest = board[row]+col;
  int *val = value[row]+col;
  const int *bon = bonus[row]+col;
  for (i=0; i<len; i++) {
    if (dest[i] == EMPTY) {		/* adding new tile */
      lval = letter_value(word[i]);
      if (bon[i] > 0) lval *= bon[i];	/* n-tuple letter score */
      sum += lval;
      if (bon[i] < 0) mult *= -bon[i];	/* n-tuple word score */
    }else{
      sum += val[i];			/* just add existing tile */
    }
  }
  return sum*mult;
}

/*----------------------------------------------------------------------
 *	Calculate the score obtained from any cross-ways word.
 *	No bonuses are applied.
 *----------------------------------------------------------------------*/
int cross_score(int row, int col,	/* where the new tile is */
		int tilescore)		/* its value */
{
  char *line = xboard[col];
  int *val = xvalue[col];
  int i;
  int score = 0;
  int any = FALSE;
  for (i=row; --i >= 0 && line[i] != EMPTY; ) {	/* look backwards */
    any = TRUE;
    score += val[i];
  }
  for (i=row; ++i <= 15-1 && line[i] != EMPTY; ) { /* look forwards */
    any = TRUE;
    score += val[i];
  }
  if (any) score += tilescore;		/* zero if none either side */
  return score;
}
		
/*----------------------------------------------------------------------
 *	return the score obtained from a given word, including
 *	subsidiary words in the cross-ways direction
 *----------------------------------------------------------------------*/
int play_score(const char *word,	/* word being added */
	       int len,			/* its length */
	       int row, int col)	/* where played */
{
  int i;
  int score = word_score(word, len, row, col);
  for (i=0; i<len; i++) {
    if (board[row][col+i] == EMPTY) {	/* if new tile being played */
      score += cross_score(row, col+i, letter_value(word[i]));
    }
  }
  return score;
}

/*----------------------------------------------------------------------
 *	Details of the best word found so far
 *----------------------------------------------------------------------*/
typedef struct { 
  char word[15+1];
  char bword[15+1];
  int row, col;
  enum dirn dirn;
  int score;
} Play;

#define NBTYPE Play
#define NBMAX 10
#define NBBETTER(x,y) ((x)->score > (y)->score)
#define NBOUTPUT(x) printf("[%d,%d]%s %s %d\n", \
	x->row, x->col, x->dirn==ACROSS? "a":"d", \
	x->word, x->score);
#include "nbest.h"

/*----------------------------------------------------------------------
 *	Consider a word given its score
 *----------------------------------------------------------------------*/
void consider(const char *word,		/* the word being played */
	      const char *bword,	/* the same, including blanks */
	      int len,
	      int row, int col,
	      int score)
{
  Play play;
  strncpy(play.word, word, len);
  play.word[len] = '\0';
  strncpy(play.bword, bword, len);
  play.bword[len] = '\0';
  play.row = row;
  play.col = col;
  play.dirn = dirn;
  play.score = score;
  nb_add(&play);
}

/*----------------------------------------------------------------------
 *	Calculate ways to play the word, given blanks used
 *	blanks may be played in more than one place
 *----------------------------------------------------------------------*/
void consider_word(const char *word,	/* word being added */
		   const char *bword,	/* word including blanks */
		   int len,		/* its length */
		   int row, int col,	/* where it is being played */
		   const char *bused)	/* blanks used */
{
  printf("%s at %d,%d\n", word, row, col);
  if (bused[0] == '\0') {
    consider(word, bword, len, row, col, play_score(bword, len, row, col));
  }else{
    char b = bused[0];
    const char *bpos;
    char newbw[15+1];
    strncpy(newbw, bword, len);
    for (bpos=bword; (bpos=strchr(bpos, b)) != NULL; bpos++) {
      int i = bpos - bword;
      if (board[row][col+i] == EMPTY) {
        newbw[i] = BLANK;
        consider_word(word, newbw, len, row, col, bused+1);
      }
    }
  }
}

/*----------------------------------------------------------------------
 *	see if a word will fit
 *	return number of letters successfully fitted
 *----------------------------------------------------------------------*/
int fit_word(const char *word, int len,	/* word to try */
	     Rack rack,			/* tiles available */
	     char line[], Map allow[],	/* contents of board */
	     char *bused)		/* values of blanks used */
{
  int i;
  char c, *match;

  /* first check that the letters are admissable */
  for (i=0; i<len; i++) {
    if (! in_map(allow[i], word[i])) {	/* impossible letter */
      return i;
    }
  }

  /* now check that the word can be made from the letters available */
  for (i=0; i<len; i++) {
    if (line[i]==EMPTY) {
      c = word[i];
      if ((match = strchr(rack.tile, c)) != NULL) {
	*match = EMPTY;			/* use up the tile */
      }else if (rack.nblank != 0) {
	--rack.nblank;			/* use a blank tile */
	*bused++ = c;			/* record what it replaced */
      }else{
	return i;
      }
    }
  }
  *bused = '\0';
  return len;
}

/*----------------------------------------------------------------------
 *	Find words which fit in the given position
 *----------------------------------------------------------------------*/
void find_words(int row, int col, int len, Map allow[])
{
  char word[15+1];
  char bused[MAXBLANK+1];
  int pos = len;
  for (init_word(word, len), word[len]='\0'; get_word(word, pos); ) {
    if ((pos = fit_word(word, len, tile_rack, board[row]+col, allow,
			bused)) == len) {
      consider_word(word, word, len, row, col, bused);
    }
  }
}
  
/*----------------------------------------------------------------------
 *	Find possible start positions for a [len] character word on the
 *	given line. A word played must include or abut an existing tile
 *	and must be bounded by empty squares or the edge of the board.
 *----------------------------------------------------------------------*/
void find_row_words(int row,		/* which row we are on */
		    const char *line,	/* current contents */
		    const char *prev,	/* previous line */
		    const char *next,	/* next line */
		    Map allow[])	/* what characters are allowed */
{
  int len, col, lastim, lastsp, lastex;

  for (len=3; len<=15; len++) {
    lastim = -1;				/* last unplayable square */
    lastsp = -1;				/* last space */
    lastex = -1;				/* last existing tile */
    for (col=0; col<15; col++) {
      if (line[col] == EMPTY) lastsp = col;
      if (line[col] != EMPTY ||
	  prev[col] != EMPTY || next[col] != EMPTY) lastex = col;
      if (allow[col] == 0) {
        lastim = col;
      }else{
        if (lastim <= col-len &&		/* 'len' letters allowed */
	    lastsp > col-len &&		/* contains an empty space */
	    lastex > col-len &&		/* adjoins or includes existing tile */
	    (col-len < 0 || line[col-len] == EMPTY) &&
	    (col+1 > 15 || line[col+1] == EMPTY)) { /* bounded by spaces */
	  find_words(row, col-len+1, len, allow+col-len+1);
        }
      }
    }
  }
}

/*----------------------------------------------------------------------
 *	check the entire board for words that may be added
 *----------------------------------------------------------------------*/
void find_best(void)
{
  int row,col;
  Map allow[15][15], alcol[15];
  Map available;

  /* find which characters are allowed on each row */
  available = rack_map(tile_rack);
  for (col=0; col<15; col++) {
    find_allowed(xboard[col], available, alcol);
    for (row=0; row<15; row++) {
      allow[row][col] = alcol[row];
    }
  }

  for (row=0; row<15; row++) {
    find_row_words(row, board[row],
		   row>0? board[row-1]: empty_line,
		   row<15-1? board[row+1]: empty_line,
		   allow[row]);
  }
}

/*----------------------------------------------------------------------
 *	main entry point
 *----------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
  progname = argv[0];

  read_board();

  dirn = ACROSS;
  board = horiz_board;
  value = horiz_value;
  xboard = vert_board;
  xvalue = vert_value;
  find_best();
  
  dirn = DOWN;
  board = vert_board;
  value = vert_value;
  xboard = horiz_board;
  xvalue = horiz_value;
  find_best();

  if (nb_count > 0) {
    nb_output(-1);
  }else{
    printf("Cannot play\n");
  }
  return 0;
}
