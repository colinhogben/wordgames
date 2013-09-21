/*--------------------------------------------------------------*
 *	Definitions of dictionary files
 *--------------------------------------------------------------*/
#ifndef _d2file_h
#define _d2file_h

/* Dictionary as loaded from file */
typedef struct {
  char id[2];				/* "d2" */
  short len;				/* length of words */
  int nhead;				/* number of head groups */
} D2fhdr;

typedef struct {
  char head[2];				/* first two chars */
  char UNUSED[2];
  int tend;				/* index past last tail */
} D2fsect;

#endif /* !_d2file_h */
