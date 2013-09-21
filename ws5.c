#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

typedef struct {
    char row[5][5];
} Square;

typedef void (*fit_func)(Square *);
static fit_func fitn[];		/* Forward decl */

typedef struct {
    int fd;
    int nword;
    const char (*words)[5+1];
} Dict;
static Dict shuffled_dict[32];

static int debug;

int main(int argc, char **argv)
{
    int mask, r, i;
    Square sq;

    for (mask=0; mask<32; mask++) {
	Dict *dict = &shuffled_dict[mask];
	char filename[40];
	struct stat st;

	sprintf(filename, "shuf%02d_%d", 5, mask);
	dict->fd = open(filename, 0);
	if (dict->fd < 0) {
	    perror(filename);
	    exit(2);
	}
	fstat(dict->fd, &st);
	dict->nword = st.st_size / (5+1);
	dict->words = mmap(0, st.st_size, PROT_READ, MAP_SHARED, dict->fd, 0);
    }

    for (r=0; r<5; r++) {
	for (i=0; i<5; i++) {
	    sq.row[r][i] = '.';
	}
    }
    fitn[0](&sq);
    return 0;
}
static void solution(Square *sq)
{
    int r;
    for (r=0; r<5; r++) {
	printf("%.5s\n", sq->row[r]);
    }
    printf("\n");
}
#define CMP1(d,a,b) ((d = (a)[0] - (b)[0]))
#define CMP2(d,a,b) ((d = (a)[1] - (b)[1]))
#define CMP3(d,a,b) ((d = (a)[0] - (b)[0]) || \
	(d = (a)[1] - (b)[1]))
#define CMP4(d,a,b) ((d = (a)[2] - (b)[2]))
#define CMP5(d,a,b) ((d = (a)[0] - (b)[0]) || \
	(d = (a)[2] - (b)[2]))
#define CMP6(d,a,b) ((d = (a)[1] - (b)[1]) || \
	(d = (a)[2] - (b)[2]))
#define CMP7(d,a,b) ((d = (a)[0] - (b)[0]) || \
	(d = (a)[1] - (b)[1]) || \
	(d = (a)[2] - (b)[2]))
#define CMP8(d,a,b) ((d = (a)[3] - (b)[3]))
#define CMP9(d,a,b) ((d = (a)[0] - (b)[0]) || \
	(d = (a)[3] - (b)[3]))
#define CMP10(d,a,b) ((d = (a)[1] - (b)[1]) || \
	(d = (a)[3] - (b)[3]))
#define CMP11(d,a,b) ((d = (a)[0] - (b)[0]) || \
	(d = (a)[1] - (b)[1]) || \
	(d = (a)[3] - (b)[3]))
#define CMP12(d,a,b) ((d = (a)[2] - (b)[2]) || \
	(d = (a)[3] - (b)[3]))
#define CMP13(d,a,b) ((d = (a)[0] - (b)[0]) || \
	(d = (a)[2] - (b)[2]) || \
	(d = (a)[3] - (b)[3]))
#define CMP14(d,a,b) ((d = (a)[1] - (b)[1]) || \
	(d = (a)[2] - (b)[2]) || \
	(d = (a)[3] - (b)[3]))
#define CMP15(d,a,b) ((d = (a)[0] - (b)[0]) || \
	(d = (a)[1] - (b)[1]) || \
	(d = (a)[2] - (b)[2]) || \
	(d = (a)[3] - (b)[3]))
#define CMP16(d,a,b) ((d = (a)[4] - (b)[4]))
#define CMP17(d,a,b) ((d = (a)[0] - (b)[0]) || \
	(d = (a)[4] - (b)[4]))
#define CMP18(d,a,b) ((d = (a)[1] - (b)[1]) || \
	(d = (a)[4] - (b)[4]))
#define CMP19(d,a,b) ((d = (a)[0] - (b)[0]) || \
	(d = (a)[1] - (b)[1]) || \
	(d = (a)[4] - (b)[4]))
#define CMP20(d,a,b) ((d = (a)[2] - (b)[2]) || \
	(d = (a)[4] - (b)[4]))
#define CMP21(d,a,b) ((d = (a)[0] - (b)[0]) || \
	(d = (a)[2] - (b)[2]) || \
	(d = (a)[4] - (b)[4]))
#define CMP22(d,a,b) ((d = (a)[1] - (b)[1]) || \
	(d = (a)[2] - (b)[2]) || \
	(d = (a)[4] - (b)[4]))
#define CMP23(d,a,b) ((d = (a)[0] - (b)[0]) || \
	(d = (a)[1] - (b)[1]) || \
	(d = (a)[2] - (b)[2]) || \
	(d = (a)[4] - (b)[4]))
#define CMP24(d,a,b) ((d = (a)[3] - (b)[3]) || \
	(d = (a)[4] - (b)[4]))
#define CMP25(d,a,b) ((d = (a)[0] - (b)[0]) || \
	(d = (a)[3] - (b)[3]) || \
	(d = (a)[4] - (b)[4]))
#define CMP26(d,a,b) ((d = (a)[1] - (b)[1]) || \
	(d = (a)[3] - (b)[3]) || \
	(d = (a)[4] - (b)[4]))
#define CMP27(d,a,b) ((d = (a)[0] - (b)[0]) || \
	(d = (a)[1] - (b)[1]) || \
	(d = (a)[3] - (b)[3]) || \
	(d = (a)[4] - (b)[4]))
#define CMP28(d,a,b) ((d = (a)[2] - (b)[2]) || \
	(d = (a)[3] - (b)[3]) || \
	(d = (a)[4] - (b)[4]))
#define CMP29(d,a,b) ((d = (a)[0] - (b)[0]) || \
	(d = (a)[2] - (b)[2]) || \
	(d = (a)[3] - (b)[3]) || \
	(d = (a)[4] - (b)[4]))
#define CMP30(d,a,b) ((d = (a)[1] - (b)[1]) || \
	(d = (a)[2] - (b)[2]) || \
	(d = (a)[3] - (b)[3]) || \
	(d = (a)[4] - (b)[4]))

