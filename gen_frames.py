import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

def rescale_partition(partition, min_val=0, max_val=199):
  """
  Rescale elements of a numpy partition to range from min_val to max_val.
  """
  min_partition = partition.min()
  max_partition = partition.max()
    
  if max_partition==min_partition:
    return partition

  rescaled_partition = min_val + ((partition - min_partition) /\
      (max_partition - min_partition)) * (max_val - min_val)
      
  return rescaled_partition

class GenerateFrames:
  def __init__(self, positions_fp:str):
    self._positions_fp = positions_fp
    self._npixels = 200
    self._nframes = 100
    self._read_positions()
    self._generate_frames()

  def _read_positions(self):
    '''Load data and remove last column which is empty'''

    data = pd.read_csv(self._positions_fp).to_numpy()[::,:-1]

    self._nsteps, n_columns = data.shape
    self._nparticles = n_columns//3 # Divide by number of spacial dimenstions
    self._positions = data.reshape(self._nsteps, self._nparticles, 3)
    # Rescale for spacial coordinates to be inside the range [0,200]
    self._positions = (self._npixels-1)*(self._positions-self._positions.min())/\
                      (self._positions.max()-self._positions.min())

  def _generate_frames(self):
    '''Generate images for each time step in order to compile an mp4 file'''

    print('\nBeginning generating frames:\n')
    img = np.zeros((self._npixels, self._npixels))

    step_size = self._nsteps//self._nframes
    for frame in range(0, self._nsteps, step_size):
      if frame%(10*step_size)==0:
        print(f'{frame}/{self._nsteps} frames generated')

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
      # Save the frame
      plt.savefig(f"frames/frame_{frame//step_size:03d}.png", dpi=200)  # dpi=100 will result in 100x100 pixel image
      plt.close()

if __name__ == '__main__':
  GenerateFrames('positions.csv')