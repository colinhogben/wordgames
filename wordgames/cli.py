from .d2 import Dict
import argparse
import os
import sys

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('-D','--dict', default='moby',
                    help='Dictionary to use')
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
    m_ladder = sub.add_parser('ladder',
                              help='Find word ladder')
    m_ladder.add_argument('-g','--gui', action='store_true',
                           help='Display graphically')
    m_ladder.add_argument('-x','--exclude', action='append', default=[],
                          help='Word(s) to exclude')
    m_ladder.add_argument('word1')
    m_ladder.add_argument('word2')
    #
    m_aladder = sub.add_parser('aladder',
                               help='Find anagram ladder')
    m_aladder.add_argument('-g','--gui', action='store_true',
                           help='Display graphically')
    m_aladder.add_argument('-x','--exclude', action='append', default=[],
                           help='Word(s) to exclude')
    m_aladder.add_argument('word1')
    m_aladder.add_argument('word2')
    #
    m_anext = sub.add_parser('anext',
                             help='Find anagram neighbours')
    m_anext.add_argument('word')
    #
    args = ap.parse_args()
    dictdir = os.path.join(os.path.expanduser('~/.local/share'),
                           args.dict)
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
    elif method == 'ladder':
        from .puzzles import word_ladder_graph, print_graph
        word1 = args.word1.upper()
        word2 = args.word2.upper()
        exclude = exclusions(args.exclude, len(word1))
        graph = word_ladder_graph(d2, word1, word2, exclude=exclude)
        if args.gui:
            from .gui import Tk, GraphView
            graph.layout_centre()
            root = Tk()
            wgraph = GraphView(root, graph)
            wgraph.pack()
            root.mainloop()
        else:
            print_graph(graph)
    elif method == 'aladder':
        from .puzzles import anagram_ladder_graph, print_graph
        word1 = args.word1.upper()
        word2 = args.word2.upper()
        exclude = anagram_exclusions(args.exclude, len(word1))
        graph = anagram_ladder_graph(d2, word1, word2, exclude=exclude)
        if args.gui:
            from .gui import Tk, GraphView
            graph.layout_centre()
            root = Tk()

            def remove_node(node):
                """Exclude a node, recalculate and redraw"""
                exclude.add(make_pool(node.name))
                graph = anagram_ladder_graph(d2, word1, word2, exclude=exclude)
                graph.layout_centre()
                wgraph.set_graph(graph)

            wgraph = GraphView(root, graph, on_click=remove_node)
            wgraph.pack()
            root.mainloop()
        else:
            print_graph(graph)
    elif method == 'anext':
        word = args.word.upper()
        pool = make_pool(word)
        wlen = len(word)
        for pnext in d2.aneighbour_map(wlen).get(pool, []):
            for wnext in d2.anagram_map(wlen)[pnext]:
                print(wnext)
    else:
        raise NotImplementedError(method)

def exclusions(wlists, wlen):
    exclude = set()
    for wlist in wlists:
        for word in wlist.upper().split(','):
            if len(word) != wlen:
                raise ValueError('Word %s is not of length %d' % (word, wlen))
            exclude.add(word)
    return exclude

def anagram_exclusions(wlists, wlen):
    exclude = set()
    for wlist in wlists:
        for word in wlist.upper().split(','):
            if len(word) != wlen:
                raise ValueError('Word %s is not of length %d' % (word, wlen))
            exclude.add(make_pool(word))
    return exclude

def make_pool(word):
    return ''.join(sorted(word))

if __name__=='__main__':
    main()
