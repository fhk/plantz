"""
get the data from my particle
"""
import socket
from datetime import datetime

socketErrors = 0
timeoutErrors = 0
host = "192.168.1.115"
port = 5212

while True:
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    print(s)
    s.settimeout(3)

# while we have a socket poll for data from Spark every 15 seconds
    while s:
        theTime = datetime.now()
        #untidy way to start as close as possible to 15.0 secs after last poll
        if ((theTime.second % 15 == 0) and (theTime.microsecond < 100000)): #it will always take more than 0.1 secs to process my request
            print(theTime)
            try:
                s.connect((host, port))        # connect to Spark server
                s.sendall(b'Pi Ready\0 ')      # client ready for data
            except socket.error:
                print('unable to connect')
                socketErrors += 1
                break

            r = 'not read anything'
            try:
                data, addr = s.recvfrom(1024)
                print("%s" % data)
            except socket.timeout:
                print("socket timeout")
                timeoutErrors += 1
                break
            except socket.error as msg:      #despite having already trapped socket errors at s.connect we need a second check here

                print("socket refused: %s" % msg)
                socketErrors += 1
                break

            if r == 0:          # if r is 0 then the sender has closed for good
                print('socket disconnected')
                print(s)
                break

 # should now have received text from server in r
 # do stuff with the data - validate and parse etc.
 # ...
 # ...
    s.close()