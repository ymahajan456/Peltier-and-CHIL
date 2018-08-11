from defines import *

def process_message( message):
    if(message[0] == 0x66):
        return decode_data(message)
    elif(message[0] == 0x99):
        return decode_insrt(message)
    else:
        return
        
def decode_data(message):
    del message[0]
    del message[-1]
    del message[-1]
    decoded_data = {}
    while(message != []):
        data_type = message[0]
        del message[0]
        
        if data_type == PELT_VOLTAGE_DATA :
            decoded_data["Pelt_Voltage"] = (message[0]<<8) + message[1]
            del message[0]
            del message[0]
            
        elif data_type == PELT_CURR_DATA :
            decoded_data["Pelt_Curr"] = (message[0] << 8) + message[1]
            del message[0]
            del message[0]
        
        else:
            decoded_data["Error"] = "Data Missing"
            
    return decoded_data
    