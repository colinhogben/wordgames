/*======================================================================
 *	Create graph for a list of words
 *	Investigate how many have common endings
 *======================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0
#define ENDCHAR '\0'
#define MAXLEN 30                      /* maximum length of word allowed */
#define MAXHASH 257                    /* size of hash table */

typedef struct {
  char ch;
  struct node_t *next;                 /* successor node */
} Succ;

typedef struct node_t {
  struct node_t *hnext;                /* next on hash chain */
  int use;			       /* number of times used */
  int nsucc;                           /* number of successors */
  Succ succ[0];                        /* array of successors */
} Node;

char *progname;
Node *cnode[MAXLEN+2];
char last[MAXLEN+1];                   /* last word examined */
int lastlen = 0;
Node *dict[MAXHASH];                   /* hashed dictionary */
int nword = 0;
int nchar = 0;
int nmatch = 0;
int nmat[MAXLEN+1];
int nalloc = 0;

Node *lookup(const Node *node);        /* find or create node in dictionary */
int hash(const Node *node);            /* calculate hash value */
Node *copynode(const Node *node);      /* copy node into free space */
int eqnode(const Node *x, const Node *y); /* compare nodes */

Node *lookup(const Node *node)
{
  int hval;
  Node *ptr;
  if (node == NULL) return NULL;
  hval = hash(node);
  for (ptr=dict[hval]; ptr != NULL; ptr=ptr->hnext) { /* scan hash list */
    if (eqnode(ptr, node)) {
      ptr->use += 1;
      return ptr;                      /* found */
    }
  }
  ptr = copynode(node);                /* not found, so create a new one */
  ptr->hnext = dict[hval];
  ptr->use = 1;
  dict[hval] = ptr;                    /* add to hash list */
  return ptr;
}

int eqnode(const Node *x, const Node *y)
{
  int i;
  if (x->nsucc != y->nsucc) return FALSE;
  for (i=0; i < x->nsucc; i++) {
    if (x->succ[i].ch != y->succ[i].ch) return FALSE;
    if (x->succ[i].next != y->succ[i].next) return FALSE;
  }
  return TRUE;
}

Node *copynode(const Node *node)
{
  int i;
  Node *new = (Node *)malloc(sizeof(Node) + node->nsucc*sizeof(Succ));
  if (new == NULL) {
    fprintf(stderr, "%s: out of allocation space\n", progname);
    exit(1);
  }
/*  nalloc += sizeof(Node) + node->nsucc*sizeof(Succ);*/
  nalloc += sizeof(int)*(1+node->nsucc);
  *new = *node;                        /* copy fixed part */
  for (i=0; i < node->nsucc; i++) {
    new->succ[i] = node->succ[i];      /* copy successors */
  }
  return new;
}

int hash(const Node *node)
{
  int i;
  int sum = node->nsucc;
  for (i=0; i<node->nsucc; i++) {
    sum *= 13;
    sum += node->succ[i].ch + (int)(node->succ[i].next);
  }
  sum %= MAXHASH;
  if (sum < 0) sum += MAXHASH;
  return sum;
}

void printnode(const Node *node,
               const char *sep)
{
  static char stub[MAXLEN+1];
  static int slen = 0;
  int i;
  if (node == NULL) {
    stub[slen] = '\0';
    printf("%s", stub);
  }else{
    for (i=0; i < node->nsucc; i++) {
      if (i != 0) printf("%s", sep);     /* separate from previous */
      stub[slen++] = node->succ[i].ch;
      printnode(node->succ[i].next, sep);
      slen--;
    }
  }
}

void dump(void)
{
  int i, j;
  Node *node;
  for (i=0; i <= lastlen; i++) {
    node = cnode[i];
    for (j=0; j<node->nsucc; j++) {
      printf("%c(", node->succ[j].ch);
      if (j<node->nsucc-1) printnode(node->succ[j].next, "|");
      printf(") ");
    }
    printf("\n");
  }
}

void addword(const char *word)
{
  int m,i;
  Node *node;
  Succ *succ;
  nword += 1;
  for (m=0; word[m]==last[m]; m++) {   /* skip matching chars */
/*    cnode[m+1]->use += 1;*/
  }
  nmatch += m;
  nmat[m] += 1;
  for (i=lastlen-1; i>=m; i--) {
    node = cnode[i];
    succ = &node->succ[node->nsucc-1];
    succ->next = lookup(cnode[i+1]);
  }
  strcpy(last, word);                  /* new 'last' word */
  lastlen = strlen(word);
  nchar += lastlen;
  for (i=m; i < lastlen; i++) {
    node = cnode[i];             /* begin new nodes */
    succ = &node->succ[node->nsucc++];
    succ->ch = word[i];
    cnode[i+1]->nsucc = 0;
/*    cnode[i+1]->use = 1;*/
  }
  node = cnode[lastlen];
  succ = &node->succ[node->nsucc++];
  succ->ch = ENDCHAR;
  succ->next = NULL;
}

#define MAXBEST 100
void printbest(void)
{
  Node *best[MAXBEST];
  int score[MAXBEST], min=0, imin=0;
  static Node dummy = {NULL, 0, 0};
  Node *ptr;
  int i, h;
  for (i=0; i<MAXBEST; i++) {
    best[i] = &dummy;
    score[i] = 0;
  }
  for (h=0; h<MAXHASH; h++) {
    for (ptr=dict[h]; ptr; ptr=ptr->hnext) {
      if (ptr->nsucc > 1 && ptr->use > min) {
	best[imin] = ptr;		/* replace minimum */
	score[imin] = ptr->use;
	imin = 0;
	for (i=1; i<MAXBEST; i++) {
	  if (score[i] < score[imin]) imin = i; /* find minimum */
	}
	min = score[imin];
      }
    }
  }
  for (i=0; i<MAXBEST; i++) {
    printf("%d\t", score[i]);
    printnode(best[i], "|");
    printf("\n");
  }
}

int main(int argc, char *argv[])
{
  char line[80];
  int i, ln;
  Node *ptr;

  progname = argv[0];
  strcpy(last, "");
  lastlen = strlen(last);
  for (i=0; i<MAXLEN+2; i++) {
    cnode[i] = (Node *)malloc(sizeof(Node) + 27*sizeof(Succ));
    cnode[i]->nsucc = 0;
  }
  cnode[0]->nsucc = 0;
  
  for (ln=1; gets(line); ln++) {
    int ok = TRUE;
    for (i=0; line[i]; i++) {
      if (! isalpha(line[i])) {
	fprintf(stderr, "%s: non-alphabetic character(s) on line %d\n",
		progname, ln);
	ok = FALSE;
	break;
      }
    }
    if (ok) {
      for (i=0; line[i]; i++) {
	if (isupper(line[i])) {
	  line[i] = tolower(line[i]);
	}
      }
      if (strcmp(line, last) <= 0) {
	fprintf(stderr, "%s: line %d not in alphabetic order\n",
		progname, ln);
	exit(1);
      }
      addword(line);
    }
  }
  addword("");

  printf("%d words, %d characters, %d matched\n", nword-1, nchar, nmatch);
  for (i=0; i<=MAXLEN+1; i++) {
    printf("matching %d: %d\n", i, nmat[i]);
  }
  printf("%d bytes would be allocated\n", nalloc);
  printbest();
  return 0;
}
