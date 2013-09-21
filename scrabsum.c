#define letter_value(l)
#define allset_map
#define add_map(m, c)
#define in_map(m, c)
#define rack_map(r) ((r).nblank!=0? allset_map: (r).available)

void print_board(void)
static void read_board(void)

static Map try_gap(char *word, int len,	int pos, Map available)
void find_allowed(const char *line, Map available, Map allow[])

int word_score(const char *, int len, int row, int col)
int cross_score(int row, int col, int tilescore)
int play_score(const char *word, int len, int row, int col)

void consider(const char *word, const char *bword, int len,
	      int row, int col, int score)
void consider_word(const char *word, const char *bword, int len,
		   int row, int col, const char *bused)

int fit_word(const char *word, int len, Rack rack, char line[],
	     Map allow[], char *bused)
void find_words(int row, int col, int len, Map allow[])
void find_row_words(int row,
		    const char *line, const char *prev, const char *next,
		    Map allow[])
void find_best(void)

int main(int argc, char *argv[])
