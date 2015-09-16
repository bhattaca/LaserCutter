/*
File name: plethora_DataStructure.h
Created by: Ari
Created on: 
Synopsis: data structure set up.
*/

#ifndef __DATA_STRUCTURE__
#define __DATA_STRUCTURE__

#include <iostream>
#include <vector>
#include <limits>
#include <unordered_map>

using namespace std;

// CONSTANTS.
namespace CONSTANTS{
	const double MatCost = 0.75;
	const double Padding = 0.1; 
	const double MaxLaserSpeed = 0.5; 
	const double MachineTimeCost = 0.07;
}

/*
* Vertex class. 
* Stores a single vertex
*/
class VERTEX{
public:
	double X, Y; 
	VERTEX(double _X=0, double _Y=0) : X(_X), Y(_Y){}; 
};

/*
* Edge Class.
* Base class for all edge types; 
* Pure Virtual functions computeMachineCost &  computeMinMaxDim 
* MUST be overridden by child class.
*/
class EDGES{
public:
	// ids of the endpoints
	vector<int> endpoints;

	//Diagonal coordinates of the rectangle enclosing the edge. 
	double edgeMaxX, edgeMaxY;
	double edgeMinX, edgeMinY;
	EDGES (){ 
		edgeMaxX=edgeMaxY=numeric_limits<double>::min();
		edgeMinX=edgeMinY=numeric_limits<double>::max();
	}
	//Given a vertex id, return the corresponding vertex
	VERTEX getVertex(int endpt, const unordered_map <int, VERTEX> &);

	//pure virtual;
	virtual double computeMachineCost(const unordered_map <int, VERTEX> &)=0;
	virtual double computeMinMaxDim(const unordered_map <int, VERTEX> &)=0;
};

/* 
* Line Segments inherits form EDGES
*/
class LINE:public EDGES{
public:
	double computeMachineCost (const unordered_map <int, VERTEX> & vertices);
	double computeMinMaxDim( const unordered_map <int, VERTEX> & vertices);
};

/*
* Circular Arcs inherits from EDGES
*/
class CIRCLE:public EDGES{
public:
	VERTEX center; 
	int clockwiseFrom; 
	double computeMachineCost (const unordered_map <int, VERTEX> & vertices);
	double computeMinMaxDim( const unordered_map <int, VERTEX> & vertices);
};

/*
* The Profile of a 2D object.
* Represented as a vector of pointers to EDGE.
*/
class PROFILE{

protected:
	double computeTotalMachineCost();
	double computeTotalMaterialCost();

public:
	// The profile is a vector of edges.
	vector <EDGES*> profileEdges;
	unordered_map <int, VERTEX> vertices;
	double maxX, maxY;
	double minX, minY;

	PROFILE(){ 
		maxX=maxY=numeric_limits<double>::min();
		minX=minY=numeric_limits<double>::max();
	}
	~PROFILE(){
		// Delete all the allocated memory
		for ( auto e:profileEdges)
			delete e; 
	}
	void printEdges();
	void printVertices();
	// Compute and return the quote. 
	double computeQuote(){return computeTotalMachineCost() + computeTotalMaterialCost();}
};

#endif