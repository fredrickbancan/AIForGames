#pragma once
#include "raymath.h"
#include "AABB.h"
#include <vector>

struct NavNode
{
	NavNode* linkTop = nullptr;
	NavNode* linkRight = nullptr;
	NavNode* linkBottom = nullptr;
	NavNode* linkLeft = nullptr;
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

	/*returns nearest node at the provided coordinates*/
	NavNode* getNodeAt(int x, int y);

	/*returns true if the provided nodes are not obstructed by a wall in the vector*/
	bool canNodeLinkToNode(NavNode* nodeA, NavNode* nodeB);

	/*returns true if the provided node has atleast 1 neightbor, useful for finding if
	  a node can be navigated to or not.*/
	bool doesNodeHaveNeighbor(NavNode* node);
public:
	NodeGraph(int nodesWide, int nodesHight, float nodeSpacing);
	~NodeGraph() { delete[] nodes; }

	/*Links all the nodes depending on if they can connect without a wall in the way*/
	void linkNodes();

	/*returns nearest valid node at the provided position*/
	NavNode* getNodeAtPos(float x, float y);

	/*renders all the nodes as small squares, and their connections as lines.
	  Node closest to provided player position will be colored gold.*/
	void debugDrawNodes(Vector2 playerPos);
};