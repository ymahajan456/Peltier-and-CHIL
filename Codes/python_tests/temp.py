import matplotlib.pyplot as plt
plt.ion()
class DynamicUpdate():
    #Suppose we know the x range
    min_x = 0
    max_x = 10
    
    def _init_(self):
        self.i = 0
        self.temp = -100

    def on_launch(self):
        #Set up plot
        self.figure, self.ax = plt.subplots()
        plt.ylabel("Current(mA)")
        plt.xlabel("Voltage(V)")
        plt.title("IV Characteristics Plot")
        #Autoscale on unknown axis and known lims on the other
        self.ax.set_autoscaley_on(True)
        self.ax.set_xlim(self.min_x, self.max_x)
        #Other stuff
        self.ax.grid()
        ...

    def on_running(self, xdata, ydata,temperature):
        #Update data (with the new _and_ the old points)
        if not(temperature == self.temp):
            self.i = self.i + 1;
            self.temp = temperature
            if self.i == 1:
                self.lines1, = self.ax.plot([],[], '-r', label = "Temperature : " + str(temperature))
            elif self.i == 2:
                self.lines2, = self.ax.plot([],[], '-b', label = "Temperature : " + str(temperature))
            elif self.i == 3:
                self.lines3, = self.ax.plot([],[], '-g', label = "Temperature : " + str(temperature))
            elif self.i == 4:
                self.lines4, = self.ax.plot([],[], '-c', label = "Temperature : " + str(temperature))
            elif self.i == 5:
                self.lines5, = self.ax.plot([],[], '-m', label = "Temperature : " + str(temperature))
            elif self.i == 6:
                self.lines6, = self.ax.plot([],[], '-y', label = "Temperature : " + str(temperature))
            plt.legend(loc = 'upper left')
            
        if self.i == 1 :
            self.lines1.set_xdata(xdata)
            self.lines1.set_ydata(ydata)
        elif self.i == 2:
            self.lines2.set_xdata(xdata)
            self.lines2.set_ydata(ydata)
        elif self.i == 3:
            self.lines3.set_xdata(xdata)
            self.lines3.set_ydata(ydata)
        elif self.i == 4:    
            self.lines4.set_xdata(xdata)
            self.lines4.set_ydata(ydata)
        elif self.i == 5:    
            self.lines5.set_xdata(xdata)
            self.lines5.set_ydata(ydata)
        elif self.i == 6:    
            self.lines6.set_xdata(xdata)
            self.lines6.set_ydata(ydata)
        
        #Need both of these in order to rescale
        self.ax.relim()
        self.ax.autoscale_view()
        #We need to draw *and* flush
        self.figure.canvas.draw()
        self.figure.canvas.flush_events()

    #Example
    def __call__(self):
        import numpy as np
        import time
        self._init_()
        self.on_launch()
        xdata = []
        ydata = []
        for x in np.arange(0,10,0.5):
            xdata.append(x)
            ydata.append(np.exp(-x**2)+10*np.exp(-(x-7)**2))
            self.on_running(xdata, ydata,1)
            time.sleep(1)
        xdata = []
        ydata = []
        for x in np.arange(0,10,0.5):
            xdata.append(x)
            ydata.append(np.exp(-x**2)+10*np.exp(-(x-6)**2))
            self.on_running(xdata, ydata,2)
            time.sleep(1)
        xdata = []
        ydata = []
        for x in np.arange(0,10,0.5):
            xdata.append(x)
            ydata.append(np.exp(-x**2)+10*np.exp(-(x-5)**2))
            self.on_running(xdata, ydata,3)
            time.sleep(1)
        xdata = []
        ydata = []
        for x in np.arange(0,10,0.5):
            xdata.append(x)
            ydata.append(np.exp(-x**2)+10*np.exp(-(x-4)**2))
            self.on_running(xdata, ydata,4)
            time.sleep(1)
        xdata = []
        ydata = []
        for x in np.arange(0,10,0.5):
            xdata.append(x)
            ydata.append(np.exp(-x**2)+10*np.exp(-(x-5.5)**2))
            self.on_running(xdata, ydata,5)
            time.sleep(1)
        return xdata, ydata

d = DynamicUpdate()
d()