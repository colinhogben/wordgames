#=======================================================================
"""
Native implementation of D2 words
"""
#=======================================================================
from __future__ import print_function
import os
import struct

class NotAlpha(ValueError):
    pass

class Dict:
    """Wrapper for whole dictionary"""
    def __init__(self, stub):
        self.stub = stub
        self.by_len = {}

    def dictl(self, length):
        try:
            dl = self.by_len[length]
        except KeyError:
            filename = os.path.join(self.stub, '%02d' % length)
            if not os.path.exists(filename):
                raise KeyError('No words of length %d' % length)
            dl = self.by_len[length] = Dictl(length)
            dl.load(filename)
        return dl

    def __contains__(self, word):
        try:
            dl = self.dictl(len(word))
            return (word.upper() in dl)
        except KeyError:
            return False

    def anagram(self, letters):
        """Iterate over anagrams"""
        length = len(letters)
        pool = Dpool()
        pool.add_word(letters)
        for word in self.dictl(length):
            poolx = pool.clone()
            if poolx.sub_word(word):
                yield word

class Dictl:
    """Sub-dictionary for a particular length"""
    def __init__(self, length):
        self.length = length

    def load(self, filename):
        with open(filename, 'rb') as f:
            magic, wlen, nhead = struct.unpack('<2s H I', f.read(8))
            if magic != b'd2':
                raise ValueError('%s: Invalid file header' % filename)
            if wlen != self.length:
                raise ValueError('%s: Length %d, expected %d' %
                                 (filename, wlen, self.length))
            self.nhead = nhead
            self.heads = []
            tends = []
            for i in range(nhead):
                b2, tend = struct.unpack('<2s 2x I', f.read(8))
                h2 = btoa(b2)
                if not h2.isalpha():
                    raise ValueError('%s: Non-alpha head %r' %
                                     (filename, h2))
                self.heads.append(h2)
                tends.append(tend)
            self.tails = []
            sofar = 0
            for tend in tends:
                tails = [btoa(f.read(self.length - 2))
                         for i in range(tend - sofar)]
                sofar = tend
                self.tails.append(tails)

    def dump(self):
        for h2, tails in zip(self.heads, self.tails):
            for tail in tails:
                print(h2 + tail)

    def __contains__(self, word):
        hix = bsearch(self.heads, word[:2])
        if hix is None:
            return False
        tix = bsearch(self.tails[hix], word[2:])
        if tix is None:
            return False
        return True

    def __iter__(self):
        return Dscan(self)

class Dscan:
    """Iterate over words"""
    def __init__(self, dictl):
        self.dl = dictl
        # Indexes
        self.hix = 0
        self.tix = 0
        self.heads = self.dl.heads
        self.tails = None
        self._get_sect()

    def _get_sect(self):
        self.head = self.dl.heads[self.hix]
        self.tails = self.dl.tails[self.hix]
        self.tix = 0

    def read(self):
        if self.tix >= len(self.tails):
            self.hix += 1
            if self.hix >= len(self.dl.heads):
                raise EOFError
            self._get_sect()
        word = self.head + self.tails[self.tix]
        self.tix += 1
        return word

    def skip(self, skip):
        raise NotImplementedError

    def __next__(self):
        """Can use as an iterator"""
        try:
            word = self.read()
        except EOFError:
            raise StopIteration
        else:
            return word
    next = __next__             # PY2

class Dpool:
    """Pool of letters"""
    def __init__(self):
        self.nlet = [0] * 26
        self.nwild = 0

    def __repr__(self):
        return '<Dpool "%s">' % self.as_string()

    def __len__(self):
        n = self.nwild
        for i in self.nlet:
            n += i
        return n

    def clone(self):
        new = self.__class__()
        new.nlet = self.nlet[:]
        new.nwild = self.nwild
        return new

    def add(self, other):
        for i, n in enumerate(other.nlet):
            self.nlet[i] += n
        self.nwild += other.nwild

    def add_word(self, word):
        for i in range(len(word)):
            self.add_char(word[i:i+1])

    def add_char(self, ch):
        try:
            self.nlet[self._index(ch[:1])] += 1
        except NotAlpha:
            self.nwild += 1

    def add_wild(self):
        self.nwild += 1

    def sub_word(self, word):
        for i in range(len(word)):
            if not self.sub_char(word[i:i+1]):
                return False
        return True

    def sub_char(self, ch):
        ix = self._index(ch[:1])
        if self.nlet[ix] > 0:
            self.nlet[ix] -= 1
            return True
        elif self.nwild > 0:
            self.nwild -= 1
            return True
        return False

    def as_string(self, wildch='.'):
        s = ''
        for i, n in enumerate(self.nlet):
            if n > 0:
                s += chr(ord('A')+i) * n
        s += wildch * self.nwild
        return s

    def _index(self, ch):
        ich = ord(ch.upper()) - ord('A')
        if not 0 <= ich < 26:
            raise NotAlpha
        return ich

def iter_chars(word):
    """Iterate over characters in a word (PY2/3 compatible)"""
    for i in range(len(word)):
        yield word[i:i+1]

def btoa(b):
    return str(b.decode('ascii'))

def bsearch(lst, item):
    """Binary search for an item in an ordered list.
    Return index if found, else None
    """
    can = 0
    cant = len(lst)
    while cant - can > 0:
        mid = (can + cant) // 2
        tst = lst[mid]
        if item == tst:
            return mid
        elif item < tst:
            cant = mid
        else:
            can = mid+1
    return None

if __name__=='__main__':
    import argparse
    import sys
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest='method', metavar='method')
    #
    m_isword = sub.add_parser('is-word',
                              help='Check if a word is in the dictionary')
    m_isword.add_argument('word',
                          help='Word to check')
    #
    m_dump = sub.add_parser('dumpl',
			    help='Dump words for given length')
    m_dump.add_argument('length', type=int,
                        help='Word length')
    #
    m_anag = sub.add_parser('anag',
                            help='Find anagrams')
    m_anag.add_argument('word',
                        help='Word, maybe including wildcards')
    #
    args = ap.parse_args()
    dictdir = os.path.expanduser('~/.local/share/moby')
    d2 = Dict(dictdir)
    method = args.method
    if method is None:          # PY3 bug
        ap.error('No method')
    elif method == 'is-word':
        if args.word not in d2:
            print('"%s" is not in the dictionary' % args.word,
                  file=sys.stderr)
            sys.exit(1)
    elif method == 'dumpl':
        dl = d2.dictl(args.length)
        #dl.dump()
        for word in dl:
            print(word)
    elif method == 'anag':
        for word in d2.anagram(args.word):
            print(word)
    else:
        raise NotImplementedError(method)
