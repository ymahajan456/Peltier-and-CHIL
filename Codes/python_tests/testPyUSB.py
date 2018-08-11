import usb.core
import usb.util

dev = usb.core.find(idVendor=0x1cbe, idProduct=0x0003)
dev.set_configuration()
cfg = dev.get_active_configuration()
intf = cfg[(0,0)]
epOut = usb.util.find_descriptor(
    intf,
    custom_match = \
    lambda e: \
        usb.util.endpoint_direction(e.bEndpointAddress) == \
        usb.util.ENDPOINT_OUT)

epIn = usb.util.find_descriptor(
    intf,
    custom_match = \
    lambda e: \
        usb.util.endpoint_direction(e.bEndpointAddress) == \
        usb.util.ENDPOINT_IN)
epOut.write('blahBlaj')
data = epIn.read(64)
print(str(data))
print([chr(ta) for ta in data])
print(type(data))
usb.util.release_interface(dev,intf)