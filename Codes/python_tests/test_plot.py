from temp_plot import *

import timeit
temperaturePlot = TempUpdate()
temperaturePlot._init_()
temperaturePlot.on_launch()
start = timeit.default_timer()
for i in range(100):
    x = [i,i+1,i+2,i+3,i+4]
    y = [i,i,i,i,i]
    temperaturePlot.on_running(x,y)
print((timeit.default_timer()-start)/500)