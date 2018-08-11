from __future__ import unicode_literals
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.Qt import QMutex
#from copy import copy
import sys
import usb.core
import usb.util

SUCCESS = "<font color=green>"
FAILURE = "<font color=red>"
NORMAL = "<font color=black>"
ATTENTION = "<font color=blue>"

# future and other python packages
import sys
import os
import random
# PyQt5 packages
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.Qt import QMutex
# Matplotlib
import matplotlib
matplotlib.use('Qt5Agg')
import numpy
from numpy import arange, sin, pi
import matplotlib.pyplot as plt
#from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
#from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT as NavigationToolbar
#from matplotlib.figure import Figure

# LibUSB
import usb.core
import usb.util