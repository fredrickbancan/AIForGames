#pragma once
#include "raymath.h"
#include "AABB.h"
#include <vector>
#include <list>
struct NavNode
{
	NavNode* linkTop = nullptr;
	NavNode* linkRight = nullptr;
	NavNode* linkBottom = nullptr;
	NavNode* linkLeft = nullptr;
	NavNode* parent = nullptr;//previous node used for generating paths. reset each time a path is requested.
	int pathCost = INT16_MAX;//cost of the path to this node. reset each time a path is requested.
	Vector2 pos{ 0,0 };
};

/*Class containing a graph of navigation nodes*/
class NodeGraph
{
private:
	int nodesWidth = 0;
	int nodesHeight = 0;
	float nodeSpacing = 0.0F;
	
	NavNode* nodes;//array of all nodes
	bool* closedList;//bool array for use as a closed list when finding path
	/*returns nearest node at the provided coordinates*/
	NavNode* getNodeAt(int x, int y);

	/*returns true if the provided nodes are not obstructed by a wall in the vector*/
	bool canNodeLinkToNode(NavNode* nodeA, NavNode* nodeB);

	/*returns true if the provided node has atleast 1 neightbor, useful for finding if
	  a node can be navigated to or not.*/
	bool doesNodeHaveNeighbor(NavNode* node);

	/*resets all the prevNavNode pointers and pathCost values to their defaults,
	  also resets the closedList booleans*/
	void prepareNodesForPath();

	/*returns the index of the provided nav node*/
	int getIndexOfNode(NavNode* node);

	/*takes in one of the links to the current node being processed in the path find and
	  determines if it has been visited and has a shorter path cost, and can add it to the
	  open list*/
	void processNodeLinkForPath(NavNode* currentNode, NavNode* connection, std::list<NavNode*>& openList);
public:
	NodeGraph(int nodesWide, int nodesHight, float nodeSpacing);
	~NodeGraph() { delete[] nodes; delete[] closedList; }

	/*Links all the nodes depending on if they can connect without a wall in the way*/
	void linkNodes();

	/*returns nearest valid node at the provided position*/
	NavNode* getNodeAtPos(float x, float y);

	/*returns an array of NavNode pointers which should be navigated to in order to reach the
	  endPos position. count will be assigned the count of nodes in the node pointer array.
	  When path is no longer being used, this pointer array must be de-allocated by the user.*/
	NavNode** getShortestPathDijkstras(Vector2 startPos, Vector2 endPos, int& pathCount);

	/*renders all the nodes as small squares, and their connections as lines.
	  Node closest to provided player position will be colored gold.*/
	void debugDrawNodes(Vector2 playerPos);

	

	int getCount() { return nodesWidth * nodesHeight; };
};