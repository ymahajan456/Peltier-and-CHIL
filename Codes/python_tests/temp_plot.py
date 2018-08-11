from defines import *
plt.ion()
class TempUpdate():
    #Suppose we know the x range
    min_y = -10
    max_y = 100
    
    def _init_(self):
        self.initialized = True

    def on_launch(self):
        #Set up plot
        self.figure, self.ax = plt.subplots()
        plt.ylabel("Temperature(C)")
        plt.xlabel("Time (s)")
        plt.title("Temperature Plot")
        #Autoscale on unknown axis and known lims on the other
        self.ax.set_autoscaley_on(True)
        self.ax.set_ylim(self.min_y, self.max_y)
        #Other stuff
        self.ax.grid()
        ...

    def on_running(self, xdata, ydata):
        #Update data (with the new _and_ the old points)
        if (self.initialized):
            self.lines, = self.ax.plot([],[], '-b')
            self.initialized = False
        self.getxdata = self.lines.get_xdata()
        self.getydata = self.lines.get_ydata()
        if (len(self.lines.get_xdata()) > 1001):
            del self.getxdata[0];
            del self.getydata[0];
        self.lines.set_xdata(numpy.append(self.getxdata, xdata))
        self.lines.set_ydata(numpy.append(self.getydata, ydata))
        #Need both of these in order to rescale
        self.ax.relim()
        self.ax.autoscale_view()
        #We need to draw *and* flush
        self.figure.canvas.draw()
        self.figure.canvas.flush_events()