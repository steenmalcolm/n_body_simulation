import os
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
root_dir = os.path.dirname(os.path.dirname(__file__))

class GenerateFrames:
  def __init__(self, positions_fp:str):
    self._positions_fp = positions_fp
    self._npixels = 200
    self._nframes = 500

    self._read_positions()
    self._generate_frames()

  def _read_positions(self):
    '''Load data and remove last column which is empty'''

    data = pd.read_csv(self._positions_fp).to_numpy()[::,:-1]

    self._nsteps, n_columns = data.shape
    self._nparticles = n_columns//3 # Divide by number of spacial dimenstions
    self._positions = data.reshape(self._nsteps, self._nparticles, 3)

    # Rescale for spacial coordinates to be inside the range [0,200]
    for i in range(3):
      self._positions[...,i] -= np.min(self._positions[...,i])-2

    max_val = self._positions.max()

    self._positions = (self._npixels-2)*self._positions / max_val

  def _generate_frames(self):
    '''Generate images for each time step in order to compile an mp4 file'''

    frames_fp = os.path.join(root_dir, 'visualization', 'frames')

    print('\nBeginning generating frames:\n')
    img = np.zeros((self._npixels, self._npixels))

    step_size = self._nsteps//self._nframes
    for frame in range(0, self._nsteps, step_size):
      if frame%(10*step_size)==0:
        print(f'{frame//step_size}/{self._nframes} frames generated')

      plt.figure(figsize=(5, 5))
      plt.axis("off")

      if frame>0:
        for particle in self._positions[frame-step_size]:
          coords = list(map(round, particle))
          img[coords[0]][coords[1]] = 0

      for particle in self._positions[frame]:
        coords = list(map(round, particle))
        img[coords[0]][coords[1]] = 1

      plt.imshow(img,cmap='gray')
      
      plt.savefig(
        os.path.join(frames_fp, f"frame_{frame//step_size:03d}.png"),
        dpi=self._npixels)
      plt.close()

if __name__ == '__main__':
  GenerateFrames(os.path.join(root_dir, 'visualization', 'positions.csv'))