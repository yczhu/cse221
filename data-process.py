import numpy as np
import sys

num = []
for line in sys.stdin:
    num.append(int(line.strip()))

n_array = np.array(num)

print n_array.shape[0], "datas"
print "Average:", np.average(n_array)
print "Standar Deviation:", np.std(n_array)
