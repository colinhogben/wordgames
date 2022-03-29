#=======================================================================
"""
Native implementation of D2 words
"""
#=======================================================================
from __future__ import print_function
from collections import OrderedDict
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
            return (word in dl)
        except KeyError:
            return False

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
            ht = []
            for i in range(nhead):
                b2, tend = struct.unpack('<2s 2x I', f.read(8))
                h2 = btoa(b2)
                if not h2.isalpha():
                    raise ValueError('%s: Non-alpha head %r' %
                                     (filename, h2))
                ht.append((h2, tend))
            self.sect = OrderedDict()
            sofar = 0
            for h2, tend in ht:
                tails = [btoa(f.read(self.length - 2))
                         for i in range(tend - sofar)]
                sofar = tend
                self.sect[h2] = tails

    def dump(self):
        for h2, tails in self.sect.items():
            for tail in tails:
                print(h2 + tail)

    def __contains__(self, word):
        try:
            tails = self.sect[word[:2]]
            # FIXME binary search
            return bsearch(tails, word[2:]) is not None
            #return (word[2:] in tails)
        except KeyError:
            return False

class Dscan:
    """Iterate over words"""
    def __init__(self, d2, length):
        self.dl = d2.dictl(length)
        self.heads = self.dl.sect.keys()
        self.tails = None
        # Indexes
        self.hix = 0
        self._get_sect()

    def _get_sect(self):
        self.head = self.heads[self.hix]
        self.tails = self.dl.sect[self.head]
        self.tix = 0

    def read(self):
        if self.tix >= len(self.tails):
            self.hix += 1
            if self.hix >= len(self.heads):
                raise EOFError
            self._get_sect()
        word = self.head + self.tails[self.tix]
        self.tix += 1
        return word

    def skip(self, skip):
        raise NotImplementedError

class Dpool:
    """Pool of letters"""
    def __init__(self):
        self.nlet = [0] * 26
        self.nwild = 0

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
        try:
            ix = self._index(ch[:1])
            if self.nlet[ix] == 0:
                return False
            self.nlet[ix] -= 1
        except NotAlpha:
            if self.nwild == 0:
                return False
            self.nwild -= 1
        return True

    def as_string(self, wildch='.'):
        s = ''
        for i, n in enumerate(self.nlet):
            if n > 0:
                s += chr(ord('A')+i) * n
        s += wildch * self.nwild
        return s

    def _index(self, ch):
        if not ch.isalpha():
            raise NotAlpha
        return ord(ch.upper()) - ord('A')

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
    dictdir = os.path.expanduser('~/.local/share/moby')
    p = Dpool()
    p.add_word('ab??zya')
    print(p.as_string())
    d2 = Dict(dictdir)
    dl = d2.dictl(7)
    for word in ('', 'A', 'TEST', 'SHSHFSH', 'TESTING'):
        print(word, word in dl, word in d2)
    #dl.dump()
    scan = Dscan(d2, 7)
    while True:
        try:
            print(scan.read())
        except EOFError:
            break
