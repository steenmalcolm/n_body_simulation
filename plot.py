import matplotlib.pyplot as plt
import numpy as np
from PIL import Image

f = open("positions.txt")
n = int(f.readline())
data = f.readlines()
positions = np.zeros((len(data), n, 2), dtype=float)
scalePos = 0
for i, line in enumerate(data):
  lineArr = line.split(",")
  for j in range(n):
    for k in range(2):
      positions[i][j][k] = float(lineArr[j*3 + k])

positions = 99*(positions-np.min(positions))/(np.max(positions)-np.min(positions))

def generate_frames(num_frames=1000):
    for i in range(0, num_frames, 10):
        print(f'\nStep {i}')
        plt.figure(figsize=(5, 5))  # 1x1 inch
        plt.axis("off")
        img = np.zeros((100, 100))

        for j, particle in enumerate(positions[i]):
          coords = list(map(round, particle))
          print(j, coords)
          img[coords[0]][coords[1]] = 1
        plt.imshow(img,cmap='gray')
        # Save the frame
        plt.savefig(f"frames/frame_{i//10:03d}.png", dpi=100)  # dpi=100 will result in 100x100 pixel image
        plt.close()

generate_frames()
