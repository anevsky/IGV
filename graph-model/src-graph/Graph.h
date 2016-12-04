/*
 
 Class:                 Graph (Graph.h)
 
 Author:                Alex Nevsky
 Created:               21.11.2009
 Email:                 anwer.man@gmail.com 
 WebSite:               http://alexnevsky.com
 
 ...
 
 Copyright 2009 Alex A. Nevsky. All rights reserved.
 
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>
#include <set>
#include <map>
#include <list>
#include <utility>

class Graph {
public:
	typedef std::string t_Vertex;
	typedef double t_EdgeCost;
	typedef std::pair< std::pair<t_Vertex, t_Vertex>, t_EdgeCost > t_Edge;
	typedef int t_Degree;
	typedef double t_Eccentricity;
	typedef double t_Distance;
	typedef double t_Radius;
	typedef std::string t_Path;
	typedef std::map< t_Vertex, std::map<t_Vertex, t_Eccentricity> > t_VertexEccentricity;
	
private:
	bool m_Directed;
	bool m_NegativeEdges; 
	std::set<t_Vertex> m_Vertices;
	std::list<t_Edge> m_Edges;
	
	// степени вершин
	std::map< t_Vertex, t_Degree > m_VerticesDegree;
	// смежные вершины для всех вершин
	std::map< t_Vertex, std::set<t_Vertex> > m_AdjacentForVertexVertices;
	// смежные ребра для всех вершин
	std::map< t_Vertex, std::list<t_Edge> > m_AdjacentForVertexEdges;
	// отклоненность вершин
	t_VertexEccentricity m_VerticesEccentricity;
	// радиус
	t_Radius m_Radius;
	
	struct Center {
		bool known;
		std::set<t_Vertex> vertices;
		std::list<t_Edge> edges;
		t_Distance maxDistanceBetweenCenter;
		
		Center() : vertices(), edges(), known(false), maxDistanceBetweenCenter(0) {}
	};
	
	Center m_Center;
	
	void EdgesOrdering(void);
	
	void DijkstraAlgorithm(const t_Vertex& input_Vertex);
	
	void FindVerticesDegree(void);
	void FindVerticesAdjacent(void);
	void FindEdgesAdjacent(void);
	void FindVerticesEccentricity(void);
	
	void FindRadius(void);
	
	void FindCenter(void);
	void FindMaxDistanceBetweenCenter(void);
	
	const t_Edge SetEdge(
	  const t_Vertex& input_Vertex1,
	  const t_Vertex& input_Vertex2, 
	  const t_EdgeCost& input_Cost, 
	  t_Edge& modify_Edge
	);
	
	void SetVertexEccentricity(
	  t_Vertex input_Vertex, 
	  std::map<t_Vertex, t_Eccentricity> input_VertexEccentricity
    );
	
	bool Graph::IsVertexInEdge(
	  const t_Edge& input_Edge, 
      const t_Vertex& input_Vertex
	);
	
	const t_Vertex& GetEdgeVertexFirst(const t_Edge& input_Edge);
	const t_Vertex& GetEdgeVertexSecond(const t_Edge& input_Edge);
	const t_EdgeCost& GetEdgeCost(const t_Edge& input_Edge);	
	
public:
	Graph() : m_Vertices(), m_Edges(), m_Directed(false), m_NegativeEdges(false) {}
	
	void SetFromFile(const std::string input_FileName);
	
	const t_Path GetShortestPath(
	  const t_Vertex& input_Vertex1, 
	  const t_Vertex& input_Vertex2
	);
	
	const t_Distance 
	GetDistanceFromVertexToVertex(									  
	  const t_Vertex& input_From, 
	  const t_Vertex& input_To
	);
	
	const t_Vertex GetNearestVertex(const t_Vertex& input_Vertex);
	
	const t_Radius GetRadius(void);
	
	const t_Degree GetVertexDegree(const t_Vertex& input_Vertex);
	const std::set<t_Vertex> GetAdjacentVertices(const t_Vertex& input_Vertex);
	const std::list<t_Edge> GetAdjacentEdges(const t_Vertex& input_Vertex);
	
	const std::map<t_Vertex, t_Eccentricity>
	GetVertexEccentricity(const t_Vertex& input_Vertex);
	
	const t_Eccentricity GetMaxVertexEccentricity(const t_Vertex& input_Vertex);
	
	std::ostream& ShowAllInfo(std::ostream& output_Stream);
	std::ostream& ShowGraph(std::ostream& output_Stream);
	std::ostream& ShowVertices(std::ostream& output_Stream);
	std::ostream& ShowEdges(std::ostream& output_Stream);
	std::ostream& ShowVerticesDegree(std::ostream& output_Stream);
	std::ostream& ShowAdjacentForVertexVertices(std::ostream& output_Stream);
	std::ostream& ShowAdjacentForVertexEdges(std::ostream& output_Stream);
	std::ostream& ShowCenterVertices(std::ostream& output_Stream);
	std::ostream& ShowCenterEdges(std::ostream& output_Stream);
	std::ostream& ShowCenterMaxDistance(std::ostream& output_Stream);
	std::ostream& ShowVerticesEccentricity(std::ostream& output_Stream);
};

#endif