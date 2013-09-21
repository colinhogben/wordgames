/*======================================================================
 *	Interface definition for a writeable d2 dictionary
 *======================================================================*/
typedef struct _Dictw *Dictw;

extern Dictw Dictw_create(int len);
extern void Dictw_add(Dictw dw, const char *word);
extern void Dictw_write(Dictw dw, const char *filename);
extern void Dictw_delete(Dictw dw);
