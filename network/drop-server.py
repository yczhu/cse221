import socket
import sys
import time

TCP_IP = '0.0.0.0'
TCP_PORT = 5005
BUFFER_SIZE = 1024  # Normally 1024, but we want fast response

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((TCP_IP, TCP_PORT))
s.listen(5)

conn, addr = s.accept()
print 'Connection address:', addr
res = ""
starttime = time.time()
while 1:
    data = conn.recv(BUFFER_SIZE)
    if not data: break
    res += data

conn.close()
s = sys.getsizeof(res) / (1024.0 * 1024.0)
t = (time.time() - starttime)
print s, "MB"
print t, "s"
print "BW:", s/t*8, "Mb/s"
