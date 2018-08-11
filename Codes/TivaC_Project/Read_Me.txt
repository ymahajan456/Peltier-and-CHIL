1. All global variables declaed in basic_includes.h  but to be initialized in main file
2. Init_Device.h   : All peripheral initialization
3. MAIN File : USB IO and Msg decoding 

Stop Interrups before sending data via USB. Don't want to receive timer interrup 
	while receiving and decoding data. Higher Prioroty to USB interrupt
	
Timer Interrupt:
	Timer duration of 5 ms
	Temperature then 3 Times IV(Give sufficient time for settling)
	Use timmer status global variable for that
	
IV Loop:


Temperature Loop:
	Effectively called after every 20 mS
	Read LM35 and INA values
	Calculate offset and set global values to affect devices
	Set Stability bit also if offset < 0.5 V
	Send required repeated data 
	
IV Loop:


