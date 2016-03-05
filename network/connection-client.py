"""
A RTT test on TCP server
"""
import socket
import time
import sys

TCP_IP = sys.argv[1]
TCP_PORT = 5005
BUFFER_SIZE = 1024
#MESSAGE = "Hello, World!"
MESSAGE = "X" * 56

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 

start_time = time.time()
s.connect((TCP_IP, TCP_PORT))
print "Setup:", (time.time() - start_time) * 1000, "ms"

i = 0
numerofpackets = 10

while i < numerofpackets:
    s.send(MESSAGE)
    response = s.recv(BUFFER_SIZE)
    #print response
    i = i + 1
print "Close"

start_time = time.time()
s.close()
print "Teardown:", (time.time() - start_time) * 1000, "ms"
