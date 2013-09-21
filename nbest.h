/*--------------------------------------------------------------*
 *	Definitions for a heap to keep the 'n' best items from
 *	a sequence presented.
 *	Includer must define:
 *	NBMAX		Number of items to store
 *	NBTYPE		Type of items
 *	NBBETTER(x,y)	Comparison of two NBTYPE pointers
 *--------------------------------------------------------------*/

static NBTYPE nb_item[NBMAX];
static NBTYPE *nb_ptr[NBMAX];
static NBTYPE *nb_sorted[NBMAX];
static int nb_count = 0;

static void nbshuffle(NBTYPE *ptr[], int n)
{
  int i, j;

  for (i=0;;) {
    if ((j=2*i+2) < n && NBBETTER(ptr[2*i+1],ptr[j])) {
      if (NBBETTER(ptr[i],ptr[j])) {
        NBTYPE *temp = ptr[i];		/* swap with right child */
        ptr[i] = ptr[j];
        ptr[j] = temp;
      }
      i = j;
    }else if ((j=2*i+1) < n) {
      if (NBBETTER(ptr[i],ptr[j])) {
        NBTYPE *temp = ptr[i];		/* swap with right child */
        ptr[i] = ptr[j];
        ptr[j] = temp;
      }
      i = j;
    }else{
      break;
    }
  }
}
        
static void nb_add(NBTYPE *value)
{
  int i, j;
  if (nb_count < NBMAX) {
    i = nb_count++;
    nb_item[i] = *value;		/* save value */
    nb_ptr[i] = &nb_item[i];
    while (i > 0 && (j = (i-1)/2, NBBETTER(nb_ptr[j],nb_ptr[i]))) {
      NBTYPE *temp = nb_ptr[i];
      nb_ptr[i] = nb_ptr[j];
      nb_ptr[j] = temp;
      i = j;
    }
  }else if (NBBETTER(value,nb_ptr[0])) { /* better than least good */
    *nb_ptr[0] = *value;		/* replace value */
    nbshuffle(nb_ptr, nb_count);
  }
}

static void nb_sort(void)
{
  NBTYPE *temp;
  int i;
  for (i=0; i<nb_count; i++) {
    nb_sorted[i] = nb_ptr[i];
  }
  for (i=nb_count; i>0; ) {
    temp = nb_sorted[0];
    nb_sorted[0] = nb_sorted[--i];
    nbshuffle(nb_sorted, i);
    nb_sorted[i] = temp;
  }
}