static void fit0(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[0];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (0 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf("fit0 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("  hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = fitn[0 | (1 << hardest)];
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[0].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[0][hardest] = word[0];
	    sq->row[hardest][0] = word[0];
	    sq->row[1][hardest] = word[1];
	    sq->row[hardest][1] = word[1];
	    sq->row[2][hardest] = word[2];
	    sq->row[hardest][2] = word[2];
	    sq->row[3][hardest] = word[3];
	    sq->row[hardest][3] = word[3];
	    sq->row[4][hardest] = word[4];
	    sq->row[hardest][4] = word[4];

	    fit_next(sq);
	}
	sq->row[0][hardest] = '.';
	sq->row[hardest][0] = '.';
	sq->row[1][hardest] = '.';
	sq->row[hardest][1] = '.';
	sq->row[2][hardest] = '.';
	sq->row[hardest][2] = '.';
	sq->row[3][hardest] = '.';
	sq->row[hardest][3] = '.';
	sq->row[4][hardest] = '.';
	sq->row[hardest][4] = '.';

    }
}

static void fit1(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[1];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (1 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    while (first < last1) {
		int diff;
		int try = (first+last1) / 2;
		CMP1(diff, dict->words[try], sq->row[r]);
		if (diff < 0) {
		    first = try+1;
		} else if (diff > 0) {
		    last1 = try;
		} else {
		    int n1 = try, n2 = try+1;
		    while (first < n1) {
			try = (first + n1)/2;
			CMP1(diff, dict->words[try], sq->row[r]);
			if (diff < 0) {
			    first = try+1;
			} else {
			    n1 = try;
			}
		    }
		    while (n2 < last1) {
			try = (n2 + last1)/2;
			CMP1(diff, dict->words[try], sq->row[r]);
			if (diff == 0) {
			    n2 = try+1;
			} else {
			    last1 = try;
			}
		    }
		    break;
		}
	    }

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf(" fit1 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("   hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = fitn[1 | (1 << hardest)];
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[1].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[1][hardest] = word[1];
	    sq->row[hardest][1] = word[1];
	    sq->row[2][hardest] = word[2];
	    sq->row[hardest][2] = word[2];
	    sq->row[3][hardest] = word[3];
	    sq->row[hardest][3] = word[3];
	    sq->row[4][hardest] = word[4];
	    sq->row[hardest][4] = word[4];

	    fit_next(sq);
	}
	sq->row[1][hardest] = '.';
	sq->row[hardest][1] = '.';
	sq->row[2][hardest] = '.';
	sq->row[hardest][2] = '.';
	sq->row[3][hardest] = '.';
	sq->row[hardest][3] = '.';
	sq->row[4][hardest] = '.';
	sq->row[hardest][4] = '.';

    }
}

static void fit2(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[2];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (2 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    while (first < last1) {
		int diff;
		int try = (first+last1) / 2;
		CMP2(diff, dict->words[try], sq->row[r]);
		if (diff < 0) {
		    first = try+1;
		} else if (diff > 0) {
		    last1 = try;
		} else {
		    int n1 = try, n2 = try+1;
		    while (first < n1) {
			try = (first + n1)/2;
			CMP2(diff, dict->words[try], sq->row[r]);
			if (diff < 0) {
			    first = try+1;
			} else {
			    n1 = try;
			}
		    }
		    while (n2 < last1) {
			try = (n2 + last1)/2;
			CMP2(diff, dict->words[try], sq->row[r]);
			if (diff == 0) {
			    n2 = try+1;
			} else {
			    last1 = try;
			}
		    }
		    break;
		}
	    }

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf(" fit2 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("   hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = fitn[2 | (1 << hardest)];
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[2].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[0][hardest] = word[0];
	    sq->row[hardest][0] = word[0];
	    sq->row[2][hardest] = word[2];
	    sq->row[hardest][2] = word[2];
	    sq->row[3][hardest] = word[3];
	    sq->row[hardest][3] = word[3];
	    sq->row[4][hardest] = word[4];
	    sq->row[hardest][4] = word[4];

	    fit_next(sq);
	}
	sq->row[0][hardest] = '.';
	sq->row[hardest][0] = '.';
	sq->row[2][hardest] = '.';
	sq->row[hardest][2] = '.';
	sq->row[3][hardest] = '.';
	sq->row[hardest][3] = '.';
	sq->row[4][hardest] = '.';
	sq->row[hardest][4] = '.';

    }
}

static void fit3(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[3];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (3 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    while (first < last1) {
		int diff;
		int try = (first+last1) / 2;
		CMP3(diff, dict->words[try], sq->row[r]);
		if (diff < 0) {
		    first = try+1;
		} else if (diff > 0) {
		    last1 = try;
		} else {
		    int n1 = try, n2 = try+1;
		    while (first < n1) {
			try = (first + n1)/2;
			CMP3(diff, dict->words[try], sq->row[r]);
			if (diff < 0) {
			    first = try+1;
			} else {
			    n1 = try;
			}
		    }
		    while (n2 < last1) {
			try = (n2 + last1)/2;
			CMP3(diff, dict->words[try], sq->row[r]);
			if (diff == 0) {
			    n2 = try+1;
			} else {
			    last1 = try;
			}
		    }
		    break;
		}
	    }

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf("  fit3 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("    hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = fitn[3 | (1 << hardest)];
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[3].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[2][hardest] = word[2];
	    sq->row[hardest][2] = word[2];
	    sq->row[3][hardest] = word[3];
	    sq->row[hardest][3] = word[3];
	    sq->row[4][hardest] = word[4];
	    sq->row[hardest][4] = word[4];

	    fit_next(sq);
	}
	sq->row[2][hardest] = '.';
	sq->row[hardest][2] = '.';
	sq->row[3][hardest] = '.';
	sq->row[hardest][3] = '.';
	sq->row[4][hardest] = '.';
	sq->row[hardest][4] = '.';

    }
}

