# Shamelessly adapted from python-astar

from abc import ABC, abstractmethod
from typing import Callable, Dict, Iterable, Union, TypeVar, Generic
from math import inf as infinity
import sortedcontainers  # type: ignore

# introduce generic type
T = TypeVar("T")


################################################################################
class SearchNode(Generic[T]):
    """Representation of a search node"""

    __slots__ = ("data", "gscore", "fscore", "closed", "came_from", "allcomers", "in_openset")

    def __init__(
        self, data: T, gscore: float = infinity, fscore: float = infinity
    ) -> None:
        self.data = data
        self.gscore = gscore
        self.fscore = fscore
        self.closed = False
        self.in_openset = False
        self.came_from: Union[None, SearchNode[T]] = None
        self.allcomers = []

    def __lt__(self, b: "SearchNode[T]") -> bool:
        """Natural order is based on the fscore value & is used by heapq operations"""
        return self.fscore < b.fscore


################################################################################
class SearchNodeDict(Dict[T, SearchNode[T]]):
    """A dict that returns a new SearchNode when a key is missing"""

    def __missing__(self, k) -> SearchNode[T]:
        v = SearchNode(k)
        self.__setitem__(k, v)
        return v


################################################################################
SNType = TypeVar("SNType", bound=SearchNode)


class OpenSet(Generic[SNType]):
    def __init__(self) -> None:
        self.sortedlist = sortedcontainers.SortedList(key=lambda x: x.fscore)

    def push(self, item: SNType) -> None:
        item.in_openset = True
        self.sortedlist.add(item)

    def pop(self) -> SNType:
        item = self.sortedlist.pop(0)
        item.in_openset = False
        return item

    def remove(self, item: SNType) -> None:
        self.sortedlist.remove(item)
        item.in_openset = False

    def __len__(self) -> int:
        return len(self.sortedlist)


################################################################################*


class AStar(ABC, Generic[T]):
    __slots__ = ()

    @abstractmethod
    def heuristic_cost_estimate(self, current: T, goal: T) -> float:
        """
        Computes the estimated (rough) distance between a node and the goal.
        The second parameter is always the goal.
        This method must be implemented in a subclass.
        """
        raise NotImplementedError

    @abstractmethod
    def distance_between(self, n1: T, n2: T) -> float:
        """
        Gives the real distance between two adjacent nodes n1 and n2 (i.e n2
        belongs to the list of n1's neighbors).
        n2 is guaranteed to belong to the list returned by the call to neighbors(n1).
        This method must be implemented in a subclass.
        """

    @abstractmethod
    def neighbors(self, node: T) -> Iterable[T]:
        """
        For a given node, returns (or yields) the list of its neighbors.
        This method must be implemented in a subclass.
        """
        raise NotImplementedError

    def is_goal_reached(self, current: T, goal: T) -> bool:
        """
        Returns true when we can consider that 'current' is the goal.
        The default implementation simply compares `current == goal`, but this
        method can be overwritten in a subclass to provide more refined checks.
        """
        return current == goal

    def reconstruct_path(self, last: SearchNode, reversePath=False) -> Iterable[T]:
        def _gen():
            current = last
            while current:
                yield current.data
                current = current.came_from

        if reversePath:
            return _gen()
        else:
            return reversed(list(_gen()))

    def extract_graph(self, last):
        graph = []
        seen = set()
        def _extract(level, node):
            if node.data in seen:
                return
            seen.add(node.data)
            graph.append((level, node.data,
                          [prev.data for prev in node.allcomers]))
            for prev in node.allcomers:
                _extract(level+1, prev)
        _extract(0, last)
        return graph

    def astar(
            self, start: T, goal: T, reversePath: bool = False, graph=False,
    ) -> Union[Iterable[T], None]:
        if self.is_goal_reached(start, goal):
            return [start]

        openSet: OpenSet[SearchNode[T]] = OpenSet()
        searchNodes: SearchNodeDict[T] = SearchNodeDict()
        startNode = searchNodes[start] = SearchNode(
            start, gscore=0.0, fscore=self.heuristic_cost_estimate(start, goal)
        )
        openSet.push(startNode)

        while openSet:
            current = openSet.pop()

            if self.is_goal_reached(current.data, goal):
                if graph:
                    return self.extract_graph(current)
                else:
                    return self.reconstruct_path(current, reversePath)

            current.closed = True

            for neighbor in map(lambda n: searchNodes[n], self.neighbors(current.data)):
                if neighbor.closed:
                    continue

                tentative_gscore = current.gscore + self.distance_between(
                    current.data, neighbor.data
                )

                if tentative_gscore > neighbor.gscore:
                    continue
                if tentative_gscore == neighbor.gscore:
                    neighbor.allcomers.append(current)
                    continue

                neighbor_from_openset = neighbor.in_openset

                if neighbor_from_openset:
                    # we have to remove the item from the heap, as its score has changed
                    openSet.remove(neighbor)

                # update the node
                neighbor.came_from = current
                neighbor.allcomers = [current]
                neighbor.gscore = tentative_gscore
                neighbor.fscore = tentative_gscore + self.heuristic_cost_estimate(
                    neighbor.data, goal
                )

                openSet.push(neighbor)

        return None


