from defines import *
plt.ion()
class IVUpdate():
    #Suppose we know the x range
    min_x = -10
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
                self.lines1, = self.ax.plot([],[], '.r', label = "Temperature : " + str(temperature))
            elif self.i == 2:
                self.lines2, = self.ax.plot([],[], '.b', label = "Temperature : " + str(temperature))
            elif self.i == 3:
                self.lines3, = self.ax.plot([],[], '.g', label = "Temperature : " + str(temperature))
            elif self.i == 4:
                self.lines4, = self.ax.plot([],[], '.c', label = "Temperature : " + str(temperature))
            elif self.i == 5:
                self.lines5, = self.ax.plot([],[], '.m', label = "Temperature : " + str(temperature))
            elif self.i == 6:
                self.lines6, = self.ax.plot([],[], '.y', label = "Temperature : " + str(temperature))
            plt.legend(loc = 'upper left')
            
        #hl.set_xdata(numpy.append(hl.get_xdata(), new_data))
        if self.i == 1 :
            self.lines1.set_xdata(numpy.append(self.lines1.get_xdata(), xdata))
            self.lines1.set_ydata(numpy.append(self.lines1.get_ydata(), ydata))
        elif self.i == 2:
            self.lines2.set_xdata(numpy.append(self.lines2.get_xdata(), xdata))
            self.lines2.set_ydata(numpy.append(self.lines2.get_ydata(), ydata))
        elif self.i == 3:
            self.lines3.set_xdata(numpy.append(self.lines3.get_ydata(), xdata))
            self.lines3.set_ydata(numpy.append(self.lines3.get_ydata(), ydata))
        elif self.i == 4:    
            self.lines4.set_xdata(numpy.append(self.lines4.get_ydata(), xdata))
            self.lines4.set_ydata(numpy.append(self.lines4.get_ydata(), ydata))
        elif self.i == 5:    
            self.lines5.set_xdata(numpy.append(self.lines5.get_ydata(), xdata))
            self.lines5.set_ydata(numpy.append(self.lines5.get_ydata(), ydata))
        elif self.i == 6:    
            self.lines6.set_xdata(numpy.append(self.lines6.get_ydata(), xdata))
            self.lines6.set_ydata(numpy.append(self.lines6.get_ydata(), ydata))
        
        #Need both of these in order to rescale
        self.ax.relim()
        self.ax.autoscale_view()
        #We need to draw *and* flush
        self.figure.canvas.draw()
        self.figure.canvas.flush_events()