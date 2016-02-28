"""
A RTT test on TCP server
"""
import socket
import time
import sys

TCP_IP = sys.argv[1]
#TCP_IP = '127.0.0.1'
#TCP_IP = '45.55.2.40'
TCP_PORT = 5005
BUFFER_SIZE = 1024
#MESSAGE = "Hello, World!"
MESSAGE = "X" * 56

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
s.connect((TCP_IP, TCP_PORT))

i = 0
numerofpackets = 10

while i < numerofpackets:
    start_time = time.time()
    s.send(MESSAGE)
    response = s.recv(BUFFER_SIZE)
    #print response
    print (time.time() - start_time) * 1000
    i = i + 1

print "Close"
s.close()