static void fit4(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[4];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (4 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    while (first < last1) {
		int diff;
		int try = (first+last1) / 2;
		CMP4(diff, dict->words[try], sq->row[r]);
		if (diff < 0) {
		    first = try+1;
		} else if (diff > 0) {
		    last1 = try;
		} else {
		    int n1 = try, n2 = try+1;
		    while (first < n1) {
			try = (first + n1)/2;
			CMP4(diff, dict->words[try], sq->row[r]);
			if (diff < 0) {
			    first = try+1;
			} else {
			    n1 = try;
			}
		    }
		    while (n2 < last1) {
			try = (n2 + last1)/2;
			CMP4(diff, dict->words[try], sq->row[r]);
			if (diff == 0) {
			    n2 = try+1;
			} else {
			    last1 = try;
			}
		    }
		    break;
		}
	    }

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf(" fit4 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("   hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = fitn[4 | (1 << hardest)];
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[4].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[0][hardest] = word[0];
	    sq->row[hardest][0] = word[0];
	    sq->row[1][hardest] = word[1];
	    sq->row[hardest][1] = word[1];
	    sq->row[3][hardest] = word[3];
	    sq->row[hardest][3] = word[3];
	    sq->row[4][hardest] = word[4];
	    sq->row[hardest][4] = word[4];

	    fit_next(sq);
	}
	sq->row[0][hardest] = '.';
	sq->row[hardest][0] = '.';
	sq->row[1][hardest] = '.';
	sq->row[hardest][1] = '.';
	sq->row[3][hardest] = '.';
	sq->row[hardest][3] = '.';
	sq->row[4][hardest] = '.';
	sq->row[hardest][4] = '.';

    }
}

static void fit5(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[5];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (5 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    while (first < last1) {
		int diff;
		int try = (first+last1) / 2;
		CMP5(diff, dict->words[try], sq->row[r]);
		if (diff < 0) {
		    first = try+1;
		} else if (diff > 0) {
		    last1 = try;
		} else {
		    int n1 = try, n2 = try+1;
		    while (first < n1) {
			try = (first + n1)/2;
			CMP5(diff, dict->words[try], sq->row[r]);
			if (diff < 0) {
			    first = try+1;
			} else {
			    n1 = try;
			}
		    }
		    while (n2 < last1) {
			try = (n2 + last1)/2;
			CMP5(diff, dict->words[try], sq->row[r]);
			if (diff == 0) {
			    n2 = try+1;
			} else {
			    last1 = try;
			}
		    }
		    break;
		}
	    }

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf("  fit5 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("    hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = fitn[5 | (1 << hardest)];
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[5].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[1][hardest] = word[1];
	    sq->row[hardest][1] = word[1];
	    sq->row[3][hardest] = word[3];
	    sq->row[hardest][3] = word[3];
	    sq->row[4][hardest] = word[4];
	    sq->row[hardest][4] = word[4];

	    fit_next(sq);
	}
	sq->row[1][hardest] = '.';
	sq->row[hardest][1] = '.';
	sq->row[3][hardest] = '.';
	sq->row[hardest][3] = '.';
	sq->row[4][hardest] = '.';
	sq->row[hardest][4] = '.';

    }
}

static void fit6(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[6];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (6 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    while (first < last1) {
		int diff;
		int try = (first+last1) / 2;
		CMP6(diff, dict->words[try], sq->row[r]);
		if (diff < 0) {
		    first = try+1;
		} else if (diff > 0) {
		    last1 = try;
		} else {
		    int n1 = try, n2 = try+1;
		    while (first < n1) {
			try = (first + n1)/2;
			CMP6(diff, dict->words[try], sq->row[r]);
			if (diff < 0) {
			    first = try+1;
			} else {
			    n1 = try;
			}
		    }
		    while (n2 < last1) {
			try = (n2 + last1)/2;
			CMP6(diff, dict->words[try], sq->row[r]);
			if (diff == 0) {
			    n2 = try+1;
			} else {
			    last1 = try;
			}
		    }
		    break;
		}
	    }

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf("  fit6 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("    hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = fitn[6 | (1 << hardest)];
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[6].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[0][hardest] = word[0];
	    sq->row[hardest][0] = word[0];
	    sq->row[3][hardest] = word[3];
	    sq->row[hardest][3] = word[3];
	    sq->row[4][hardest] = word[4];
	    sq->row[hardest][4] = word[4];

	    fit_next(sq);
	}
	sq->row[0][hardest] = '.';
	sq->row[hardest][0] = '.';
	sq->row[3][hardest] = '.';
	sq->row[hardest][3] = '.';
	sq->row[4][hardest] = '.';
	sq->row[hardest][4] = '.';

    }
}

static void fit7(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[7];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (7 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    while (first < last1) {
		int diff;
		int try = (first+last1) / 2;
		CMP7(diff, dict->words[try], sq->row[r]);
		if (diff < 0) {
		    first = try+1;
		} else if (diff > 0) {
		    last1 = try;
		} else {
		    int n1 = try, n2 = try+1;
		    while (first < n1) {
			try = (first + n1)/2;
			CMP7(diff, dict->words[try], sq->row[r]);
			if (diff < 0) {
			    first = try+1;
			} else {
			    n1 = try;
			}
		    }
		    while (n2 < last1) {
			try = (n2 + last1)/2;
			CMP7(diff, dict->words[try], sq->row[r]);
			if (diff == 0) {
			    n2 = try+1;
			} else {
			    last1 = try;
			}
		    }
		    break;
		}
	    }

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf("   fit7 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("     hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = fitn[7 | (1 << hardest)];
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[7].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[3][hardest] = word[3];
	    sq->row[hardest][3] = word[3];
	    sq->row[4][hardest] = word[4];
	    sq->row[hardest][4] = word[4];

	    fit_next(sq);
	}
	sq->row[3][hardest] = '.';
	sq->row[hardest][3] = '.';
	sq->row[4][hardest] = '.';
	sq->row[hardest][4] = '.';

    }
}

static void fit8(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[8];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (8 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    while (first < last1) {
		int diff;
		int try = (first+last1) / 2;
		CMP8(diff, dict->words[try], sq->row[r]);
		if (diff < 0) {
		    first = try+1;
		} else if (diff > 0) {
		    last1 = try;
		} else {
		    int n1 = try, n2 = try+1;
		    while (first < n1) {
			try = (first + n1)/2;
			CMP8(diff, dict->words[try], sq->row[r]);
			if (diff < 0) {
			    first = try+1;
			} else {
			    n1 = try;
			}
		    }
		    while (n2 < last1) {
			try = (n2 + last1)/2;
			CMP8(diff, dict->words[try], sq->row[r]);
			if (diff == 0) {
			    n2 = try+1;
			} else {
			    last1 = try;
			}
		    }
		    break;
		}
	    }

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf(" fit8 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("   hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = fitn[8 | (1 << hardest)];
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[8].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[0][hardest] = word[0];
	    sq->row[hardest][0] = word[0];
	    sq->row[1][hardest] = word[1];
	    sq->row[hardest][1] = word[1];
	    sq->row[2][hardest] = word[2];
	    sq->row[hardest][2] = word[2];
	    sq->row[4][hardest] = word[4];
	    sq->row[hardest][4] = word[4];

	    fit_next(sq);
	}
	sq->row[0][hardest] = '.';
	sq->row[hardest][0] = '.';
	sq->row[1][hardest] = '.';
	sq->row[hardest][1] = '.';
	sq->row[2][hardest] = '.';
	sq->row[hardest][2] = '.';
	sq->row[4][hardest] = '.';
	sq->row[hardest][4] = '.';

    }
}

