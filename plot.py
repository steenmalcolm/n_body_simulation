import matplotlib.pyplot as plt
import numpy as np
import time

f = open("positions.txt")
n = int(f.readline())
data = f.readlines()
positions = np.zeros((len(data), n, 3), dtype=float)
scalePos = 0
for i, line in enumerate(data):
  lineArr = line.split(",")
  for j in range(n):
    for k in range(3):
      positions[i][j][k] = float(lineArr[j*3 + k])

scale = 99./np.max(positions)

for i in range(len(data)):

  map = np.zeros((100, 100, 3), dtype=float)
  for j in range(n):
    map[int(positions[i][j][0]*scale)][int(positions[i][j][1]*scale)] = [1., 1., 1.]
  plt.imshow(map)
  plt.show()
  plt.close()
  time.sleep(0.01)
  