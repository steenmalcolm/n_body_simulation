#ifndef PTCL
#define PTCL
#include<iostream>
#include<random>
#include<iomanip>
using namespace std;


class Particle{
private:
  int id;
  float r[3];
  float v[3];
  float m;

public:
////////////////////////////////////////////////////////////////////////
// Particle Constructors

  Particle(float coord[6], float m): id(rand()), m(m){
    float rNew[3] = {coord[0], coord[1], coord[2]};
    float vNew[3] = {coord[3], coord[4], coord[5]};
    setR(rNew);
    setV(vNew);
  }

  Particle(float coord[6]): m(5.){
    float rNew[3] = {coord[0], coord[1], coord[2]};
    float vNew[3] = {coord[3], coord[4], coord[5]};
    setR(rNew);
    setV(vNew);
  } 

  Particle(): m(1.){
    float r[3] = {0., 0., 0.};
    float v[3] = {0., 0., 0.};
    setR(r);
    setV(v);
  }

////////////////////////////////////////////////////////////////////////
// Class getter & setters

  void print(){
    // cout << r[0] << " "<< r[1] << " "<< r[2] << endl;
  }

  int getId(){
    return id;
  }

  float* getR(){
    return r;
  }

  float* getV(){
    return v;
  }

  float getM(){
    return m;
  }

  void setR(float rNew[3]){
    for(int i=0; i<3; i++){
      // if ((rNew[i]>1.) || (rNew[i]<0.))
      //   cout << "Warning: new position is outside of lattice!\n";
      r[i] = rNew[i];
    }
  }

  void setV(float vNew[3]){
    for(int i=0; i<3; i++)
      v[i] = vNew[i];
  }

  void setM(float mNew){
    
    if (mNew<0)
      cout << "Warning: mass set to negative!\n";
    m = mNew;
  }

  void updateV(float addV[3]){
    
    for(int i=0; i<3; i++){
      v[i] +=addV[i];  
      
      
      }
  }
  
  void updateR(float addR[3]){
    for(int i=0; i<3; i++)
      r[i] +=addR[i];
  }


////////////////////////////////////////////////////////////////////////
// Overload Operators

  void operator()(float coord[6]){
    float rNew[3] = {coord[0], coord[1], coord[2]};
    float vNew[3] = {coord[3], coord[4], coord[5]};
    setR(rNew);
    setV(vNew);
  }
};


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
// ParticleField Class keeps track of all Particles in the simulation//
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

class ParticleField{
private:
  int nPtcl;
  Particle* field;
  FILE* fileName = fopen("positions.txt", "w");

public:
////////////////////////////////////////////////////////////////////////
// Particle Field Constructors

  ParticleField(int n, float coords[]): nPtcl(n) {
    fprintf(fileName, "%d \n", n);
    field = new Particle[n]();
    float coord[6] = {0., 0., 0., 0., 0., 0.};

    for(int i=0; i<n; i++){
      for(int j=0; j<6; j++)
        coord[j] = coords[6*i + j];
      field[i](coord);
    }
  }

  ParticleField(int n): nPtcl(n) {
    fprintf(fileName, "%d \n", n);
    field = new Particle[n]();
    // initialize all particle coordinates with random numbers between 0 and 1
    float randCoord[6] = {0., 0., 0., 0., 0., 0.};
    long int norm = pow(2, 30);
    for(int i=0; i < n; i++){
      for(int j=0; j < 6; j++){
        randCoord[j] = (float)rand()/(float)norm;
        if (j<3)
          randCoord[j] *= 10;
      }
      field[i](randCoord);
    }
  }

  ~ParticleField(){
    fclose(fileName);
    delete field;
  }

////////////////////////////////////////////////////////////////////////
// Getter and Setter

  int getN(){
    return nPtcl;
  }

  Particle* getField(){
    return field;
  }

////////////////////////////////////////////////////////////////////////
// Overload Operators

  Particle &operator[](int i){
    return field[i];
  }


////////////////////////////////////////////////////////////////////////
// Class Methods

  void printParticles(){
    cout << "Number of particles: " << nPtcl << endl;
    for(int i=0; i<nPtcl; i++){
      cout << i <<". Particle: (";
      cout << setprecision(4) << field[i].getR()[0] << ", " << field[i].getR()[1] << ", " << field[i].getR()[2];
      cout << ")\n";
    }
  }

  void print(){
    const int dim = 40;
    int ptclPos[2];
    char map[dim][dim];
    
    for(int i=0; i<dim; i++)
      for(int j=0; j<dim; j++)
        map[i][j] = ' ';

    for(int i=0; i<nPtcl; i++){
      for(int x=0; x<2; x++)
        ptclPos[x] = (int)field[i].getR()[x];
      map[ptclPos[0]][ptclPos[1]] = 'O';
    }

    for(int i=0; i<dim; i++){
      for(int j=0; j<dim; j++)
        cout << map[i][j];
      cout << endl;
    }
  }

  void updateVel(float delT){

    Particle* curPtcl, *nextPtcl;
    float diff[3], velCur[3], velNext[3], dist; 

    for(int i=1; i<nPtcl; i++){
      curPtcl = &field[i];

      for(int j=0; j<i; j++){
        nextPtcl = &field[j];
        dist     = 0;
        for(int x=0; x<3; x++){
          diff[x] = nextPtcl->getR()[x] - curPtcl->getR()[x];
          dist   += diff[x]*diff[x];
        }
        dist   = pow(dist, 1.5);
        for(int x=0; x<3; x++){
          velCur[x]   = delT * curPtcl->getM() * diff[x] / dist;
          velNext[x]  = -1 * delT * nextPtcl->getM() * diff[x] / dist;  
        }
        curPtcl->updateV(velCur);
        nextPtcl->updateV(velNext);
      }
    }
  }

  void updateR(float delT){

    Particle curPtcl;
    float rAdd[3];
    
    for(int i=0; i<nPtcl; i++){
      for(int x=0; x<3; x++){
        fprintf(fileName, "%-8.5f, ", field[i].getR()[x] );
        rAdd[x] = delT * field[i].getV()[x];
      }
      field[i].updateR(rAdd);
    }
    fprintf(fileName, "\n");
  }


};
#endif