static void fit9(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[9];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (9 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    while (first < last1) {
		int diff;
		int try = (first+last1) / 2;
		CMP9(diff, dict->words[try], sq->row[r]);
		if (diff < 0) {
		    first = try+1;
		} else if (diff > 0) {
		    last1 = try;
		} else {
		    int n1 = try, n2 = try+1;
		    while (first < n1) {
			try = (first + n1)/2;
			CMP9(diff, dict->words[try], sq->row[r]);
			if (diff < 0) {
			    first = try+1;
			} else {
			    n1 = try;
			}
		    }
		    while (n2 < last1) {
			try = (n2 + last1)/2;
			CMP9(diff, dict->words[try], sq->row[r]);
			if (diff == 0) {
			    n2 = try+1;
			} else {
			    last1 = try;
			}
		    }
		    break;
		}
	    }

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf("  fit9 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("    hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = fitn[9 | (1 << hardest)];
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[9].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[1][hardest] = word[1];
	    sq->row[hardest][1] = word[1];
	    sq->row[2][hardest] = word[2];
	    sq->row[hardest][2] = word[2];
	    sq->row[4][hardest] = word[4];
	    sq->row[hardest][4] = word[4];

	    fit_next(sq);
	}
	sq->row[1][hardest] = '.';
	sq->row[hardest][1] = '.';
	sq->row[2][hardest] = '.';
	sq->row[hardest][2] = '.';
	sq->row[4][hardest] = '.';
	sq->row[hardest][4] = '.';

    }
}

static void fit10(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[10];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (10 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    while (first < last1) {
		int diff;
		int try = (first+last1) / 2;
		CMP10(diff, dict->words[try], sq->row[r]);
		if (diff < 0) {
		    first = try+1;
		} else if (diff > 0) {
		    last1 = try;
		} else {
		    int n1 = try, n2 = try+1;
		    while (first < n1) {
			try = (first + n1)/2;
			CMP10(diff, dict->words[try], sq->row[r]);
			if (diff < 0) {
			    first = try+1;
			} else {
			    n1 = try;
			}
		    }
		    while (n2 < last1) {
			try = (n2 + last1)/2;
			CMP10(diff, dict->words[try], sq->row[r]);
			if (diff == 0) {
			    n2 = try+1;
			} else {
			    last1 = try;
			}
		    }
		    break;
		}
	    }

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf("  fit10 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("    hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = fitn[10 | (1 << hardest)];
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[10].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[0][hardest] = word[0];
	    sq->row[hardest][0] = word[0];
	    sq->row[2][hardest] = word[2];
	    sq->row[hardest][2] = word[2];
	    sq->row[4][hardest] = word[4];
	    sq->row[hardest][4] = word[4];

	    fit_next(sq);
	}
	sq->row[0][hardest] = '.';
	sq->row[hardest][0] = '.';
	sq->row[2][hardest] = '.';
	sq->row[hardest][2] = '.';
	sq->row[4][hardest] = '.';
	sq->row[hardest][4] = '.';

    }
}

static void fit11(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[11];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (11 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    while (first < last1) {
		int diff;
		int try = (first+last1) / 2;
		CMP11(diff, dict->words[try], sq->row[r]);
		if (diff < 0) {
		    first = try+1;
		} else if (diff > 0) {
		    last1 = try;
		} else {
		    int n1 = try, n2 = try+1;
		    while (first < n1) {
			try = (first + n1)/2;
			CMP11(diff, dict->words[try], sq->row[r]);
			if (diff < 0) {
			    first = try+1;
			} else {
			    n1 = try;
			}
		    }
		    while (n2 < last1) {
			try = (n2 + last1)/2;
			CMP11(diff, dict->words[try], sq->row[r]);
			if (diff == 0) {
			    n2 = try+1;
			} else {
			    last1 = try;
			}
		    }
		    break;
		}
	    }

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf("   fit11 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("     hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = fitn[11 | (1 << hardest)];
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[11].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[2][hardest] = word[2];
	    sq->row[hardest][2] = word[2];
	    sq->row[4][hardest] = word[4];
	    sq->row[hardest][4] = word[4];

	    fit_next(sq);
	}
	sq->row[2][hardest] = '.';
	sq->row[hardest][2] = '.';
	sq->row[4][hardest] = '.';
	sq->row[hardest][4] = '.';

    }
}

static void fit12(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[12];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (12 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    while (first < last1) {
		int diff;
		int try = (first+last1) / 2;
		CMP12(diff, dict->words[try], sq->row[r]);
		if (diff < 0) {
		    first = try+1;
		} else if (diff > 0) {
		    last1 = try;
		} else {
		    int n1 = try, n2 = try+1;
		    while (first < n1) {
			try = (first + n1)/2;
			CMP12(diff, dict->words[try], sq->row[r]);
			if (diff < 0) {
			    first = try+1;
			} else {
			    n1 = try;
			}
		    }
		    while (n2 < last1) {
			try = (n2 + last1)/2;
			CMP12(diff, dict->words[try], sq->row[r]);
			if (diff == 0) {
			    n2 = try+1;
			} else {
			    last1 = try;
			}
		    }
		    break;
		}
	    }

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf("  fit12 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("    hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = fitn[12 | (1 << hardest)];
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[12].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[0][hardest] = word[0];
	    sq->row[hardest][0] = word[0];
	    sq->row[1][hardest] = word[1];
	    sq->row[hardest][1] = word[1];
	    sq->row[4][hardest] = word[4];
	    sq->row[hardest][4] = word[4];

	    fit_next(sq);
	}
	sq->row[0][hardest] = '.';
	sq->row[hardest][0] = '.';
	sq->row[1][hardest] = '.';
	sq->row[hardest][1] = '.';
	sq->row[4][hardest] = '.';
	sq->row[hardest][4] = '.';

    }
}

