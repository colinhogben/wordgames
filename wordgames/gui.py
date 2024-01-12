from tkinter import *
from functools import partial

class GraphView(Canvas):
    def __init__(self, parent, graph, *args, width=800, height=600, on_click=None, **kw):
        Canvas.__init__(self, parent, *args, width=width, height=height, **kw)
        self.graph = graph
        self.on_click = on_click
        self.nodes = []
        self.links = []
        self._populate()

    def _populate(self):
        for row in self.graph.rows:
            for node in row.nodes:
                frm = Frame(self,
                            width=node.width, height=node.height,
                            bg='white')
                frm.pack_propagate(False) # Don't shrink to fit
                lab = Label(frm, text='\n'.join(node.items),
                            background='yellow')
                lab.bind('<1>', partial(self._click, node))
                lab.pack()
                win = self.create_window(node.ox, node.oy,
                                         tags=('node',),
                                         window=frm, anchor=NW)
                self.nodes.append(win)
                self.update()
                for prev in node.prev:
                    link = self.create_line((prev.midbot, node.midtop),
                                            arrow='last',
                                            tags=('link',))
                    self.links.append(link)
        self.configure(width=self.graph.width,
                       height=self.graph.height)

    def set_graph(self, graph):
        """Replace with another graph"""
        # Clear existing items
        self.delete('node')
        self.nodes = []
        self.delete('link')
        self.links = []
        # Insert the new graph
        self.graph = graph
        self._populate()

    def _click(self, node, event):
        if self.on_click:
            self.on_click(node)
