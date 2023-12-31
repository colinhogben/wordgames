from .alg import AStar, Graph
from collections import Counter

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
    def __init__(self, d2dict, wlen):
        # Get pool neighbours
        self.pool_neighbours = d2dict.aneighbour_map(wlen)

    def distance_between(self, n1, n2):
        return 1

    def heuristic_cost_estimate(self, current, goal):
        return Bag(current).distance(Bag(goal))

    def neighbors(self, node):
        return self.pool_neighbours.get(node, [])

def anagram_ladder_graph(d2dict, word1, word2):
    assert len(word1) == len(word2)
    wlen = len(word1)
    pool1 = Bag(word1).string()
    pool2 = Bag(word2).string()
    aa = AnagramAStar(d2dict, wlen)
    graph_data = aa.astar(pool2, pool1, graph=True)
    graph = Graph()
    amap = d2dict.anagram_map(wlen)
    for level, pool, links in graph_data:
        graph.add_node(level, pool, amap[pool])
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
