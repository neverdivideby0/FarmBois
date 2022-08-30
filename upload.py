import serial
import pytz
from datetime import datetime
import pandas as pd
import os
import sys
import subprocess
from threading import Thread
from azure.storage.blob import BlobServiceClient, BlobClient, ContainerClient, __version__
no_of_rcd_per_file = 100
i = 1
connect_str='DefaultEndpointsProtocol=https;AccountName=hpcic;AccountKey=sz4WvFivVmRUxfae4mgelpU1tG1UWEKqimpCNKVv4K12OjK5Ou2Fbwy6aV5xTv/w12asW/Lul14I+AStux1tfw==;EndpointSuffix=core.windows.net'
blob_service_client = BlobServiceClient.from_connection_string(connect_str)
container_name = "hpcic"
cwd = '/home/pi/Desktop/readings'
running_threads=[]

        
        
def upload():
    for filename in os.scandir(cwd): #Iterates through files in folder
        if filename.is_file(): #Checks each file is a file
            index_of_filename=filename.path.find('farmbox') #Gets index of filename from path
            fileName=filename.path[index_of_filename:]#Gets the filename from the path
            if fileName not in running_threads:# If no thread assigned to upload that file
                try:
                    if len(pd.read_csv(cwd + '/' + fileName))+1 >= no_of_rcd_per_file: #Check file is completed
                        filePath = os.path.join(cwd, fileName)
                        running_threads.append(fileName)
                        t = Thread(target = upload_blob, args =(fileName, filePath, blob_service_client))
                        t.daemon = True
                        t.start()
                except pd.errors.EmptyDataError: #Pandas cannot read new/empty csv files
                    continue
                except FileNotFoundError: #Pandas mistakingly checks for deleted files as the deletion is slower
                    continue
                except: #for all other exceptions
                    continue
    print('Uploading: '+ str(running_threads)) # for testing purposes
    return

def upload_blob(fileName, filePath, blob_service_client):
    try:
        blob_client = blob_service_client.get_blob_client(container=container_name, blob=fileName)
        with open(filePath, "rb") as data:
            blob_client.upload_blob(data, overwrite=True) 
            subprocess.call(['sudo','rm', filePath]) #removes local file after successful upload
            running_threads.remove(fileName) #removes thread name in running_threads
    except Exception as e:
        print(e) #Shows error with uploading the file and removes the thread so that the file can be queued to be upload again
        running_threads.remove(fileName)

while True:
    upload()