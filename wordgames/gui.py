from tkinter import *

class GraphView(Canvas):
    def __init__(self, parent, graph, *args, width=800, height=600, **kw):
        Canvas.__init__(self, parent, *args, width=width, height=height, **kw)
        self.graph = graph
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
                lab.pack()
                win = self.create_window(node.ox, node.oy,
                                         window=frm, anchor=NW)
                self.update()
                for prev in node.prev:
                    link = self.create_line((prev.midbot, node.midtop),
                                            arrow='last')
        self.configure(width=self.graph.width,
                       height=self.graph.height)
