from defines import *
from USB_device import *

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
        self.qTextOutput.append(type+message+"</font>\n")
    
    def handle_data(self,data):
        if self.tivaC.mutex.tryLock():
            new_data = [byte for byte in data]
            self.tivaC.primaryBuff = []
            #print("print\n"+str(self.tivaC.primaryBuff))
            self.tivaC.mutex.unlock()
            #processed_data = [chr(new_data[i]) for i in range(len(new_data))]
            self.browser_write(" ".join(new_data))
            #self.browser_write(" ".join(str(ord(i)) for i in new_data))
            
    def send_data(self):
        self.tivaC.send_data(str(self.qTextInput.text()))
        
if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    Form = QtWidgets.QWidget()
    ui = Ui_Form()
    ui.setupUi(Form)
    Form.show()
    sys.exit(app.exec_())

