#include <iostream>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include "lattice.hpp"
#include "ptcl.hpp"
using namespace std;

int main(int argc, char *argv[]){
  int n_particles = 4;
  int time_steps = 3000000;
  float dT = 0.0003;

  for (int i=1; i<argc; i+=2){
    if (strcmp(argv[i], "-n") == 0)
      n_particles = stoi(argv[i+1]);
    else if (strcmp(argv[i], "-ts")==0)
      time_steps = stoi(argv[i+1]);
    else if (strcmp(argv[i], "-dT"))
      dT = atof(argv[i+1]);
    }
 
  srand(time(NULL));
  State* particle_states = new State[n_particles]();
  for (int i=0; i<n_particles; i++){
    float* position = new float[3]();
    float* velocity = new float[3]();

    // Three coordinates
    for (int j=0; j<3; j++){
      // Ensure that particles stay in x/y-plane
      if (j==2){
        position[j] = 0.;
        velocity[j] = 0.;
      }
      else{
        position[j] = 400*float(rand()/pow(2,16));

        // Ensure the total impulse remains zero, assuming all masses are equal.
        if (i==n_particles-1){
          velocity[j]=0.;
          for (int k=0; k<n_particles-1; k++)
            velocity[j] -= particle_states[k].velocity[j];
        }
        else
          velocity[j] = 0.2*float(rand()/pow(2,16)); 
      }
    }
    particle_states[i] = {position, velocity};
  }

  
  ParticleField myField(n_particles, particle_states);
  for(int i=0; i<time_steps; i++){
    myField.updateVel(dT);
    myField.updateR(dT);
  }
  return 0;

}