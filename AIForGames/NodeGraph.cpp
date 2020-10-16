#include "NodeGraph.h"
#include "raylib.h"
#include "Ray2D.h"
#include "Game.h"

bool NodeGraph::doesNodeHaveNeighbor(NavNode* node)
{
	return node->linkTop != nullptr || node->linkRight != nullptr || node->linkBottom != nullptr || node->linkLeft != nullptr;
}

NodeGraph::NodeGraph(int nodesWide, int nodesHight, float nodeSpacing)
{
	nodesWidth = nodesWide;
	nodesHeight = nodesHight;
	this->nodeSpacing = nodeSpacing;
	nodes = new NavNode[nodesWidth * nodesHeight];

	//initializing nodes
	float xOffset = 0;
	float yOffset = 0;
	for (int x = 0; x < nodesWidth; x++)
	{
		for (int y = 0; y < nodesHeight; y++)
		{
			getNodeAt(x,y)->pos.x = xOffset;
			getNodeAt(x,y)->pos.y = yOffset;

			yOffset += nodeSpacing;
			if (yOffset / nodeSpacing >= nodesHeight)
			{
				yOffset = 0;
			}
		}
		xOffset += nodeSpacing;
		if (xOffset / nodeSpacing >= nodesWidth)
		{
			xOffset = 0;
		}
	}
}

NavNode* NodeGraph::getNodeAt(int x, int y)
{
	/*if the provided coords is out of range*/
	if ((x * nodesHeight) + y > nodesWidth * nodesHeight)
	{
		return nullptr;
	}

	return &nodes[(x * nodesHeight) + y];
}

NavNode* NodeGraph::getNodeAtPos(float x, float y)
{
	NavNode* bestNodeResult = nullptr;
	//testing closest node to see if it is valid (if it can be navigated to), otherwise, find closest valid node
	bestNodeResult = getNodeAt(roundf(x / nodeSpacing), roundf(y / nodeSpacing));
	
	if (!doesNodeHaveNeighbor(bestNodeResult))//if the current result isnt valid
	{
		bestNodeResult = getNodeAt(roundf(x / nodeSpacing) - 1, roundf(y / nodeSpacing));//try the node to the left
	}

	return bestNodeResult;
}

void NodeGraph::linkNodes()
{
	NavNode* nodeA;
	NavNode* nodeB;
	for (int x = 0; x < nodesWidth; x++)
	{
		for (int y = 0; y < nodesHeight; y++)
		{
			nodeA = getNodeAt(x, y);
			//link top node
			if ((nodeB = getNodeAt(x, y + 1)) != nullptr)
			{
				if (canNodeLinkToNode(nodeA, nodeB))
				{
					nodeA->linkTop = nodeB;
				}
			}

			//link right node
			if ((nodeB = getNodeAt(x + 1, y)) != nullptr)
			{
				if (canNodeLinkToNode(nodeA, nodeB))
				{
					nodeA->linkRight = nodeB;
				}
			}

			//link bottom node
			if ((nodeB = getNodeAt(x, y - 1)) != nullptr)
			{
				if (canNodeLinkToNode(nodeA, nodeB))
				{
					nodeA->linkBottom = nodeB;
				}
			}

			//link left node
			if ((nodeB = getNodeAt(x - 1, y)) != nullptr)
			{
				if (canNodeLinkToNode(nodeA, nodeB))
				{
					nodeA->linkLeft = nodeB;
				}
			}
		}
	}
}

bool NodeGraph::canNodeLinkToNode(NavNode* nodeA, NavNode* nodeB)
{
	return !Game::get()->doesRayHitWall(Ray2D(nodeA->pos, Vector2Subtract(nodeB->pos, nodeA->pos)));
}


void NodeGraph::debugDrawNodes(Vector2 playerPos)
{
	for (int i = 0; i < (nodesWidth * nodesHeight); i++)
	{
		NavNode* currentNode = &nodes[i];
		if (getNodeAtPos(playerPos.x, playerPos.y) == currentNode)
		{
			DrawRectangle(currentNode->pos.x - 2, currentNode->pos.y - 2, 4, 4, GOLD);
		}
		else
		{
			DrawRectangle(currentNode->pos.x - 2, currentNode->pos.y - 2, 4, 4, DARKGREEN);
		}
		if (currentNode->linkTop != nullptr)
		{
			DrawLine(currentNode->pos.x, currentNode->pos.y, currentNode->pos.x, currentNode->pos.y + nodeSpacing, DARKPURPLE);
		}
		if (currentNode->linkRight != nullptr)
		{
			DrawLine(currentNode->pos.x, currentNode->pos.y, currentNode->pos.x + nodeSpacing, currentNode->pos.y, DARKPURPLE);
		}
		if (currentNode->linkBottom != nullptr)
		{
			DrawLine(currentNode->pos.x, currentNode->pos.y, currentNode->pos.x, currentNode->pos.y - nodeSpacing, DARKPURPLE);
		}
		if (currentNode->linkLeft != nullptr)
		{
			DrawLine(currentNode->pos.x, currentNode->pos.y, currentNode->pos.x - nodeSpacing, currentNode->pos.y, DARKPURPLE);
		}
	}
}