static void fit13(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[13];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (13 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    while (first < last1) {
		int diff;
		int try = (first+last1) / 2;
		CMP13(diff, dict->words[try], sq->row[r]);
		if (diff < 0) {
		    first = try+1;
		} else if (diff > 0) {
		    last1 = try;
		} else {
		    int n1 = try, n2 = try+1;
		    while (first < n1) {
			try = (first + n1)/2;
			CMP13(diff, dict->words[try], sq->row[r]);
			if (diff < 0) {
			    first = try+1;
			} else {
			    n1 = try;
			}
		    }
		    while (n2 < last1) {
			try = (n2 + last1)/2;
			CMP13(diff, dict->words[try], sq->row[r]);
			if (diff == 0) {
			    n2 = try+1;
			} else {
			    last1 = try;
			}
		    }
		    break;
		}
	    }

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf("   fit13 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("     hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = fitn[13 | (1 << hardest)];
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[13].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[1][hardest] = word[1];
	    sq->row[hardest][1] = word[1];
	    sq->row[4][hardest] = word[4];
	    sq->row[hardest][4] = word[4];

	    fit_next(sq);
	}
	sq->row[1][hardest] = '.';
	sq->row[hardest][1] = '.';
	sq->row[4][hardest] = '.';
	sq->row[hardest][4] = '.';

    }
}

static void fit14(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[14];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (14 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    while (first < last1) {
		int diff;
		int try = (first+last1) / 2;
		CMP14(diff, dict->words[try], sq->row[r]);
		if (diff < 0) {
		    first = try+1;
		} else if (diff > 0) {
		    last1 = try;
		} else {
		    int n1 = try, n2 = try+1;
		    while (first < n1) {
			try = (first + n1)/2;
			CMP14(diff, dict->words[try], sq->row[r]);
			if (diff < 0) {
			    first = try+1;
			} else {
			    n1 = try;
			}
		    }
		    while (n2 < last1) {
			try = (n2 + last1)/2;
			CMP14(diff, dict->words[try], sq->row[r]);
			if (diff == 0) {
			    n2 = try+1;
			} else {
			    last1 = try;
			}
		    }
		    break;
		}
	    }

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf("   fit14 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("     hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = fitn[14 | (1 << hardest)];
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[14].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[0][hardest] = word[0];
	    sq->row[hardest][0] = word[0];
	    sq->row[4][hardest] = word[4];
	    sq->row[hardest][4] = word[4];

	    fit_next(sq);
	}
	sq->row[0][hardest] = '.';
	sq->row[hardest][0] = '.';
	sq->row[4][hardest] = '.';
	sq->row[hardest][4] = '.';

    }
}

static void fit15(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[15];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (15 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    while (first < last1) {
		int diff;
		int try = (first+last1) / 2;
		CMP15(diff, dict->words[try], sq->row[r]);
		if (diff < 0) {
		    first = try+1;
		} else if (diff > 0) {
		    last1 = try;
		} else {
		    int n1 = try, n2 = try+1;
		    while (first < n1) {
			try = (first + n1)/2;
			CMP15(diff, dict->words[try], sq->row[r]);
			if (diff < 0) {
			    first = try+1;
			} else {
			    n1 = try;
			}
		    }
		    while (n2 < last1) {
			try = (n2 + last1)/2;
			CMP15(diff, dict->words[try], sq->row[r]);
			if (diff == 0) {
			    n2 = try+1;
			} else {
			    last1 = try;
			}
		    }
		    break;
		}
	    }

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf("    fit15 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("      hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = solution;
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[15].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[4][hardest] = word[4];
	    sq->row[hardest][4] = word[4];

	    fit_next(sq);
	}
	sq->row[4][hardest] = '.';
	sq->row[hardest][4] = '.';

    }
}

static void fit16(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[16];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (16 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    while (first < last1) {
		int diff;
		int try = (first+last1) / 2;
		CMP16(diff, dict->words[try], sq->row[r]);
		if (diff < 0) {
		    first = try+1;
		} else if (diff > 0) {
		    last1 = try;
		} else {
		    int n1 = try, n2 = try+1;
		    while (first < n1) {
			try = (first + n1)/2;
			CMP16(diff, dict->words[try], sq->row[r]);
			if (diff < 0) {
			    first = try+1;
			} else {
			    n1 = try;
			}
		    }
		    while (n2 < last1) {
			try = (n2 + last1)/2;
			CMP16(diff, dict->words[try], sq->row[r]);
			if (diff == 0) {
			    n2 = try+1;
			} else {
			    last1 = try;
			}
		    }
		    break;
		}
	    }

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf(" fit16 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("   hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = fitn[16 | (1 << hardest)];
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[16].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[0][hardest] = word[0];
	    sq->row[hardest][0] = word[0];
	    sq->row[1][hardest] = word[1];
	    sq->row[hardest][1] = word[1];
	    sq->row[2][hardest] = word[2];
	    sq->row[hardest][2] = word[2];
	    sq->row[3][hardest] = word[3];
	    sq->row[hardest][3] = word[3];

	    fit_next(sq);
	}
	sq->row[0][hardest] = '.';
	sq->row[hardest][0] = '.';
	sq->row[1][hardest] = '.';
	sq->row[hardest][1] = '.';
	sq->row[2][hardest] = '.';
	sq->row[hardest][2] = '.';
	sq->row[3][hardest] = '.';
	sq->row[hardest][3] = '.';

    }
}

static void fit17(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[17];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (17 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    while (first < last1) {
		int diff;
		int try = (first+last1) / 2;
		CMP17(diff, dict->words[try], sq->row[r]);
		if (diff < 0) {
		    first = try+1;
		} else if (diff > 0) {
		    last1 = try;
		} else {
		    int n1 = try, n2 = try+1;
		    while (first < n1) {
			try = (first + n1)/2;
			CMP17(diff, dict->words[try], sq->row[r]);
			if (diff < 0) {
			    first = try+1;
			} else {
			    n1 = try;
			}
		    }
		    while (n2 < last1) {
			try = (n2 + last1)/2;
			CMP17(diff, dict->words[try], sq->row[r]);
			if (diff == 0) {
			    n2 = try+1;
			} else {
			    last1 = try;
			}
		    }
		    break;
		}
	    }

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf("  fit17 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("    hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = fitn[17 | (1 << hardest)];
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[17].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[1][hardest] = word[1];
	    sq->row[hardest][1] = word[1];
	    sq->row[2][hardest] = word[2];
	    sq->row[hardest][2] = word[2];
	    sq->row[3][hardest] = word[3];
	    sq->row[hardest][3] = word[3];

	    fit_next(sq);
	}
	sq->row[1][hardest] = '.';
	sq->row[hardest][1] = '.';
	sq->row[2][hardest] = '.';
	sq->row[hardest][2] = '.';
	sq->row[3][hardest] = '.';
	sq->row[hardest][3] = '.';

    }
}

