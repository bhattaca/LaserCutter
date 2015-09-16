/*
File name: main.cxx
Created by: Ari
Created on: 
Synopsis: 2D laser cutting quote generator
*/

#include<iostream>
#include <sstream>
#include <map>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include "plethora_DataStructure.h"

using namespace std;
using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;




//  Parse the input file (json) and create the 2D object;
bool parse(PROFILE* &obj, const string & fileName) {
	// Read json.
	//ifstream file ( "Rectangle.json");
	//ifstream file ("ExtrudeCircularArc.json");
	//ifstream file ("ExtrudeCircularArc.json");
	//ifstream file ("circle.json");

	ifstream file(fileName); 
	ptree pt;
	if ( file.is_open()){
		std::stringstream is;
		is << file.rdbuf();
		file.close(); 
		boost::property_tree::read_json(is, pt);
	}
	else  {
		cerr <<"Cannot find file."<< endl;
		return false;
	}
	//EDGES
	BOOST_FOREACH(ptree::value_type &v,
		pt.get_child("Edges")){
			//cout << v.first << endl;
			//cout << v.second.get_child("Type").get_value<string>() <<endl;
			string edgeType = v.second.get_child("Type").get_value<string>();
			if ( edgeType == "LineSegment"){
				LINE *l = new LINE();
				BOOST_FOREACH(ptree::value_type &v2,
					v.second.get_child("Vertices")){ 
						l->endpoints.emplace_back(v2.second.get_value<int>());
				}
				obj->profileEdges.emplace_back(l);
			}
			else if ( edgeType == "CircularArc"){
				CIRCLE *l = new CIRCLE();
				BOOST_FOREACH(ptree::value_type &v2,
					v.second.get_child("Vertices")){ 
						l->endpoints.emplace_back(v2.second.get_value<int>());
				}
				l->center.X = v.second.get_child("Center").get_child("X").get_value<double>();
				l->center.Y = v.second.get_child("Center").get_child("Y").get_value<double>();
				l->clockwiseFrom = v.second.get_child("ClockwiseFrom").get_value<int>();
				obj->profileEdges.emplace_back(l);
			}
			else
			{
				cerr <<"Unkown Edge Type error." << endl;
			}
	}
	// VERTICES
	BOOST_FOREACH(ptree::value_type &v, pt.get_child("Vertices")){
		VERTEX vert;
		int id = atoi(v.first.c_str());
		vert.X = v.second.get_child("Position").get_child("X").get_value<double>();
		vert.Y = v.second.get_child("Position").get_child("Y").get_value<double>();
		obj->vertices.insert(make_pair(id, vert));
	}
	return true; 
}

int main (){

	PROFILE* obj1 = new PROFILE(); 
	string f; 
	do{
		cout <<"Enter file name (.json) " <<endl;
		cin >> f;} 
	while(!parse(obj1,f));
	cout <<"Total Quote for your object is ";
	cout << std::fixed << std::setprecision(2) << obj1->computeQuote() << " Dollars Only!" <<endl;
	delete obj1;
	return 0;
}