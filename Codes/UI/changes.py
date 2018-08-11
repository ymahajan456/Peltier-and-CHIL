#all the changes in Main file after auto generating UI again

#change includes

from defines import *
from graph_canvas import *
from USB_device import *

# functions to be added in UI class
    def connect_GUI(self):
        #self.qStartButton.clicked.connect(self.start_button_pressed)
        #self.qStopButton.clicked.connect(self.stop_button_pressed)
        #self.qPauseButton.clicked.connect(self.pause_button_pressed)
        self.qViewGraph.toggled.connect(self.qGraphWidget.setVisible)
        self.qGraphWidget.visibilityChanged.connect(self.qViewGraph.setChecked)
        self.qViewTerminal.toggled.connect(self.qTerminalWidget.setVisible)
        self.qTerminalWidget.visibilityChanged.connect(self.qViewTerminal.setChecked)
        self.fetch_thread = QtCore.QThread()
        self.device_connected = False
        self.qActionConnectDevice.triggered.connect(self.connect_device)
        self.qManPWMDApplyButton.clicked.connect(self.sendManualPWMDValue)
        self.setup_graph_widget()
        
    def setup_graph_widget(self):
        self.graph_canvas = GraphCanvas(parent = self.qGraphWidgetWindow)
        self.qGraphVLayout.addWidget(self.graph_canvas)
        self.qGraphWidget.toolbar = NavigationToolbar(self.graph_canvas, 
                self.qGraphWidgetWindow)
     
    def sendManualPWMDValue(self):
        if(self.device_connected):
            self.tivaC.send_data("abcd")
        
    def connect_device(self):
        if self.device_connected:
            if self.tivaC.connected:
                self.qButtonSend.clicked.disconnect(self.send_data)
                self.tivaC.disconnect_device()
                del self.tivaC
                self.device_connected = False
                self.device_terminal_write("Tive C disconnected",FAILURE)
                self.qConnectionLabel.setStyleSheet("color: rgb(255, 255, 255);\n"
                    "background-color: rgb(255, 0, 0);\n"
                    "border-color: rgb(255, 255, 255);")
                self.qConnectionLabel.setText("Disconnected")
        else:
            self.device_terminal_write("Attempting to connect ..")
            self.tivaC = USB_device(idVendor = 0x1cbe, idProduct = 0x0003)
            for retry in range(0,5):
                connected = self.tivaC.connect_device()
                if connected:
                    self.tivaC.moveToThread(self.fetch_thread)
                    self.tivaC.dataReceived.connect(self.handle_data)
                    self.fetch_thread.started.connect(self.tivaC.read_loop)
                    self.fetch_thread.start()
                    self.device_terminal_write("TIva C connected",SUCCESS)
                    self.qConnectionLabel.setStyleSheet("color: rgb(0, 0, 0);\n"
                        "background-color: rgb(0,255, 0);\n"
                        "border-color: rgb(0, 0, 0);")
                    self.qConnectionLabel.setText("Connected")
                    self.device_connected = True
                    break
                else:
                    self.device_terminal_write("Retrying ("+str(retry)+"/4)",NORMAL)
            if not self.tivaC.connected:
                self.device_terminal_write("Unable to connect",FAILURE)
    
    def handle_data(self,data):
        if self.tivaC.mutex.tryLock():
            new_data = [byte for byte in data]
            self.tivaC.primaryBuff = []
            #print("print\n"+str(self.tivaC.primaryBuff))
            self.tivaC.mutex.unlock()
            #processed_data = [chr(new_data[i]) for i in range(len(new_data))]
            self.device_terminal_write(" ".join(new_data))
            #self.browser_write(" ".join(str(ord(i)) for i in new_data))
            
    def device_terminal_write(self,message,type = NORMAL):
        self.qTerminalBrowser.append(type+message+"</font>\n")
                
                
# add connect_handles in setup function of UI class


