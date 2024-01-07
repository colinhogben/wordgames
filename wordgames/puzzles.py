from .alg import AStar, Graph
from collections import Counter

empty_set = set()

class WordAStar(AStar):
    """Shortest-path finder for word ladders"""
    def __init__(self, d2dict, exclude=empty_set):
        self.d2dict = d2dict
        self.exclude = exclude

    def distance_between(self, n1, n2):
        return 1

    def heuristic_cost_estimate(self, current, goal):
        return len([True for c,g in zip(current, goal) if c != g])

    def neighbors(self, node):
        for i, c in enumerate(node):
            for n in 'ABCDEFGHIJKLMNOPQRSTUVWXYZ':
                if n != c:
                    word = node[:i] + n + node[i+1:]
                    if word in self.d2dict and word not in self.exclude:
                        yield word

class Bag(Counter):
    def string(self):
        return ''.join(sorted(self.elements()))

    def __hash__(self):
        return hash(self.string())

    def distance(self, other):
        sum = 0
        for c in self:
            dif = other[c] - self[c]
            if dif > 0:
                sum += dif
        return sum

class AnagramAStar(AStar):
    def __init__(self, d2dict, wlen, exclude=empty_set):
        # Get pool neighbours
        self.pool_neighbours = d2dict.aneighbour_map(wlen)
        self.exclude = exclude

    def distance_between(self, n1, n2):
        return 1

    def heuristic_cost_estimate(self, current, goal):
        return Bag(current).distance(Bag(goal))

    def neighbors(self, node):
        for pool in  self.pool_neighbours.get(node, []):
            if pool not in self.exclude:
                yield pool

def word_ladder_graph(d2dict, word1, word2, exclude=empty_set):
    assert len(word1) == len(word2)
    wlen = len(word1)
    wa = WordAStar(d2dict, exclude=exclude)
    graph_data = wa.astar(word2, word1, graph=True)
    graph = Graph()
    for level, word, links in graph_data:
        graph.add_node(level, word, [word])
    for level, word, links in graph_data:
        for link in links:
            graph.add_link(word, link)
    return graph

def anagram_ladder_graph(d2dict, word1, word2, exclude=empty_set):
    assert len(word1) == len(word2)
    wlen = len(word1)
    pool1 = Bag(word1).string()
    pool2 = Bag(word2).string()
    aa = AnagramAStar(d2dict, wlen, exclude=exclude)
    graph_data = aa.astar(pool2, pool1, graph=True)
    graph = Graph()
    amap = d2dict.anagram_map(wlen)
    for level, pool, links in graph_data:
         # Known start and end
        if pool == pool1:
            words = [word1]
        elif pool == pool2:
            words = [word2]
        else:
            words = amap[pool]
        graph.add_node(level, pool, words)
    for level, pool, links in graph_data:
        for link in links:
            graph.add_link(pool, link)
    return graph

def print_graph(graph):
    next_map = {}
    for row in graph.rows:
        for node in row.nodes:
            next_map[node.name] = node.next
    def _do(level, node):
        print('%s%s' % ('  ' * level,
                        '/'.join(node.items)))
        for nnode in node.next:
            _do(level+1, nnode)
    _do(0, graph.rows[0].nodes[0])
