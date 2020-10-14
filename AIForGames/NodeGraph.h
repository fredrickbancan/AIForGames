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

	/*returns nearest node at the provided position*/
	NavNode* getNodeAtPos(float x, float y);
	
	/*returns true if the provided nodes are not obstructed by a wall in the vector*/
	bool canNodeLinkToNode(NavNode* nodeA, NavNode* nodeB, const std::vector<AABB>* walls);

	/*returns true of the provided ray intersects with the provided box*/
	bool intersecting(struct Ray2D ray, AABB box);

	/*Returns a position vector along the ray closest to the provided point*/
	Vector2 getClosestPointOnRay(struct Ray2D ray, Vector2 testPoint);
public:
	NodeGraph(int nodesWide, int nodesHight, float nodeSpacing);
	~NodeGraph() { delete[] nodes; }

	/*Links all the nodes depending on if they can connect without a wall in the way*/
	void linkNodes(const std::vector<AABB>* walls);

	/*renders all the nodes as small squares, and their connections as lines*/
	void debugDrawNodes();
};