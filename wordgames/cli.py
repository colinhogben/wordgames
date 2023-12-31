from .d2 import Dict
import argparse
import os
import sys

def main():
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
    m_anag.add_argument('-l','--length', type=int,
                        help='Length (if not using all letters)')
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
        for word in d2.anagram(args.word, length=args.length):
            print(word)
    else:
        raise NotImplementedError(method)

if __name__=='__main__':
    main()
