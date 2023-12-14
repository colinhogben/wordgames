CC = cc
LD = cc
CFLAGS = -g -Wall

BINDIR = .

APPS   = anag boggle caesar d2test dcomp dmatch evens countdown \
        find1 find2 find3 \
	monotone palin palinx qback reverse rotate scrab \
	subst subtract useonly wordx woxrd xpalin xrev xword \
	fitx jumble1 wordsquare target useall

BINAPPS := $(addprefix $(BINDIR)/,$(APPS))
all:	$(BINAPPS)

clean:	FORCE
	$(dolines app,$(APPS),rm $(app).o)

veryclean0:	clean	FORCE
	$(dolines app,$(APPS),rm $(BINDIR)/$(app))

veryclean:	FORCE
	rm -f $(BINAPPS)

dmatch_LIBS = match.o
$(BINDIR)/dmatch: match.o

#--- Default rule for d2 dictionary-based apps
$(BINAPPS):	$(BINDIR)/%:	%.o d2.o mem.o
	$(LD) -o $@ $(LDFLAGS) $< d2.o mem.o $($*_LIBS)

$(addsuffix .o,$(APPS)):	%.o:	%.c
	$(CC) -c -o $@ $(CFLAGS) $<

d2make: d2make.c d2write.c msort.o mem.o

#--
palind : palind.o
	$(LD) -o palind palind.o $(LIB)

lsplit : lsplit.o
	$(LD) -o lsplit lsplit.o $(LIB)

makednl : makednl.o
	$(LD) -o makednl makednl.o $(LIB)

ccount : ccount.o
	$(LD) -o ccount ccount.o $(LIB)

makedict : makedict.o
	$(LD) -o makedict makedict.o $(LIB)

ckboggle : o.ckboggle
	link -o ckboggle o.ckboggle ${LIB}

o.ckboggle : c.ckboggle
	cc -c -o o.ckboggle c.ckboggle

dictsubs : dictsubs.o
	$(LD) -o dictsubs dictsubs.o $(LIB)

dnl.o : dnl.c
	$(CC) -c $(CFLAGS) dnl.c $(CPATH)

elim : c.elim
	$(CC) -o $@ $^ $(LIB)

ws5.c:	gen_ws
	gen_ws 5 > $@

.PHONY:	FORCE
