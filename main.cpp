#include<iostream>
#include<math.h>
#include<stdio.h>
#include <unistd.h>
#include"lattice.hpp"
#include"ptcl.hpp"
using namespace std;

void bar(int a[3]){}


int main(){
  
  float coords[18] = {20., 10., 0., 0.1, 0., 0., 20., 30.,0., -0.1, -0., 0., 0., 0., 0., 0., 0., 0.};
  ParticleField myField(3, coords);
  float dT = 0.5;
  for(int i=0; i<1000; i++){
    myField.updateVel(dT);
    myField.updateR(dT);
  }
   
    
  
  return 0;


}