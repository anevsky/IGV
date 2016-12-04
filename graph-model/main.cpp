/*
 
 Programm:              Graph (main.cpp)
 
 Author:                Alex Nevsky
 Created:               12.12.2009
 Email:                 anwer.man@gmail.com 
 WebSite:               http://alexnevsky.com
 
 This program is designed to work with an abstract data type
 of the vertices and edges between them, called a graph. 
 
 Implemented a large number of basic operations for processing the
 graph, so you can easily expand the program if you need be
 (using license Creative Commons (by-nc-sa)
 Attribution Non-commercial Share Alike).
 
 All the basic operations are guaranteed to work for a simple
 undirected graph with nonnegative edges.
 
 If you have any questions, suggestions or comments, you can always 
 find my contact on the site http://alexnevsky.com .
 
 I hope you found the program useful.
 
 Best regards,
 Alex Nevsky
 
 Copyright 2009 Alex A. Nevsky. All rights reserved.
 
 */

#include <iostream>
#include <string>

#include "Classes/File.h"
#include "Classes/Graph.h"

using namespace std;

int main (int argc, char * const argv[]) {
	
	cout << 
	"# Alex Nevsky, group 821702, (c) 2009;" << endl <<		
	"# Belarusian State University of Informatics and Radioelectronics" << endl <<
	"# Department of Artificial Intelligence" << endl <<		
	"# Design Programs in Intelligent Systems." << endl << endl<<
	"# PPvIS, Lab 5: Graph." << endl << endl;
	
	Graph graph;
	
	int userCommand = -1;
	
	do {
		if (userCommand == 0) {
			cout << "Program Info:" << endl;
			cout << "This program is designed to work with an abstract data type \
of the vertices and edges between them, called a graph." << endl << endl << 
			"Implemented a large number of basic operations for processing the \
graph, so you can easily expand the program if you need be." << endl << endl << 
			"All the basic operations are guaranteed to work for a simple \
undirected graph with nonnegative edges." << endl << endl <<
			"If you have any questions, suggestions or comments, you can always \
find my contact on the site http://alexnevsky.com." << endl << endl <<
			"I hope you found the program useful." << endl << endl <<
			"Best regards," << endl << "Alex Nevsky" << endl;
		}
		else if (userCommand == 1) {
			cout << "Set Graph From File 'graphData.txt'" << endl;
			cout << "Processing data. Please, wait a few seconds..." << endl;
			graph.SetFromFile("graphData.txt");
			cout << "Done!" << endl;
		}
		else if (userCommand == 2) {
			cout << "Set Graph From My File" << endl;
			string fileName;
			cout << "Enter file name:" << endl;
			cin >> fileName;
			cout << endl;
			cout << "Processing data. Please, wait a few seconds..." << endl;
			graph.SetFromFile(fileName);
			cout << "Done!" << endl;
		}
		else if (userCommand == 3) {
			cout << "Get Shortest Path From Vertex To Vertex" << endl;
			string input_Vertex1, input_Vertex2;
			cout << "Vertex From:" << endl;
			cin >> input_Vertex1;
			cout << "Vertex To:" << endl;
			cin >> input_Vertex2;
			cout << endl;
			cout << "Path can be as following:" << endl;
			cout << graph.GetShortestPath(input_Vertex1, input_Vertex2) << endl;
			cout << "Done!" << endl;
		}
		else if (userCommand == 4) {
			cout << "Get Distance From Vertex To Vertex" << endl;
			string input_Vertex1, input_Vertex2;
			cout << "Vertex From:" << endl;
			cin >> input_Vertex1;
			cout << "Vertex To:" << endl;
			cin >> input_Vertex2;
			cout << endl;
			cout << "Distance from vertex '" << input_Vertex1 << "' to vertex '" <<
			input_Vertex2 << "' = " << 
			graph.GetDistanceFromVertexToVertex(input_Vertex1, input_Vertex2) << endl;
			cout << "Done!" << endl;
		}
		else if (userCommand == 5) {
			cout << "Get Nearest Vertex For Vertex" << endl;
			string input_Vertex;
			cout << "Enter Vertex:" << endl;
			cin >> input_Vertex;
			cout << endl;
			cout << "Nearest vertex for vertex '" << input_Vertex << "' is vertex '" <<
			graph.GetNearestVertex(input_Vertex) << "'." << endl;
			cout << "Done!" << endl;
		}
		else if (userCommand == 6) {
			cout << "Get Graph Radius" << endl;
			cout << "Graph radius = " <<
			graph.GetRadius() << "." << endl;
			cout << "Done!" << endl;
		}
		else if (userCommand == 7) {
			cout << "Get Vertex Degree" << endl;
			string input_Vertex;
			cout << "Enter Vertex:" << endl;
			cin >> input_Vertex;
			cout << endl;
			cout << "'" << input_Vertex << "' degree = " <<
			graph.GetVertexDegree(input_Vertex) << "." << endl;
			cout << "Done!" << endl;
		}
		else if (userCommand == 8) {
			cout << "Get Adjacent Vertices For Vertex" << endl;
			
			string input_Vertex;
			cout << "Enter Vertex:" << endl;
			cin >> input_Vertex;
			cout << endl;
			
			cout << "Adjacent Vertices For Vertex '" << input_Vertex <<
			"': ";
			
			std::set<Graph::t_Vertex> adjacentVertices = 
			graph.GetAdjacentVertices(input_Vertex);
			
			size_t needComma = adjacentVertices.size()-1;
			
			for (
				 std::set<Graph::t_Vertex>::const_iterator setIter = 
				 adjacentVertices.begin(); 
				 setIter != adjacentVertices.end(); 
				 ++setIter
				 ) 
			{
				cout << *setIter;
				if (needComma) {
					--needComma;
					cout << ", ";
				}
			}
			
			cout << endl;
			
			cout << "Done!" << endl;
		}
		else if (userCommand == 9) {
			cout << "Get Vertex Eccentricity" << endl;
			
			string input_Vertex;
			cout << "Enter Vertex:" << endl;
			cin >> input_Vertex;
			cout << endl;
			
			cout << "Eccentricity For Vertex '" << input_Vertex <<
			"': " << endl;
			
			std::map<Graph::t_Vertex, Graph::t_Eccentricity> ecMap = 
			graph.GetVertexEccentricity(input_Vertex);
			
			for (
				 std::map<Graph::t_Vertex, Graph::t_Eccentricity>::iterator 
				 ecMapIter = ecMap.begin(); 
				 ecMapIter != ecMap.end(); 
				 ++ecMapIter
				 ) 
			{
				cout << "From vertex '" << input_Vertex << "' to vertex '" 
				<< ecMapIter->first << "': " << ecMapIter->second << endl;
			}
			
			cout << "Done!" << endl;
		}
		else if (userCommand == 10) {
			cout << "Show Graph" << endl;
			graph.ShowGraph(cout);
			cout << "Done!" << endl;
		}
		else if (userCommand == 11) {
			cout << "Show All Graph Info" << endl;
			graph.ShowAllInfo(cout);
			cout << "Done!" << endl;
		}
		else if (userCommand == 12) {
			cout << "Save Graph Info To File" << endl;
			string input_FileName;
			cout << "Enter File Name To Save:" << endl;
			cin >> input_FileName;
			
			bool success(false);
			std::ofstream out;
			out.open(input_FileName.c_str());
			if (out) {
				graph.ShowAllInfo(out);
				success = true;
			}	
			else {
				success = false;
				std::cerr << "Error: unable to save data to file \""
				<< input_FileName << "\"!" << std::endl;
			}
			out.close();
			
			cout << "Done!" << endl;
		}
		else if (userCommand == 13) {
			cout << "Exit" << endl;
			cout << "Good bye, dear user!-)" << endl;
			break;
		}
		
		cout << endl;
		cout << "Menu:" << endl <<
		"0 - Program Info" << endl <<
		"1 - Set Graph From File 'graphData.txt'" << endl <<
		"2 - Set Graph From My File" << endl <<
		"3 - Get Shortest Path From Vertex To Vertex" << endl <<
		"4 - Get Distance From Vertex To Vertex" << endl <<
		"5 - Get Nearest Vertex For Vertex" << endl <<
		"6 - Get Graph Radius" << endl <<
		"7 - Get Vertex Degree" << endl <<
		"8 - Get Adjacent Vertices For Vertex" << endl <<
		"9 - Get Vertex Eccentricity" << endl <<
		"10 - Show Graph" << endl <<
		"11 - Show All Graph Info" << endl <<
		"12 - Save Graph Info To File" << endl <<
		"13 - Exit" << endl <<
		endl;
		
	} while (cin >> userCommand);
	
    return 0;
}
