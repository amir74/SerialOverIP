local uci  = require "luci.model.uci"                           
local var = "9600"                                              
state = uci.cursor()                                            
state.get("COM")                                                
                                                                
m = Map("COM", "", "Setting to send over data to serial port")  
                                                                
d = m:section( TypedSection, "info", "COM0", "serial port 1")   
                                                                
b = d:option( Value ,"BaudRate", "Baud Rate", "speed which the p
b.optional = false                                              
b:value("9600", "9600")                                         
b:value("57600", "57600")                                       
b:value("34800", "34800")                                       
b:value("115200", "115200")                                     
--Sanitize inputs maybe?                                        
                                                                
                                                                
                                                                
                                                                
tcp = d:option(Value, "TCPPort", "TCP port", "specify the TCP po
tcp.default = "4001"                                            
                                                                
db = d:option( ListValue, "DataBits", "Data Bits", "what bit is 
db.default = "8"                                                
db:value( "Baudot", "5")                                        
db:value( "ASCII", "7")                                         
db:value( "Common", "8")                                        
db:value( "Rare", "9")                                          
                                                                
pa = d:option( ListValue, "Parity", "Parity", "Parity policy to 
pa:value( "NONE", "None")                                       
pa:value( "ODD", "Odd")                                         
pa:value( "EVEN", "Even")                                       
pa.default = "NONE"                                             
                                                                
sb = d:option( ListValue, "StopBits", "Stop Bits", "Bits for sto
sb:value( "one", "1")                                           
sb:value( "two", "2")                                           
sb.default = "one"                                              

fc = d:option( ListValue, "FlowCtrl", "Flow Control", "Control t
fc:value( "XON", "XON/XOFF")                                    
fc:value( "NO", "No Handshake")                                 
fc.default = "XON" 

return m
