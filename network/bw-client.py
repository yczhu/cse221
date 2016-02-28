"""
A RTT test on TCP server
"""
import socket
import time
import sys

TCP_IP = sys.argv[1]
TCP_PORT = 5005
BUFFER_SIZE = 1024
#MESSAGE = "X" * 1

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
s.connect((TCP_IP, TCP_PORT))

f = open ("sample.txt", "rb") 
l = f.read(BUFFER_SIZE)
while (l):
    s.send(l)
    l = f.read(BUFFER_SIZE)

f.close()
print "Close"
s.close()

