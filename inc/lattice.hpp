#include<iostream>
#include<iomanip>
#include"ptcl.hpp"
using namespace std;

class Lattice{
private:
  float*** lattice;
  int dimension;

  // Iterator variables
  float* iterator;
  int pos;
  bool reachedEnd;

public:
/////////////////////////////////////////////////////////////////////////////
// Constructors

  Lattice(int dim) :dimension(dim), pos(0), reachedEnd(false) {
    
    // initialize lattice grid length 'dim' with entries '0'
    this->lattice = new float**[dim];
    
    for(int i=0; i<dim; i++){
      this->lattice[i] = new float*[dim];

      for(int j=0; j<dim; j++)
        this->lattice[i][j] = new float[dim]();
    }
    this->start();
  }


  Lattice(){
    Lattice(16);
  }

/////////////////////////////////////////////////////////////////////////////
// Class getter & setter

  float*** getLattice(){
    return this->lattice;
  }

  int getDimension(){
    return dimension;
  }

  void setLattice(float***& newLattice){
    lattice = newLattice;
  }

  void setDimension(unsigned int newDimension){
    dimension = newDimension;
  }

/////////////////////////////////////////////////////////////////////////////
// Class Methods 

  int* assignNodeIndex(float ptclR[3], int nodeIndex[3], float nodeDist[3]){
    for(int i=0; i<3; i++){
      nodeIndex[i]      = (int)(ptclR[i]*(dimension-1));
      nodeDist[i]    = ptclR[i]*(dimension-1) - nodeIndex[i];
      

    }
    return nodeIndex;
  }

  int* assignNodeIndex(float ptclR[3], int nodeIndex[3]){
    float nodeDist[3];
    return assignNodeIndex(ptclR, nodeIndex, nodeDist);
  }


  void getNodes(Particle ptcl, float* nodePtr[8], float nodeDist[3]){
    
    int nodeIndex[3];
    assignNodeIndex(ptcl.getR(), nodeIndex, nodeDist);
    
    for(int i=0; i<3; i++)
      if(nodeIndex[i] >= dimension-1)
        cout <<"Warning: node Index out of range: " <<i << " "<< nodeIndex << endl;
      
    for(int dx3=0; dx3<2; dx3++)
      for(int dx2=0; dx2<2; dx2++)
        for(int dx1=0; dx1<2; dx1++)
          nodePtr[4*dx3 + 2*dx2+ dx1] = &(lattice[nodeIndex[0]+dx1][nodeIndex[1]+dx2][nodeIndex[2]+dx3]);
  }


  float* getNode(Particle ptcl, int corner){
    int nodeIndex[3];
    assignNodeIndex(ptcl.getR(), nodeIndex);
    switch(corner){
      case 0:
        break;
      case 1:
        nodeIndex[0]++;
        break;
      case 2:
        nodeIndex[1]++;
        break;
      case 3:
        nodeIndex[0]++; nodeIndex[1]++;
        break;
      case 4:
        nodeIndex[2]++;
        break;
      case 5:
        nodeIndex[0]++; nodeIndex[2]++;
        break;
      case 6:
        nodeIndex[1]++; nodeIndex[2]++;
        break;
      case 7:
        nodeIndex[0]++; nodeIndex[1]++; nodeIndex[2]++;
        break;
    }
    return &lattice[nodeIndex[0]][nodeIndex[1]][nodeIndex[2]];
  }

  void scaleField(float factor){
    for(int i=0; i<dimension; i++)
      for(int j=0; j<dimension; j++)
        for(int k=0; k<dimension; k++)
          lattice[i][j][k] *= factor;
  }

  void print(){
    
    for(int k=0; k<dimension; k++){
      for(int j=0; j<dimension; j++){
        for(int i=0; i<dimension; i++)
          cout << std::setprecision(5) << lattice[i][j][k] << "  ";
        cout << endl;
      }
      cout << "\n\n";
    }
  }

  void setNumber(){
    for(int i=0; i<dimension; i++)
      for(int j=0; j<dimension; j++)
        for(int k=0; k<dimension; k++)
          lattice[i][j][k] = (float)i*dimension*dimension + j*dimension + k;
  }
/////////////////////////////////////////////////////////////////////////////
// Overload Operators

  float &operator[](int i){
    if (i >= dimension*dimension*dimension){
      cout << "index too large" << endl;
      exit(-1);
    }
    // match i to lattice unit

    int index[3] = {0, 0, 0};
    int dim = 0;
    
    while(i!=0){
      index[dim] = i % (int)pow(dimension, dim+1);
      i -= index[dim];
      index[dim] /= pow(dimension, dim);
      dim++;
    }

    return this->lattice[index[2]][index[1]][index[0]];
  }

/////////////////////////////////////////////////////////////////////////////
//Iteration Methods

  float* &start(){
    this->iterator = this->lattice[0][0];
    return this->iterator;
  }

  float* &next(){
    if (pos == dimension*dimension*dimension-1){
      this->reachedEnd = true;
      pos = 0;
    }
    else{
      ++pos;
      iterator = &(*this)[pos];
    }
    return this->iterator;
  }

  bool end(){
    if (this->reachedEnd){
      this->reachedEnd = false;
      return false;
    }
    return true;
  }
  };


/////////////////////////////////////////////////////////////////////////////
// Lattice to track potential

class LatticePotential: public Lattice{

public:
  LatticePotential(int dim): Lattice(dim){}

  LatticePotential(): Lattice(){}

/////////////////////////////////////////////////////////////////////////////
// Class Methods
  
  void partitionMass(Particle ptcl){
    float nodeDist[3];
    float* nodePtr[8];
    getNodes(ptcl, nodePtr, nodeDist);
    float sumNodeWeight = 0;
    float nodeWeightPtr[8];
    
    for(int dx3=0; dx3<2; dx3++)
      for(int dx2=0; dx2<2; dx2++)
        for(int dx1=0; dx1<2; dx1++) {
          float nodeWeight                   =  abs((1 - dx1 - nodeDist[0])*(1 - dx2 - nodeDist[1])*(1 - dx3 - nodeDist[2]));
          nodeWeightPtr[4*dx3 + 2*dx2 + dx1] =  nodeWeight;
          sumNodeWeight                      += nodeWeight;
        }
    for(int i=0; i<8; i++){
      *nodePtr[i] = ptcl.getM()*nodeWeightPtr[i]/sumNodeWeight;
    }
  }


  void CIC(ParticleField field){
    for(int i=0; i<field.getN(); i++)
      partitionMass(field[i]);
  }



  
};