static void fit18(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[18];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (18 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    while (first < last1) {
		int diff;
		int try = (first+last1) / 2;
		CMP18(diff, dict->words[try], sq->row[r]);
		if (diff < 0) {
		    first = try+1;
		} else if (diff > 0) {
		    last1 = try;
		} else {
		    int n1 = try, n2 = try+1;
		    while (first < n1) {
			try = (first + n1)/2;
			CMP18(diff, dict->words[try], sq->row[r]);
			if (diff < 0) {
			    first = try+1;
			} else {
			    n1 = try;
			}
		    }
		    while (n2 < last1) {
			try = (n2 + last1)/2;
			CMP18(diff, dict->words[try], sq->row[r]);
			if (diff == 0) {
			    n2 = try+1;
			} else {
			    last1 = try;
			}
		    }
		    break;
		}
	    }

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf("  fit18 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("    hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = fitn[18 | (1 << hardest)];
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[18].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[0][hardest] = word[0];
	    sq->row[hardest][0] = word[0];
	    sq->row[2][hardest] = word[2];
	    sq->row[hardest][2] = word[2];
	    sq->row[3][hardest] = word[3];
	    sq->row[hardest][3] = word[3];

	    fit_next(sq);
	}
	sq->row[0][hardest] = '.';
	sq->row[hardest][0] = '.';
	sq->row[2][hardest] = '.';
	sq->row[hardest][2] = '.';
	sq->row[3][hardest] = '.';
	sq->row[hardest][3] = '.';

    }
}

static void fit19(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[19];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (19 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    while (first < last1) {
		int diff;
		int try = (first+last1) / 2;
		CMP19(diff, dict->words[try], sq->row[r]);
		if (diff < 0) {
		    first = try+1;
		} else if (diff > 0) {
		    last1 = try;
		} else {
		    int n1 = try, n2 = try+1;
		    while (first < n1) {
			try = (first + n1)/2;
			CMP19(diff, dict->words[try], sq->row[r]);
			if (diff < 0) {
			    first = try+1;
			} else {
			    n1 = try;
			}
		    }
		    while (n2 < last1) {
			try = (n2 + last1)/2;
			CMP19(diff, dict->words[try], sq->row[r]);
			if (diff == 0) {
			    n2 = try+1;
			} else {
			    last1 = try;
			}
		    }
		    break;
		}
	    }

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf("   fit19 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("     hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = fitn[19 | (1 << hardest)];
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[19].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[2][hardest] = word[2];
	    sq->row[hardest][2] = word[2];
	    sq->row[3][hardest] = word[3];
	    sq->row[hardest][3] = word[3];

	    fit_next(sq);
	}
	sq->row[2][hardest] = '.';
	sq->row[hardest][2] = '.';
	sq->row[3][hardest] = '.';
	sq->row[hardest][3] = '.';

    }
}

static void fit20(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[20];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (20 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    while (first < last1) {
		int diff;
		int try = (first+last1) / 2;
		CMP20(diff, dict->words[try], sq->row[r]);
		if (diff < 0) {
		    first = try+1;
		} else if (diff > 0) {
		    last1 = try;
		} else {
		    int n1 = try, n2 = try+1;
		    while (first < n1) {
			try = (first + n1)/2;
			CMP20(diff, dict->words[try], sq->row[r]);
			if (diff < 0) {
			    first = try+1;
			} else {
			    n1 = try;
			}
		    }
		    while (n2 < last1) {
			try = (n2 + last1)/2;
			CMP20(diff, dict->words[try], sq->row[r]);
			if (diff == 0) {
			    n2 = try+1;
			} else {
			    last1 = try;
			}
		    }
		    break;
		}
	    }

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf("  fit20 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("    hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = fitn[20 | (1 << hardest)];
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[20].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[0][hardest] = word[0];
	    sq->row[hardest][0] = word[0];
	    sq->row[1][hardest] = word[1];
	    sq->row[hardest][1] = word[1];
	    sq->row[3][hardest] = word[3];
	    sq->row[hardest][3] = word[3];

	    fit_next(sq);
	}
	sq->row[0][hardest] = '.';
	sq->row[hardest][0] = '.';
	sq->row[1][hardest] = '.';
	sq->row[hardest][1] = '.';
	sq->row[3][hardest] = '.';
	sq->row[hardest][3] = '.';

    }
}

static void fit21(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[21];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (21 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    while (first < last1) {
		int diff;
		int try = (first+last1) / 2;
		CMP21(diff, dict->words[try], sq->row[r]);
		if (diff < 0) {
		    first = try+1;
		} else if (diff > 0) {
		    last1 = try;
		} else {
		    int n1 = try, n2 = try+1;
		    while (first < n1) {
			try = (first + n1)/2;
			CMP21(diff, dict->words[try], sq->row[r]);
			if (diff < 0) {
			    first = try+1;
			} else {
			    n1 = try;
			}
		    }
		    while (n2 < last1) {
			try = (n2 + last1)/2;
			CMP21(diff, dict->words[try], sq->row[r]);
			if (diff == 0) {
			    n2 = try+1;
			} else {
			    last1 = try;
			}
		    }
		    break;
		}
	    }

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf("   fit21 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("     hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = fitn[21 | (1 << hardest)];
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[21].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[1][hardest] = word[1];
	    sq->row[hardest][1] = word[1];
	    sq->row[3][hardest] = word[3];
	    sq->row[hardest][3] = word[3];

	    fit_next(sq);
	}
	sq->row[1][hardest] = '.';
	sq->row[hardest][1] = '.';
	sq->row[3][hardest] = '.';
	sq->row[hardest][3] = '.';

    }
}

