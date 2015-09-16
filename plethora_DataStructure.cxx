/*
File name: plethora_DataStructure.cxx
Created by: Ari
Created on: 
Synopsis: data structure set up.
*/
#include "plethora_DataStructure.h"
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>


// HELPER FUNCTION DEF


double compute_l2_distance(const VERTEX&, const VERTEX&);

//DEBUG :: Print edges
void PROFILE::printEdges(){
	cout <<"PRINT EDGES"<< endl;
	for (auto e:profileEdges){
		for (auto f:e->endpoints) cout <<f<<" ";
		cout << endl;
	}
}

//DEBUG :: Print Vertices
void PROFILE::printVertices(){
	cout <<"PRINT VERTICES"<< endl;
	for ( auto i:vertices){
		cout << i.first << " "<< i.second.X <<" "<< i.second.Y << endl;
	}
}

// Compute the total machine cost
// Calls the virtual function computeMachineCost
double PROFILE::computeTotalMachineCost(){
	double sum(0);
	for (auto e:profileEdges){
		sum+=e->computeMachineCost(vertices);
	}
	return sum;
}
// Compute the total materials cost. 
// First computes the min and max X,Y and then computes the cost of the materials.
double PROFILE::computeTotalMaterialCost(){
	for (unsigned int i=0; i < profileEdges.size();i++){
		profileEdges[i]->computeMinMaxDim(vertices);

		maxX = max(maxX, profileEdges[i]->edgeMaxX);
		maxY = max(maxY, profileEdges[i]->edgeMaxY);

		minX = min(minX, profileEdges[i]->edgeMinX);
		minY = min(minY, profileEdges[i]->edgeMinY);
		//DEBUG check if max min works!
		//cout << minX <<" "<< minY<<", "<< maxX<<" "<< maxY <<endl;
	}
	//DEBUG
	//cout <<"Material Cost  "<< ((maxX-minX)+0.1)*((maxY-minY)+0.1)*CONSTANTS::MatCost<<endl;
	return  ((maxX-minX) + CONSTANTS::Padding)
		*((maxY-minY) + CONSTANTS::Padding) * CONSTANTS::MatCost;
}

VERTEX EDGES::getVertex(int id,const unordered_map <int, VERTEX> & vertices){
	unordered_map<int, VERTEX>::const_iterator it = vertices.find(id);
	if ( it !=vertices.end()){
		return it->second;
	}
	else
	{
		cerr <<"ERROR VERTEX "<< id << "NOT DEFINED."<< endl; 
		exit(0);
	}
}

// Compute Machine Cost for Line Segment
double LINE::computeMachineCost(const unordered_map <int, VERTEX> & vertices){
	// machine cost
	VERTEX v1 = getVertex(endpoints[0], vertices);
	VERTEX v2 = getVertex(endpoints[1], vertices);
	double dist = compute_l2_distance( v1, v2);
	double machineCost =  (dist/CONSTANTS::MaxLaserSpeed)*CONSTANTS::MachineTimeCost;
	//DEBUG
	//cout <<"machine cost " << machineCost << endl;
	return machineCost;
}
// Compute Min Max X,Y for Line Segment. 
double LINE::computeMinMaxDim(const unordered_map <int, VERTEX> & vertices){
	for(auto endPt:endpoints){
		VERTEX v1 = getVertex(endPt,vertices);
		edgeMaxX = max(edgeMaxX, v1.X);
		edgeMaxY = max(edgeMaxY, v1.Y);

		edgeMinX = min(edgeMinX, v1.X);
		edgeMinY = min(edgeMinY, v1.Y);
	}
	return  0;
}
// Compute Machine Cost for  circular arc
double CIRCLE::computeMachineCost(const unordered_map <int, VERTEX> & vertices){
	// machine cost
	VERTEX v1 = getVertex(endpoints[0], vertices);
	VERTEX v2 = getVertex(endpoints[1], vertices);

	double c = compute_l2_distance(v1, v2);
	double r = compute_l2_distance(v1, center);
	double angle = acos((2.0*r*r-c*c)/(2*r*r));
	double angleDegree = angle*57.2957795;

	double perimeterDist = ((2.0*M_PI*r)/360.0)*angleDegree;
	double machineCost =  (perimeterDist/(CONSTANTS::MaxLaserSpeed*exp(-1/r)))
		* CONSTANTS::MachineTimeCost;
	return machineCost;
}
// Compute Min Max X,Y for Circle
double CIRCLE::computeMinMaxDim(const unordered_map <int, VERTEX> & vertices){
	if (clockwiseFrom == endpoints[0]){
		for(unsigned int i=0;i<endpoints.size();i++){
			VERTEX v1 = getVertex(endpoints[i],vertices);
			edgeMaxX = max(edgeMaxX, v1.X);
			edgeMaxY = max(edgeMaxY, v1.Y);
			edgeMinX = min(edgeMinX, v1.X);
			edgeMinY = min(edgeMinY, v1.Y);
		}
	}
	else{
		for(unsigned int i=0;i<endpoints.size();i++){
			VERTEX v1 = getVertex(endpoints[i],vertices);
			edgeMaxX = max(edgeMaxX, v1.X);
			edgeMaxY = max(edgeMaxY, v1.Y);
			edgeMinX = min(edgeMinX, v1.X);
			edgeMinY = min(edgeMinY, v1.Y);
		}
		// compute 3rd point.
		VERTEX p = getVertex(clockwiseFrom, vertices);

		double r = compute_l2_distance(p, center);
		double pointCX = center.X;
		double pointCY = center.Y;
		if( p.Y < center.Y){ pointCX -= r;}
		else if (p.X <  center.X){ pointCY += r;}
		else if (p.Y > center.Y){ pointCX += r;}
		else if (p.X > center.X){pointCY -= r;}
		else{cerr <<"ERROR: UNDEFINED CASE " <<endl;}
		edgeMaxX = max(edgeMaxX, pointCX);
		edgeMaxY = max(edgeMaxY, pointCY);
		edgeMinX = min(edgeMinX, pointCX);
		edgeMinY = min(edgeMinY, pointCY);
	}
	return 0;
}


// HELPER FUNCTIONS
double compute_l2_distance( const VERTEX &v1, const VERTEX &v2){
	return sqrt((v1.X-v2.X)*(v1.X-v2.X)+(v1.Y-v2.Y)*(v1.Y-v2.Y));
}