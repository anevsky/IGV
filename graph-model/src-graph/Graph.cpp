/*
 
 Class implementation:  Graph (Graph.cpp)
 
 Author:                Alex Nevsky
 Created:               21.11.2009
 Email:                 anwer.man@gmail.com 
 WebSite:               http://alexnevsky.com
 
 ...
 
 Copyright 2009 Alex A. Nevsky. All rights reserved.
 
 */

#include <algorithm>

#include "Graph.h"
#include "File.h"

//--------------------------------------------------------------
//
//--------------------------------------------------------------
const Graph::t_Edge Graph::SetEdge(
  const Graph::t_Vertex& input_Vertex1, 
  const Graph::t_Vertex& input_Vertex2,
  const Graph::t_EdgeCost& input_Cost,
  Graph::t_Edge& modify_Edge
) 
{
	std::pair<t_Vertex, t_Vertex> vertices;
	vertices.first = input_Vertex1;
	vertices.second = input_Vertex2;
	modify_Edge.first = vertices;
	
	t_EdgeCost cost(input_Cost);
	modify_Edge.second = cost;
	
	return modify_Edge;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
const Graph::t_Vertex& 
Graph::GetEdgeVertexFirst(const Graph::t_Edge& input_Edge) 
{
	return input_Edge.first.first;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
const Graph::t_Vertex& 
Graph::GetEdgeVertexSecond(const Graph::t_Edge& input_Edge) 
{
	return input_Edge.first.second;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
const Graph::t_EdgeCost& 
Graph::GetEdgeCost(const Graph::t_Edge& input_Edge) 
{
	return input_Edge.second;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
void Graph::SetFromFile(const std::string input_FileName) {
	File file;
	file.Read(input_FileName);
	File::t_FileData fileData;
	file.GetData(fileData);
	
	if (m_Vertices.size()) {
		std::cerr << "Oops!.. Graph already created!" << std::endl;
		return;
	}
	
	std::string data;
	for (File::t_FileData::iterator iter = fileData.begin(); 
		 iter != fileData.end(); ++iter) {
		if (*iter != '\n') {
			data.push_back(*iter);
		}
		
		if (*iter == '\n' || iter+1 == fileData.end()) {
			const File::t_FileData VERTICES_DELIMITER = "--";
			File::t_FileData::size_type posVerticesDelimiter = 
			data.find(VERTICES_DELIMITER);
			
			const File::t_FileData EDGE_COST_DELIMITER = "$";
			File::t_FileData::size_type posEdgeCostDelimiter = 
			data.find(EDGE_COST_DELIMITER);
			if (posEdgeCostDelimiter == File::t_FileData::npos) {
				posEdgeCostDelimiter = 0;
			} 
			
			t_Vertex 
			vertex1(data, 0, posVerticesDelimiter), 
			vertex2(data, posVerticesDelimiter+2, 
					posEdgeCostDelimiter-posVerticesDelimiter-2);
			
			t_EdgeCost edgeCost;
			if (posEdgeCostDelimiter != 0) {
				File::t_FileData cost(data, posEdgeCostDelimiter+1);
				edgeCost = atof(cost.c_str());
				if (edgeCost < 0) {
					m_NegativeEdges = true;
				}
			} else {
				edgeCost = 1;
			}
			
			data.clear();
			
			// set m_Vertices
			m_Vertices.insert(vertex1);
			m_Vertices.insert(vertex2);
			
			// set m_Edges
			t_Edge edge;
			SetEdge(vertex1, vertex2, edgeCost, edge);
			if (std::count(m_Edges.begin(), m_Edges.end(), edge) == false) {
				m_Edges.push_back(edge);
			}
			
			// reverse edge for undirected graph
			if (m_Directed == false && vertex1 != vertex2) {
				t_Edge reverseEdge;
				SetEdge(vertex2, vertex1, edgeCost, reverseEdge);
				if (std::count(m_Edges.begin(), m_Edges.end(), reverseEdge) == false) {
					m_Edges.push_back(reverseEdge);
				}
			}
		}
	}
	
	EdgesOrdering();
	FindVerticesDegree();
	FindVerticesAdjacent();
	FindEdgesAdjacent();
	FindVerticesEccentricity();
	FindRadius();
	FindCenter();
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
void Graph::EdgesOrdering(void) 
{
	m_Edges.sort();
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
bool Graph::IsVertexInEdge(
  const Graph::t_Edge& input_Edge, 
  const Graph::t_Vertex& input_Vertex
)
{
	if (GetEdgeVertexFirst(input_Edge) == input_Vertex) {
		return true;
	} else {
		return false;
	}

}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
void Graph::FindVerticesDegree(void) 
{
	for (
		 std::set<t_Vertex>::iterator vertexIter = m_Vertices.begin(); 
		 vertexIter != m_Vertices.end(); 
		 ++vertexIter
		 )
	{
		t_Degree vertexDegree(0);
		for (
			 std::list<t_Edge>::iterator edgeIter = m_Edges.begin(); 
			 edgeIter != m_Edges.end(); 
			 ++edgeIter
			 ) 
		{
			if (IsVertexInEdge(*edgeIter, *vertexIter)) {
				++vertexDegree;
				} else {
					// если степень нужной вершины нашли, 
					// другие ребра просматривать не нужно
					if (vertexDegree) {
						break;
					}
				}

		}
		m_VerticesDegree[*vertexIter] = vertexDegree;
		vertexDegree = 0;		
	}
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
void Graph::FindVerticesAdjacent(void) 
{
	std::set<t_Vertex> adjacentVertices;
	for (
		 std::set<t_Vertex>::iterator vertexIter = m_Vertices.begin(); 
		 vertexIter != m_Vertices.end(); 
		 ++vertexIter
		 )
	{
		for (
			 std::list<t_Edge>::iterator edgeIter = m_Edges.begin(); 
			 edgeIter != m_Edges.end(); 
			 ++edgeIter
			 ) 
		{
			if (IsVertexInEdge(*edgeIter, *vertexIter)) {
				t_Edge edge(*edgeIter);
				adjacentVertices.insert(GetEdgeVertexSecond(edge));
			} else {
				// если смежные вершины для нужной вершины нашли, 
				// другие ребра просматривать не нужно 
				if (adjacentVertices.size()) {
					break;
				}
			}
		}
		m_AdjacentForVertexVertices[*vertexIter] = adjacentVertices;
		adjacentVertices.clear();
	}
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
void Graph::FindEdgesAdjacent(void) 
{
	std::list<t_Edge> adjacentEdges;
	for (
		 std::set<t_Vertex>::iterator vertexIter = m_Vertices.begin(); 
		 vertexIter != m_Vertices.end(); 
		 ++vertexIter
		 )
	{
		for (
			 std::list<t_Edge>::iterator edgeIter = m_Edges.begin(); 
			 edgeIter != m_Edges.end(); 
			 ++edgeIter
			 ) 
		{
			if (IsVertexInEdge(*edgeIter, *vertexIter)) {
				t_Edge edge(*edgeIter);
				adjacentEdges.push_back(edge);
			} else {
				// если смежные ребра для нужной вершины нашли, 
				// другие ребра просматривать не нужно 
				if (adjacentEdges.size()) {
					break;
				}
			}
		}
		m_AdjacentForVertexEdges[*vertexIter] = adjacentEdges;
		adjacentEdges.clear();
	}
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
void Graph::FindVerticesEccentricity(void)
{
	if (m_NegativeEdges == false) {
		for (
			 std::set<t_Vertex>::const_iterator iter = m_Vertices.begin();
			 iter != m_Vertices.end(); 
			 ++iter
			 ) 
		{
			DijkstraAlgorithm(*iter);
		}
	} else {
		std::cerr << "Oops!.. Unfortunately, Graph::FindVerticesEccentricity(void) \
not implemented in this version of the program for this input data with negative \
edges!" << std::endl;
	}
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
void Graph::FindRadius(void)
{
	const double INFINITY(10e12);
	
	t_Radius radius(INFINITY);
	
	for (
		 std::set<t_Vertex>::const_iterator iter = m_Vertices.begin();
		 iter != m_Vertices.end(); 
		 ++iter
		 ) 
	{
		if (GetMaxVertexEccentricity(*iter) < radius) {
			radius = GetMaxVertexEccentricity(*iter);
		}
	}
	
	m_Radius = radius;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
const Graph::t_Vertex 
Graph::GetNearestVertex(const t_Vertex& input_Vertex) 
{
	const double INFINITY(10e12);
	
	std::list<t_Edge> adjacentEdges = GetAdjacentEdges(input_Vertex);
	
	t_EdgeCost minEdgeCost(INFINITY), edgeCost;
	
	t_Vertex nearestVertex;
	
	for (
		 std::list<t_Edge>::const_iterator listIter = adjacentEdges.begin(); 
		 listIter != adjacentEdges.end(); 
		 ++listIter
		 ) 
	{
		edgeCost = GetEdgeCost(*listIter);
		if (edgeCost < minEdgeCost) {
			minEdgeCost = edgeCost;
			nearestVertex = GetEdgeVertexSecond(*listIter);
		}
	}
	
	return nearestVertex;
}

//--------------------------------------------------------------
// Алгоритм Дейкстры
//
// Поиск кратчайшего пути из одной вершины исходного графа во все другие 
// 
// ОГРАНИЧЕНИЯ:
// - Отсутствие в исходном графе ребер с отрицательным весом
//--------------------------------------------------------------
void Graph::DijkstraAlgorithm(const Graph::t_Vertex& input_Vertex) 
{
	size_t someNodesComputed(m_Vertices.size());
	
	do {
		--someNodesComputed;
		
		const double INFINITY(10e12);
		
		t_Vertex source = input_Vertex;
		
		std::set<t_Vertex> unVisitedVertices = m_Vertices;
		
		std::map<t_Vertex, t_Eccentricity> vertexEccentricity;
		
		for (
			 std::set<t_Vertex>::iterator iter = unVisitedVertices.begin();
			 iter != unVisitedVertices.end(); 
			++iter
			) 
		{
			t_Eccentricity minEccentricity(INFINITY);
			vertexEccentricity[*iter] = minEccentricity;
		}
		
		vertexEccentricity[source] = 0;
		
		unVisitedVertices.erase(source);
		
		t_Vertex currentVertex(source);
		
		t_Vertex nextVertex;
		
		while (unVisitedVertices.size()) {
			
			if (true) {
				
				std::list<t_Edge> adjacentEdgesForCurrentVertex = 
				GetAdjacentEdges(currentVertex);
				
				std::set<t_Vertex> unUsedVertices;
				
				for (
					 std::list<t_Edge>::iterator iter = 
					 adjacentEdgesForCurrentVertex.begin(); 
					 iter != adjacentEdgesForCurrentVertex.end(); 
					 ++iter
					 ) 
				{
					t_Vertex secondVertex = GetEdgeVertexSecond(*iter);
					
					if (unVisitedVertices.count(secondVertex) == 1) {
						unUsedVertices.insert(secondVertex);
						
						t_Eccentricity currentVertexCost;
						
						currentVertexCost = vertexEccentricity[secondVertex];
						
						if (
							(vertexEccentricity[GetEdgeVertexFirst(*iter)] 
							 + GetEdgeCost(*iter)) < currentVertexCost
							) 
						{
							vertexEccentricity[secondVertex] = 
							(vertexEccentricity[GetEdgeVertexFirst(*iter)] 
							+ GetEdgeCost(*iter));
						}
					}
				}
			}
		
			unVisitedVertices.erase(currentVertex);
			
			if (true) {
				// получить ближайшую непросмотренную вершину к истоку
				std::map<t_Vertex, t_Eccentricity> ecMap;
				
				ecMap = m_VerticesEccentricity[source];
				
				t_Eccentricity minEccentricity(INFINITY);
				
				for (
					 std::map<t_Vertex, t_Eccentricity>::iterator 
					 ecMapIter = ecMap.begin(); 
					 ecMapIter != ecMap.end(); 
					 ++ecMapIter
					 ) 
				{
					if (
						(unVisitedVertices.count(ecMapIter->first) > 0)
						&& (ecMapIter->second < minEccentricity)
						) 
					{
						minEccentricity = ecMapIter->second;
						nextVertex = ecMapIter->first;
					}
				}
			}
			
			// повторить для следующей
			if (currentVertex == nextVertex) {
				break;
			} else {
				currentVertex = nextVertex;
			}
			
		}
		
		SetVertexEccentricity(source, vertexEccentricity);
		
	} while (someNodesComputed);
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
void Graph::FindCenter(void) 
{
	if (m_Center.known == false) {
		
		for (
			 std::set<t_Vertex>::const_iterator iter = m_Vertices.begin();
			 iter != m_Vertices.end(); 
			 ++iter
			 ) 
		{
			if (GetMaxVertexEccentricity(*iter) == m_Radius) {
				m_Center.vertices.insert(*iter);
			}
		}
		
		for (
			 std::set<t_Vertex>::const_iterator iterV = m_Center.vertices.begin();
			 iterV != m_Center.vertices.end(); 
			 ++iterV
			 ) 
		{
			std::list<t_Edge> adjacentEdges = GetAdjacentEdges(*iterV);
			
			for (
				 std::list<t_Edge>::const_iterator iterE = adjacentEdges.begin(); 
				 iterE != adjacentEdges.end(); 
				 ++iterE
				 ) 
			{
				if (m_Center.vertices.count(GetEdgeVertexSecond(*iterE)) > 0) {
					m_Center.edges.push_back(*iterE);
				}
			}
		}
		
		m_Center.edges.sort();
		
		FindMaxDistanceBetweenCenter();
		
		m_Center.known = true;
	}
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
void Graph::FindMaxDistanceBetweenCenter(void)
{
	const double INFINITY(10e12);
	
	t_Distance maxDistance(-INFINITY);
	
	for (
		 std::set<t_Vertex>::const_iterator iterV1 = m_Center.vertices.begin();
		 iterV1 != m_Center.vertices.end(); 
		 ++iterV1
		 ) 
	{
		for (
			 std::set<t_Vertex>::const_iterator iterV2 = m_Center.vertices.begin();
			 iterV2 != m_Center.vertices.end(); 
			 ++iterV2
			 ) 
		{
			if (GetDistanceFromVertexToVertex(*iterV1, *iterV2) > maxDistance) {
				maxDistance = GetDistanceFromVertexToVertex(*iterV1, *iterV2);
			}
		}
	}
	
	m_Center.maxDistanceBetweenCenter = maxDistance;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
void Graph::SetVertexEccentricity(
  Graph::t_Vertex input_Vertex, 
  std::map<Graph::t_Vertex, Graph::t_Eccentricity> input_VertexEccentricity
)
{	
	m_VerticesEccentricity[input_Vertex] = input_VertexEccentricity;
}



//--------------------------------------------------------------
//
//--------------------------------------------------------------
const std::map<Graph::t_Vertex, Graph::t_Eccentricity>  
Graph::GetVertexEccentricity(const Graph::t_Vertex& input_Vertex) 
{
	if (m_VerticesEccentricity.count(input_Vertex)) {
		return m_VerticesEccentricity[input_Vertex];
	} else {
		std::map<t_Vertex, t_Eccentricity> inCorrect;
		return inCorrect;
	}
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
const Graph::t_Eccentricity 
Graph::GetMaxVertexEccentricity(const Graph::t_Vertex& input_Vertex)
{
	const double INFINITY(10e12);
	
	t_Eccentricity maxEccentricity(-INFINITY);
	
	std::map<t_Vertex, t_Eccentricity> ecMap;
	
	ecMap = m_VerticesEccentricity[input_Vertex];
	
	for (
		 std::map<t_Vertex, t_Eccentricity>::iterator 
		 ecMapIter = ecMap.begin(); 
		 ecMapIter != ecMap.end(); 
		 ++ecMapIter
		 ) 
	{
		if (ecMapIter->second > maxEccentricity) 
		{
			maxEccentricity = ecMapIter->second;
		}
	}
	
	return maxEccentricity;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
const Graph::t_Radius Graph::GetRadius(void)
{
	return m_Radius;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
const Graph::t_Distance 
Graph::GetDistanceFromVertexToVertex(									  
  const Graph::t_Vertex& input_From, 
  const Graph::t_Vertex& input_To
)
{
	if (m_VerticesEccentricity.count(input_From)) {
		std::map<t_Vertex, t_Eccentricity>
		ecMap = m_VerticesEccentricity[input_From];
		
		return ecMap[input_To];
	} else {
		t_Eccentricity inCorrect;
		return inCorrect;
	}

}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
const Graph::t_Path Graph::GetShortestPath(
  const Graph::t_Vertex& input_Vertex1, 
  const Graph::t_Vertex& input_Vertex2
)
{
	const double INFINITY(10e12);
	
	t_Distance distance = GetDistanceFromVertexToVertex(input_Vertex1, input_Vertex2);
	
	if (
		distance == INFINITY ||
		m_Vertices.count(input_Vertex1) == 0 ||
		m_Vertices.count(input_Vertex2) == 0
	) 
	{
		std::cerr << "Is no way from '" << input_Vertex1 <<
		"' to '" << input_Vertex2 << "'." << std::endl;
		
		t_Path path;
		return path;
	}
	
	if (input_Vertex1 == input_Vertex2) {
		t_Path path(input_Vertex1);
		return path;
	}
	
	std::list<t_Path> way;
	
	if (true) {
		std::set<Graph::t_Vertex> adjacentVertices = 
		GetAdjacentVertices(input_Vertex2);
		
		way.push_back(input_Vertex2);
		
		--distance;
	}
	
	t_Vertex currentVertex;
	currentVertex = input_Vertex2;
	
	while (distance) {
		std::set<t_Vertex> adjacentVertices = 
		GetAdjacentVertices(currentVertex);
		
		for (
			 std::set<t_Vertex>::const_iterator setIter = 
			 adjacentVertices.begin(); 
			 setIter != adjacentVertices.end(); 
			 ++setIter
			 ) 
		{
			if (GetDistanceFromVertexToVertex(input_Vertex1, *setIter) == distance) {
				way.push_back(*setIter);
				currentVertex = (*setIter);
				break;
			}
		}
		--distance;
	}
	
	t_Path path;
	
	path += input_Vertex1;
	path += "--";
	
	size_t needDash = way.size()-1;
	
	for(
		std::list<t_Path>::reverse_iterator iter = way.rbegin(); 
		iter != way.rend(); 
		++iter
		) 
	{
		path += *iter;
		if (needDash) {
			--needDash;
			path += "--";
		}
	}
		
	return path;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
const Graph::t_Degree 
Graph::GetVertexDegree(const Graph::t_Vertex& input_Vertex) 
{
	std::map< t_Vertex, t_Degree >::iterator mapIter = 
	m_VerticesDegree.find(input_Vertex);
	if (mapIter != m_VerticesDegree.end()) {
		return mapIter->second;
	}
	return -1;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
const std::set<Graph::t_Vertex> 
Graph::GetAdjacentVertices(const Graph::t_Vertex& input_Vertex) 
{
	std::map< t_Vertex, std::set<t_Vertex> >::iterator mapIter = 
	m_AdjacentForVertexVertices.find(input_Vertex);
	if (mapIter != m_AdjacentForVertexVertices.end()) {
		return mapIter->second;
	} else {
		std::set<t_Vertex> inCorrect;
		return inCorrect;
	}
	
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
const std::list<Graph::t_Edge> 
Graph::GetAdjacentEdges(const Graph::t_Vertex& input_Vertex) 
{
	std::map< t_Vertex, std::list<t_Edge> >::iterator mapIter = 
	m_AdjacentForVertexEdges.find(input_Vertex);
	if (mapIter != m_AdjacentForVertexEdges.end()) {
		return mapIter->second;
	} else {
		std::list<t_Edge> inCorrect;
		return inCorrect;
	}
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
template <typename T>
void PrintValues(T beg, T end, std::ostream& out) 
{
	while (beg != end) {
		out << *beg++;
		if (beg != end) out << ", ";
	}
	out << std::endl;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
std::ostream& Graph::ShowAllInfo(std::ostream& output_Stream) 
{
	ShowGraph(output_Stream);
	ShowVerticesDegree(output_Stream);
	ShowAdjacentForVertexVertices(output_Stream);
	ShowAdjacentForVertexEdges(output_Stream);
	ShowVerticesEccentricity(output_Stream);
	return output_Stream;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
std::ostream& Graph::ShowGraph(std::ostream& output_Stream) 
{
	ShowVertices(output_Stream);
	ShowEdges(output_Stream);
	output_Stream << "Graph Radius:" << std::endl << GetRadius() << 
	std::endl << std::endl;
	ShowCenterVertices(output_Stream);
	ShowCenterEdges(output_Stream);
	ShowCenterMaxDistance(output_Stream);
	return output_Stream;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
std::ostream& Graph::ShowVertices(std::ostream& output_Stream) 
{
	output_Stream << "Graph Vertices (" << m_Vertices.size() << " pieces): " 
	<< std::endl;
	PrintValues(m_Vertices.begin(), m_Vertices.end(), output_Stream);
	output_Stream << std::endl;
	return output_Stream;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
std::ostream& Graph::ShowEdges(std::ostream& output_Stream) 
{
	output_Stream << "Graph Edges (" << m_Edges.size() << " pieces): " << std::endl;
	for (
		 std::list<t_Edge>::iterator iter = m_Edges.begin(); 
		 iter != m_Edges.end(); 
		 ++iter
		 ) 
	{
		output_Stream << GetEdgeVertexFirst(*iter) << " -- " 
		<< GetEdgeVertexSecond(*iter) 
		<< ", cost: $" << GetEdgeCost(*iter) << std::endl;
	}
	output_Stream << std::endl;
	return output_Stream;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
std::ostream& Graph::ShowVerticesDegree(std::ostream& output_Stream) 
{
	output_Stream << "Vertices Degree: " << std::endl;
	for (
		 std::map< t_Vertex, t_Degree >::iterator iter = m_VerticesDegree.begin(); 
		 iter != m_VerticesDegree.end(); 
		 ++iter
		 ) 
	{
		output_Stream << iter->first << ", degree: " << 
		iter->second << std::endl;
	}
	output_Stream << std::endl;
	return output_Stream;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
std::ostream& Graph::ShowAdjacentForVertexVertices(std::ostream& output_Stream) 
{
	output_Stream << "Adjacent Vertices For Vertex: " << std::endl;
	for (
		 std::map< t_Vertex, std::set<t_Vertex> >::iterator 
		 mapIter = m_AdjacentForVertexVertices.begin(); 
		 mapIter != m_AdjacentForVertexVertices.end(); 
		 ++mapIter
		 ) 
	{
		output_Stream << mapIter->first << ", adjacent vertices (" 
		<< mapIter->second.size() << " pieces): ";
		const std::set<t_Vertex>& vertexSet(mapIter->second);
		for (
			 std::set<t_Vertex>::const_iterator setIter = vertexSet.begin(); 
			 setIter != vertexSet.end(); 
			 ++setIter
			 ) 
		{
			output_Stream << *setIter << ", ";
		}
		output_Stream << std::endl;
	}
	output_Stream << std::endl;
	return output_Stream;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
std::ostream& Graph::ShowAdjacentForVertexEdges(std::ostream& output_Stream) 
{
	output_Stream << "Adjacent Edges For Vertex: " << std::endl;
	for (
		 std::map< t_Vertex, std::list<t_Edge> >::iterator 
		 mapIter = m_AdjacentForVertexEdges.begin(); 
		 mapIter != m_AdjacentForVertexEdges.end(); 
		 ++mapIter
		 ) 
	{
		output_Stream << mapIter->first << ", adjacent edges ("
		<< mapIter->second.size() << " pieces): ";
		const std::list<t_Edge>& edgesList(mapIter->second);
		for (
			 std::list<t_Edge>::const_iterator listIter = edgesList.begin(); 
			 listIter != edgesList.end(); 
			 ++listIter
			 ) 
		{
			output_Stream << GetEdgeVertexFirst(*listIter) << "--"
			<< GetEdgeVertexSecond(*listIter) << "($" 
			<< GetEdgeCost(*listIter) << "), ";
		}
		output_Stream << std::endl;
	}
	output_Stream << std::endl;
	return output_Stream;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
std::ostream& Graph::ShowCenterVertices(std::ostream& output_Stream) 
{
	output_Stream << "Center Vertices (" << m_Center.vertices.size() << " pieces): " 
	<< std::endl;
	PrintValues(m_Center.vertices.begin(), m_Center.vertices.end(), output_Stream);
	output_Stream << std::endl;
	return output_Stream;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
std::ostream& Graph::ShowCenterEdges(std::ostream& output_Stream) 
{
	output_Stream << "Center Edges (" << m_Center.edges.size() << " pieces): " 
	<< std::endl;
	for (
		 std::list<t_Edge>::iterator iter = m_Center.edges.begin(); 
		 iter != m_Center.edges.end(); 
		 ++iter
		 ) 
	{
		output_Stream << GetEdgeVertexFirst(*iter) << " -- " 
		<< GetEdgeVertexSecond(*iter) << ", cost: $" << GetEdgeCost(*iter) 
		<< std::endl;
	}
	output_Stream << std::endl;
	return output_Stream;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
std::ostream& Graph::ShowCenterMaxDistance(std::ostream& output_Stream) 
{
	output_Stream << "Max Distance Between Center: " << std::endl;
	
	output_Stream << m_Center.maxDistanceBetweenCenter << std::endl;
	
	output_Stream << std::endl;
	return output_Stream;
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
std::ostream& Graph::ShowVerticesEccentricity(std::ostream& output_Stream) 
{
	output_Stream << "Eccentricity:" << std::endl;
	for (
		 t_VertexEccentricity::iterator 
		 mapIter = m_VerticesEccentricity.begin(); 
		 mapIter != m_VerticesEccentricity.end(); 
		 ++mapIter
		 ) 
	{
		std::map<t_Vertex, t_Eccentricity> ecMap = mapIter->second;
		
		for (
			 std::map<t_Vertex, t_Eccentricity>::iterator 
			 ecMapIter = ecMap.begin(); 
			 ecMapIter != ecMap.end(); 
			 ++ecMapIter
			 ) 
		{
			output_Stream << "From vertex '" << mapIter->first << "' to vertex '" 
			<< ecMapIter->first << "': " << ecMapIter->second << std::endl;
		}
	}
	output_Stream << std::endl;
	
	return output_Stream;
}
