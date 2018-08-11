from defines import *

class GraphCanvas(FigureCanvas):
    def __init__(self, parent = None, width = 10, height = 4, dpi = 100):
        figure = Figure(figsize = (width,height),dpi=dpi)
        self.axes = figure.add_subplot(111)  # only one plot
        self.axes.clear()
        self.compute_initial_figure()
        FigureCanvas.__init__(self,figure)
        self.setParent(parent)
        FigureCanvas.setSizePolicy(self,QtWidgets.QSizePolicy.Expanding,
                QtWidgets.QSizePolicy.Expanding)
        FigureCanvas.updateGeometry(self)
        timer = QtCore.QTimer(self)
        timer.timeout.connect(self.update_figure)
        timer.start(1000)
    
    def compute_initial_figure(self):
        t = arange(0.0, 3.0, 0.01)
        s = sin(2*pi*t)
        self.axes.plot(t, s)
        
        
    def update_figure(self):
        l = [random.randint(0, 10) for i in range(4)]
        self.axes.clear()
        self.axes.plot([0, 1, 2, 3], l, 'r')
        self.draw()
