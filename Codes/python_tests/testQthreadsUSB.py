from defines import *
from USB_device import *
from IV_plot import * 
from temp_plot import *

def decode_iv_data(iv_data):
    v_array = []
    i_array = []
    while(len(iv_data)):
        voltage = ((ord(iv_data[0][1]) << 8) + ord(iv_data[0][2])) * 10 / 4095
        current = ((ord(iv_data[0][3]) << 8) + ord(iv_data[0][4])) * 500 / 4095 / 11
        v_array.append((-voltage) if ord(iv_data[0][0])  else (voltage))
        i_array.append((-current) if ord(iv_data[0][0]) else (current))
        del iv_data[0]
    return [v_array, i_array]
    

class Ui_Form(object):
    def setupUi(self, Form):
        Form.setObjectName("Form")
        Form.resize(240, 320)
        self.qTextOutput = QtWidgets.QTextBrowser(Form)
        self.qTextOutput.setGeometry(QtCore.QRect(10, 200, 221, 121))
        self.qTextOutput.setObjectName("qTextOutput")
        self.qTextInput = QtWidgets.QLineEdit(Form)
        self.qTextInput.setGeometry(QtCore.QRect(10, 10, 221, 51))
        self.qTextInput.setObjectName("qTextInput")
        self.qButtonConnect = QtWidgets.QPushButton(Form)
        self.qButtonConnect.setGeometry(QtCore.QRect(20, 110, 75, 23))
        self.qButtonConnect.setObjectName("qButtonConnect")
        self.qButtonSend = QtWidgets.QPushButton(Form)
        self.qButtonSend.setGeometry(QtCore.QRect(144, 110, 81, 23))
        self.qButtonSend.setObjectName("qButtonSend")
        self.fetch_thread = QtCore.QThread()
        self.device_connected = False

        self.retranslateUi(Form)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        _translate = QtCore.QCoreApplication.translate
        Form.setWindowTitle(_translate("Form", "Form"))
        self.qButtonConnect.setText(_translate("Form", "Connect"))
        self.qButtonSend.setText(_translate("Form", "Send"))
        self.qButtonConnect.clicked.connect(self.connect_device)
        self.qButtonSend.clicked.connect(self.send_data)
        
    def connect_device(self):
        if self.device_connected:
            if self.tivaC.connected:
                self.qButtonSend.clicked.disconnect(self.send_data)
                self.tivaC.disconnect_device()
                del self.tivaC
                self.device_connected = False
                self.browser_write("Tive C disconnected",FAILURE)
        else:
            self.browser_write("Attempting to connect ..")
            self.tivaC = USB_device(idVendor = 0x1cbe, idProduct = 0x0003)
            for retry in range(0,5):
                connected = self.tivaC.connect_device()
                if connected:
                    self.ivCount = 0;
                    self.ivPolt = IVUpdate()
                    self.ivPolt.on_launch()
                    self.ivPolt._init_()
                    self.temperaturePlot = TempUpdate();
                    self.temperaturePlot._init_();
                    self.temperaturePlot.on_launch();
                    self.time = 0;
                    self.timeArray = []
                    self.tempArray = []
                    self.ivArray = []
                    self.iv_has_data = False
                    self.last_message = 40
                    self.only_temp_count = 0
                    #self.message_count = 0
                    self.tivaC.moveToThread(self.fetch_thread)
                    self.tivaC.dataReceived.connect(self.handle_data)
                    self.fetch_thread.started.connect(self.tivaC.read_loop)
                    self.qButtonSend.clicked.connect(self.send_data)
                    self.fetch_thread.start()
                    self.browser_write("TIva C connected",SUCCESS)
                    self.device_connected = True
                    break
                else:
                    self.browser_write("Retrying ("+str(retry)+"/4)",NORMAL)
            if not self.tivaC.connected:
                self.browser_write("Unable to connect",FAILURE)
            
    def browser_write(self,message,type = NORMAL):
        self.qTextOutput.append(type+message+"</font>")
        #self.message_count = self.message_count + 1
    
    def handle_data(self,data):
        if self.tivaC.mutex.tryLock():
            new_data = [byte for byte in data]
            self.tivaC.primaryBuff = []
            #print("print\n"+str(self.tivaC.primaryBuff))
            self.tivaC.mutex.unlock()
            #processed_data = [chr(new_data[i]) for i in range(len(new_data))]
            #self.browser_write(" ".join(new_data))
            self.browser_write(" ".join(str(ord(i)) for i in new_data))
            if(len(new_data) == 1):
                self.time = self.time + 1
                self.only_temp_count = self.only_temp_count + 1
                if (self.time % 7 == 0):
                    self.timeArray.append(self.time * 0.04)
                    self.tempArray.append(ord(new_data[0]) if ord(new_data[0]) < 127 else (ord(new_data[0]) - 256))
                    if (not self.iv_has_data):
                        self.temperaturePlot.on_running(self.timeArray, self.tempArray);
                    self.timeArray = []
                    self.tempArray = []
            elif((len(new_data) == 5) and (ord(new_data[0]) in [0,1])):
                self.ivCount = self.ivCount + 1
                self.iv_has_data = True
                if (self.ivCount % 20 == 0):
                    self.ivArray.append(new_data)
                self.only_temp_count  = 0
                
            if(self.only_temp_count > 10 and self.iv_has_data):
                [v_data, i_data] = decode_iv_data(self.ivArray)
                self.ivPolt.on_running(v_data, i_data, self.last_message)
                self.ivArray =[]
                self.iv_has_data = False
                
    def send_data(self):
        #print(bytes([int(str(self.qTextInput.text()))]))
        self.tivaC.send_data(bytes([int(str(self.qTextInput.text()))]))
        self.last_message = int(str(self.qTextInput.text()))
        
if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    Form = QtWidgets.QWidget()
    ui = Ui_Form()
    ui.setupUi(Form)
    Form.show()
    sys.exit(app.exec_())