class Node:
    def __init__(self, graph, name, items=None):
        self.graph = graph
        self.name = name
        if items is None:
            items = name.split('\n')
        self.items = items
        # Origin (top left), updated by layout
        self.ox = 0
        self.oy = 0
        # Linke
        self.prev = []
        self.next = []
        # Derived
        self.length = len(self.items)
        self.maxchars = max([len(item) for item in self.items])
        self.width = self.maxchars * self.graph.charwidth + 2 * graph.padding
        self.height = self.length * self.graph.charheight + 2 * graph.padding

    @property
    def midtop(self):
        return (self.ox + self.width//2, self.oy)

    @property
    def midbot(self):
        return (self.ox + self.width//2, self.oy + self.height)

class Row:
    def __init__(self, nodes):
        self.nodes = list(nodes)

    @property
    def width(self):
        rhs = self.nodes[-1]
        return rhs.ox + rhs.width - self.nodes[0].ox

    @property
    def height(self):
        return max([node.height for node in self.nodes])

class Graph:
    def __init__(self, charheight=17, charwidth=12, padding=2, hgap=10, vgap=20):
        self.charheight = charheight
        self.charwidth = charwidth
        self.padding = padding
        self.hgap = hgap
        self.vgap = vgap
        self.rows = []          # [Row...]
        self.by_id = {}         # name -> node

    @property
    def width(self):
        return max([row.width for row in self.rows]) + 2 * self.hgap

    @property
    def height(self):
        last_row = self.rows[-1]
        return last_row.nodes[0].oy + last_row.height + 2 * self.vgap

    def add_node(self, rowindex, name, items=None):
        while len(self.rows) <= rowindex:
            self.rows.append(Row([]))
        node = Node(self, name, items)
        self.rows[rowindex].nodes.append(node)
        self.by_id[name] = node
        return node

    def find_node(self, name):
        return self.by_id[name]

    def add_link(self, start, end):
        startnode = self.find_node(start)
        endnode = self.find_node(end)
        startnode.next.append(endnode)
        endnode.prev.append(startnode)

    def layout_left(self):
        oy = self.vgap
        for row in self.rows:
            ox = self.hgap
            for node in row.nodes:
                node.ox, node.oy = ox, oy
                ox += node.width + self.hgap
            oy += max([node.height for node in row.nodes]) + self.vgap

    def layout_centre(self):
        self.layout_left()
        widths = [row.width for row in self.rows]
        maxwidth = max(widths)
        for row, width in zip(self.rows, widths):
            shift = (maxwidth - width) // 2
            for node in row.nodes:
                node.ox += shift