static void fit22(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[22];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (22 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    while (first < last1) {
		int diff;
		int try = (first+last1) / 2;
		CMP22(diff, dict->words[try], sq->row[r]);
		if (diff < 0) {
		    first = try+1;
		} else if (diff > 0) {
		    last1 = try;
		} else {
		    int n1 = try, n2 = try+1;
		    while (first < n1) {
			try = (first + n1)/2;
			CMP22(diff, dict->words[try], sq->row[r]);
			if (diff < 0) {
			    first = try+1;
			} else {
			    n1 = try;
			}
		    }
		    while (n2 < last1) {
			try = (n2 + last1)/2;
			CMP22(diff, dict->words[try], sq->row[r]);
			if (diff == 0) {
			    n2 = try+1;
			} else {
			    last1 = try;
			}
		    }
		    break;
		}
	    }

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf("   fit22 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("     hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = fitn[22 | (1 << hardest)];
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[22].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[0][hardest] = word[0];
	    sq->row[hardest][0] = word[0];
	    sq->row[3][hardest] = word[3];
	    sq->row[hardest][3] = word[3];

	    fit_next(sq);
	}
	sq->row[0][hardest] = '.';
	sq->row[hardest][0] = '.';
	sq->row[3][hardest] = '.';
	sq->row[hardest][3] = '.';

    }
}

static void fit23(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[23];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (23 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    while (first < last1) {
		int diff;
		int try = (first+last1) / 2;
		CMP23(diff, dict->words[try], sq->row[r]);
		if (diff < 0) {
		    first = try+1;
		} else if (diff > 0) {
		    last1 = try;
		} else {
		    int n1 = try, n2 = try+1;
		    while (first < n1) {
			try = (first + n1)/2;
			CMP23(diff, dict->words[try], sq->row[r]);
			if (diff < 0) {
			    first = try+1;
			} else {
			    n1 = try;
			}
		    }
		    while (n2 < last1) {
			try = (n2 + last1)/2;
			CMP23(diff, dict->words[try], sq->row[r]);
			if (diff == 0) {
			    n2 = try+1;
			} else {
			    last1 = try;
			}
		    }
		    break;
		}
	    }

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf("    fit23 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("      hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = solution;
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[23].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[3][hardest] = word[3];
	    sq->row[hardest][3] = word[3];

	    fit_next(sq);
	}
	sq->row[3][hardest] = '.';
	sq->row[hardest][3] = '.';

    }
}

static void fit24(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[24];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (24 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    while (first < last1) {
		int diff;
		int try = (first+last1) / 2;
		CMP24(diff, dict->words[try], sq->row[r]);
		if (diff < 0) {
		    first = try+1;
		} else if (diff > 0) {
		    last1 = try;
		} else {
		    int n1 = try, n2 = try+1;
		    while (first < n1) {
			try = (first + n1)/2;
			CMP24(diff, dict->words[try], sq->row[r]);
			if (diff < 0) {
			    first = try+1;
			} else {
			    n1 = try;
			}
		    }
		    while (n2 < last1) {
			try = (n2 + last1)/2;
			CMP24(diff, dict->words[try], sq->row[r]);
			if (diff == 0) {
			    n2 = try+1;
			} else {
			    last1 = try;
			}
		    }
		    break;
		}
	    }

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf("  fit24 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("    hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = fitn[24 | (1 << hardest)];
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[24].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[0][hardest] = word[0];
	    sq->row[hardest][0] = word[0];
	    sq->row[1][hardest] = word[1];
	    sq->row[hardest][1] = word[1];
	    sq->row[2][hardest] = word[2];
	    sq->row[hardest][2] = word[2];

	    fit_next(sq);
	}
	sq->row[0][hardest] = '.';
	sq->row[hardest][0] = '.';
	sq->row[1][hardest] = '.';
	sq->row[hardest][1] = '.';
	sq->row[2][hardest] = '.';
	sq->row[hardest][2] = '.';

    }
}

static void fit25(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[25];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (25 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    while (first < last1) {
		int diff;
		int try = (first+last1) / 2;
		CMP25(diff, dict->words[try], sq->row[r]);
		if (diff < 0) {
		    first = try+1;
		} else if (diff > 0) {
		    last1 = try;
		} else {
		    int n1 = try, n2 = try+1;
		    while (first < n1) {
			try = (first + n1)/2;
			CMP25(diff, dict->words[try], sq->row[r]);
			if (diff < 0) {
			    first = try+1;
			} else {
			    n1 = try;
			}
		    }
		    while (n2 < last1) {
			try = (n2 + last1)/2;
			CMP25(diff, dict->words[try], sq->row[r]);
			if (diff == 0) {
			    n2 = try+1;
			} else {
			    last1 = try;
			}
		    }
		    break;
		}
	    }

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf("   fit25 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("     hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = fitn[25 | (1 << hardest)];
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[25].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[1][hardest] = word[1];
	    sq->row[hardest][1] = word[1];
	    sq->row[2][hardest] = word[2];
	    sq->row[hardest][2] = word[2];

	    fit_next(sq);
	}
	sq->row[1][hardest] = '.';
	sq->row[hardest][1] = '.';
	sq->row[2][hardest] = '.';
	sq->row[hardest][2] = '.';

    }
}

static void fit26(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[26];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (26 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    while (first < last1) {
		int diff;
		int try = (first+last1) / 2;
		CMP26(diff, dict->words[try], sq->row[r]);
		if (diff < 0) {
		    first = try+1;
		} else if (diff > 0) {
		    last1 = try;
		} else {
		    int n1 = try, n2 = try+1;
		    while (first < n1) {
			try = (first + n1)/2;
			CMP26(diff, dict->words[try], sq->row[r]);
			if (diff < 0) {
			    first = try+1;
			} else {
			    n1 = try;
			}
		    }
		    while (n2 < last1) {
			try = (n2 + last1)/2;
			CMP26(diff, dict->words[try], sq->row[r]);
			if (diff == 0) {
			    n2 = try+1;
			} else {
			    last1 = try;
			}
		    }
		    break;
		}
	    }

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf("   fit26 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("     hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = fitn[26 | (1 << hardest)];
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[26].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[0][hardest] = word[0];
	    sq->row[hardest][0] = word[0];
	    sq->row[2][hardest] = word[2];
	    sq->row[hardest][2] = word[2];

	    fit_next(sq);
	}
	sq->row[0][hardest] = '.';
	sq->row[hardest][0] = '.';
	sq->row[2][hardest] = '.';
	sq->row[hardest][2] = '.';

    }
}

