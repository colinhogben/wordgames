/*======================================================================
 *	Interface definition for a d2 dictionary
 *======================================================================*/

#define MAXLEN 20

typedef struct _Dict *Dict;
typedef struct _Dictl *Dictl;
typedef struct _Dscan *Dscan;

extern Dict Dict_open(const char *stub);
extern int Dict_contains(Dict d, const char *word, int len);
#define Dict_isword Dict_contains	/* Obsolete interface */
extern void Dict_close(Dict d);

extern Dictl Dictl_open(Dict d, int len);
extern Dictl Dictl_fopen(const char *filename);
extern int Dictl_contains(Dictl dl, const char *word);
#define Dictl_isword Dictl_contains	/* Obsolete interface */
extern int Dictl_length(Dictl dl);	/* Number of elements */
extern void Dictl_close(Dictl dl);

extern Dscan Dscan_open(Dict d, int len);
extern Dscan Dscan_openl(Dictl dl);
extern const char *Dscan_read(Dscan ds);
extern void Dscan_skip(Dscan ds, int skip);
extern void Dscan_close(Dscan ds);

typedef struct _Dpool Dpool;

#define dpool_clear(pp) memset(pp,0,sizeof(Dpool))
extern void dpool_add(Dpool *, const Dpool *);
#define dpool_add_char(pp,c) (isalpha(c) ? (pp)->nlet[toupper(c)-'A']++ : (pp)->nwild++)
extern void dpool_add_word(Dpool *, const char *);
extern int dpool_sub(Dpool *, const Dpool *);
extern int dpool_sub_char(Dpool *, int);
extern int dpool_sub_word(Dpool *, const char *);
extern void dpool_string(char *, const Dpool *, int /*cWild*/);
