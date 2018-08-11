# future and other python packages
from __future__ import unicode_literals
import sys
import os
import random
# PyQt5 packages
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.Qt import QMutex
# Matplotlib
import matplotlib
matplotlib.use('Qt5Agg')
from numpy import arange, sin, pi
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT as NavigationToolbar
from matplotlib.figure import Figure

# LibUSB
import usb.core
import usb.util

# 
SUCCESS = "<font color=green>"
FAILURE = "<font color=red>"
NORMAL = "<font color=black>"
ATTENTION = "<font color=blue>"

DEVICE_VOLTAGE_MAX = 5000   # mV
DEVICE_VOLTAGE_MIN = -5000
DEVICE_CURRENT_MAX = 500    # mA
DEVICE_CURRENT_MIN = -500
PELTIER_CURRENT_MAX = 40    # 100 mA

# data packet
DATA_PACKET_START = 0x99
DATA_PACKET_END = 0x00

# instructin packet
INSTR_PACKET_START = 0x66
INSTR_PACKET_END = 0x00

PERIOIC_DATA_REQUEST_TEMP_LOOP = 0x01
INIT_PERIPHERALS = 0x03


# Periodic Data Request Constants 
PELT_CURRENT_REQ = 0x80
CURR_TEMP_REQ = 0x40
PELT_VOLTAGE_REQ = 0x20
isSTABLE_REQ = 0x10
SETPOINT_TEMP_REQ = 0x08
SETPOINT_CURRENT_REQ = 0x40
SETPOINT_VOLTAGE_REQ = 0x20

#init peripherals 
CURR_SENSOR_INIT = 0x80
TEMP_SENSOR_INIT = 0x40

# PARAMETER_SET
SET_PELT_VOLTAGE = 0x12
SET_PELT_CURRENT = 0x11
SET_TEMPERATURE = 0x10

# Data Packet Sysm

SETPOINT_DEV_V_DATA = 0x11
SETPOINT_DEV_I_DATA = 0x12
SETPOINT_TEMP_DATA = 0x13
isSTABLE_DATA = 0x14
PELT_VOLTAGE_DATA = 0x15
PELT_CURR_DATA = 0x17
CURR_TEMP_DATA = 0x16