static void fit27(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[27];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (27 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    while (first < last1) {
		int diff;
		int try = (first+last1) / 2;
		CMP27(diff, dict->words[try], sq->row[r]);
		if (diff < 0) {
		    first = try+1;
		} else if (diff > 0) {
		    last1 = try;
		} else {
		    int n1 = try, n2 = try+1;
		    while (first < n1) {
			try = (first + n1)/2;
			CMP27(diff, dict->words[try], sq->row[r]);
			if (diff < 0) {
			    first = try+1;
			} else {
			    n1 = try;
			}
		    }
		    while (n2 < last1) {
			try = (n2 + last1)/2;
			CMP27(diff, dict->words[try], sq->row[r]);
			if (diff == 0) {
			    n2 = try+1;
			} else {
			    last1 = try;
			}
		    }
		    break;
		}
	    }

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf("    fit27 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("      hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = solution;
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[27].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[2][hardest] = word[2];
	    sq->row[hardest][2] = word[2];

	    fit_next(sq);
	}
	sq->row[2][hardest] = '.';
	sq->row[hardest][2] = '.';

    }
}

static void fit28(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[28];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (28 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    while (first < last1) {
		int diff;
		int try = (first+last1) / 2;
		CMP28(diff, dict->words[try], sq->row[r]);
		if (diff < 0) {
		    first = try+1;
		} else if (diff > 0) {
		    last1 = try;
		} else {
		    int n1 = try, n2 = try+1;
		    while (first < n1) {
			try = (first + n1)/2;
			CMP28(diff, dict->words[try], sq->row[r]);
			if (diff < 0) {
			    first = try+1;
			} else {
			    n1 = try;
			}
		    }
		    while (n2 < last1) {
			try = (n2 + last1)/2;
			CMP28(diff, dict->words[try], sq->row[r]);
			if (diff == 0) {
			    n2 = try+1;
			} else {
			    last1 = try;
			}
		    }
		    break;
		}
	    }

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf("   fit28 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("     hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = fitn[28 | (1 << hardest)];
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[28].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[0][hardest] = word[0];
	    sq->row[hardest][0] = word[0];
	    sq->row[1][hardest] = word[1];
	    sq->row[hardest][1] = word[1];

	    fit_next(sq);
	}
	sq->row[0][hardest] = '.';
	sq->row[hardest][0] = '.';
	sq->row[1][hardest] = '.';
	sq->row[hardest][1] = '.';

    }
}

static void fit29(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[29];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (29 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    while (first < last1) {
		int diff;
		int try = (first+last1) / 2;
		CMP29(diff, dict->words[try], sq->row[r]);
		if (diff < 0) {
		    first = try+1;
		} else if (diff > 0) {
		    last1 = try;
		} else {
		    int n1 = try, n2 = try+1;
		    while (first < n1) {
			try = (first + n1)/2;
			CMP29(diff, dict->words[try], sq->row[r]);
			if (diff < 0) {
			    first = try+1;
			} else {
			    n1 = try;
			}
		    }
		    while (n2 < last1) {
			try = (n2 + last1)/2;
			CMP29(diff, dict->words[try], sq->row[r]);
			if (diff == 0) {
			    n2 = try+1;
			} else {
			    last1 = try;
			}
		    }
		    break;
		}
	    }

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf("    fit29 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("      hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = solution;
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[29].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[1][hardest] = word[1];
	    sq->row[hardest][1] = word[1];

	    fit_next(sq);
	}
	sq->row[1][hardest] = '.';
	sq->row[hardest][1] = '.';

    }
}

static void fit30(Square *sq)
{
    int r;
    Dict *dict = &shuffled_dict[30];
    int hardest = -1, hardest_count = 1000000, hardest_first = 0;
    for (r=0; r<5; r++) {
	if (! (30 & (1 << r))) {
	    int first, last1, count;
	    first = 0;
	    last1 = dict->nword;

	    while (first < last1) {
		int diff;
		int try = (first+last1) / 2;
		CMP30(diff, dict->words[try], sq->row[r]);
		if (diff < 0) {
		    first = try+1;
		} else if (diff > 0) {
		    last1 = try;
		} else {
		    int n1 = try, n2 = try+1;
		    while (first < n1) {
			try = (first + n1)/2;
			CMP30(diff, dict->words[try], sq->row[r]);
			if (diff < 0) {
			    first = try+1;
			} else {
			    n1 = try;
			}
		    }
		    while (n2 < last1) {
			try = (n2 + last1)/2;
			CMP30(diff, dict->words[try], sq->row[r]);
			if (diff == 0) {
			    n2 = try+1;
			} else {
			    last1 = try;
			}
		    }
		    break;
		}
	    }

	    count = last1 - first;

	    if (count == 0) return;
	    if (count < hardest_count) {
		hardest = r;
		hardest_count = count;
		hardest_first = first;
	    }
	}
    }

    if (debug) {
	printf("    fit30 %.5s %.5s %.5s %.5s %.5s\n",
	       sq->row[0], sq->row[1], sq->row[2], sq->row[3], sq->row[4]);
	printf("      hardest=%d match=%.5s, count=%d first=%.5s last=%.5s\n",
	       hardest, sq->row[hardest], hardest_count,
	       dict->words[hardest_first],
	       dict->words[hardest_first + hardest_count-1]);
    }
    {
	fit_func fit_next = solution;
	int t;
	for (t=0; t<hardest_count; t++) {
	    const char *word = shuffled_dict[30].words[hardest_first + t];
	    /* memcpy(sq->row[hardest], word, 5); */
	    sq->row[0][hardest] = word[0];
	    sq->row[hardest][0] = word[0];

	    fit_next(sq);
	}
	sq->row[0][hardest] = '.';
	sq->row[hardest][0] = '.';

    }
}


static fit_func fitn[32] = {
  &fit0,
  &fit1,
  &fit2,
  &fit3,
  &fit4,
  &fit5,
  &fit6,
  &fit7,
  &fit8,
  &fit9,
  &fit10,
  &fit11,
  &fit12,
  &fit13,
  &fit14,
  &fit15,
  &fit16,
  &fit17,
  &fit18,
  &fit19,
  &fit20,
  &fit21,
  &fit22,
  &fit23,
  &fit24,
  &fit25,
  &fit26,
  &fit27,
  &fit28,
  &fit29,
  &fit30,
  NULL,
};
