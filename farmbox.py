#https://roboticsbackend.com/raspberry-pi-arduino-serial-communication/
#top left raspi3 arduino-pi connection is /dev/ttyACM0
#!/usr/bin/env python3
import serial
import time
import pytz
from datetime import datetime
import os
import sys
import subprocess
from threading import Thread
from azure.storage.blob import BlobServiceClient, BlobClient, ContainerClient, __version__
no_of_rcd_per_file = 288
i = 1
connect_str='DefaultEndpointsProtocol=https;AccountName=hpcic;AccountKey=sz4WvFivVmRUxfae4mgelpU1tG1UWEKqimpCNKVv4K12OjK5Ou2Fbwy6aV5xTv/w12asW/Lul14I+AStux1tfw==;EndpointSuffix=core.windows.net'
blob_service_client = BlobServiceClient.from_connection_string(connect_str)
container_name = "hpcic"
cwd = '/home/pi/Desktop/readings'

        
        
if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    ser.reset_input_buffer()
    while True:
        try:
            if ser.in_waiting > 0:
                date_time = datetime.now(pytz.timezone('Asia/Singapore'))
                line = ser.readline().decode('utf-8').rstrip()
                if line:
                    if "65534"in line:
                        os.execv(sys.executable, ['python3'] + sys.argv)
                    #line = line + " Leaves:" + str(no_of_leaves)+ " Datetime: " +str(date_time.strftime('%y%m%d_%H%M'))
                    line = line + " Datetime: " +str(date_time.strftime('%y%m%d_%H%M')) 

                print(line)
                if i%no_of_rcd_per_file==1:
                    fileName = 'farmbox'+ str(date_time.strftime('%y%m%d_%H%M'))+'.csv'
                    filePath = os.path.join(cwd, fileName)
                    g = open(filePath, 'w')
                
                g.write(line+'\n') #Coordinates are written into csv file based on date
                i += 1
                

        except Exception as e:
            print(e)
            continue
            
    
            
        
            
        
        time.sleep(60)