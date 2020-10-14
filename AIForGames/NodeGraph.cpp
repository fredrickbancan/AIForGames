#include "NodeGraph.h"
#include "raylib.h"
#include "Ray2D.h"

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
	return getNodeAt((int)x / nodeSpacing, (int)y / nodeSpacing);
}

void NodeGraph::linkNodes(const std::vector<AABB>* walls)
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
				if (canNodeLinkToNode(nodeA, nodeB, walls))
				{
					nodeA->linkTop = nodeB;
				}
			}

			//link right node
			if ((nodeB = getNodeAt(x + 1, y)) != nullptr)
			{
				if (canNodeLinkToNode(nodeA, nodeB, walls))
				{
					nodeA->linkRight = nodeB;
				}
			}

			//link bottom node
			if ((nodeB = getNodeAt(x, y - 1)) != nullptr)
			{
				if (canNodeLinkToNode(nodeA, nodeB, walls))
				{
					nodeA->linkBottom = nodeB;
				}
			}

			//link left node
			if ((nodeB = getNodeAt(x - 1, y)) != nullptr)
			{
				if (canNodeLinkToNode(nodeA, nodeB, walls))
				{
					nodeA->linkLeft = nodeB;
				}
			}
		}
	}
}

bool NodeGraph::canNodeLinkToNode(NavNode* nodeA, NavNode* nodeB, const std::vector<AABB>* walls)
{
	bool obscured = false;
	Ray2D rayFromAToB;
	for (std::vector<AABB>::const_iterator wall = walls->begin(); wall != walls->end(); wall++)
	{
		//if ray from node a to node b intersects any wall, obscured = true	
		if (intersecting((rayFromAToB = Ray2D( nodeA->pos, Vector2Subtract(nodeB->pos, nodeA->pos))), *wall))
		{
			obscured = true;
		}
	}
	return !obscured;
}

bool NodeGraph::intersecting(Ray2D ray, AABB box)
{
	//if ray is axis aligned, do these, faster and avoids divide by 0
	////
	if (ray.direction.x == 0)//ray points straight y
	{
		if (ray.origin.x < box.minBounds.x || ray.origin.x > box.maxBounds.x)//if the ray is missing the box in the y direction
		{
			return false;
		}

		if (ray.direction.y < 0)//if ray pointing down
		{
			//ray must start above or inside the box and the distance from ray origin to box must be shorter than length
			return (ray.origin.y > box.minBounds.y) && (box.maxBounds.y - ray.origin.y <= ray.length);
		}
		else
		{
			//ray must start below or inside the box and the distance from ray origin to box must be shorter than length
			return (ray.origin.y < box.maxBounds.y) && (box.minBounds.y - ray.origin.y <= ray.length);
		}
	}

	if (ray.direction.y == 0)//ray points straight x
	{
		if (ray.origin.y < box.minBounds.y || ray.origin.y > box.maxBounds.y)//if the ray is missing the box in the y direction
		{
			return false;
		}
		if (ray.direction.x < 0)//if ray pointing left
		{
			//ray must start to right or inside the box and the distance from ray origin to box must be shorter than length
			return (ray.origin.x > box.minBounds.x) && (box.maxBounds.x - ray.origin.x <= ray.length);
		}
		else
		{
			//ray must start to left or inside the box and the distance from ray origin to box must be shorter than length
			return (ray.origin.x < box.maxBounds.x) && (box.minBounds.x - ray.origin.x <= ray.length);
		}
	}
	////

	//distances from ray origin to each axis of box
	float xMin = 0, xMax = 0, yMin = 0, yMax = 0;

	//getting distances
	////
	if (ray.direction.x < 0)
	{
		if (ray.origin.x < box.minBounds.x)//testing if box is behind ray
		{
			return false;
		}
		xMin = (box.maxBounds.x - ray.origin.x) / ray.direction.x;
		xMax = (box.minBounds.x - ray.origin.x) / ray.direction.x;
	}
	else
	{
		if (ray.origin.x > box.maxBounds.x)//testing if box is behind ray
		{
			return false;
		}
		xMin = (box.minBounds.x - ray.origin.x) / ray.direction.x;
		xMax = (box.maxBounds.x - ray.origin.x) / ray.direction.x;
	}

	if (ray.direction.y < 0)
	{
		if (ray.origin.y < box.minBounds.y)//testing if box is behind ray
		{
			return false;
		}
		yMin = (box.maxBounds.y - ray.origin.y) / ray.direction.y;
		yMax = (box.minBounds.y - ray.origin.y) / ray.direction.y;
	}
	else
	{
		if (ray.origin.y > box.maxBounds.y)//testing if box is behind ray
		{
			return false;
		}
		yMin = (box.minBounds.y - ray.origin.y) / ray.direction.y;
		yMax = (box.maxBounds.y - ray.origin.y) / ray.direction.y;
	}
	////

	//ensure ray is hitting box
	if (xMin > yMax || yMin > xMax)
	{
		return false;
	}

	float firstContactDist = std::max(xMin, yMin);
	return firstContactDist >= 0 && firstContactDist <= ray.length;
}

Vector2 NodeGraph::getClosestPointOnRay(Ray2D ray, Vector2 testPoint)
{
	float rayLength = Vector2Length(ray.direction);
	Vector2 rayDirection = Vector2Normalize(ray.direction);
	Vector2 vecFromRayOriginToTestPoint = Vector2Subtract(testPoint, ray.origin);
	float dotProduct = Vector2DotProduct(vecFromRayOriginToTestPoint, rayDirection);
	float dotClamped = Clamp(dotProduct, 0, rayLength);
	return Vector2Scale(rayDirection, dotClamped);
}

void NodeGraph::debugDrawNodes()
{
	for (int i = 0; i < (nodesWidth * nodesHeight); i++)
	{
		NavNode currentNode = nodes[i];
		DrawRectangle(currentNode.pos.x - 2, currentNode.pos.y - 2, 4, 4, DARKGREEN);
		if (currentNode.linkTop != nullptr)
		{
			DrawLine(currentNode.pos.x, currentNode.pos.y, currentNode.pos.x, currentNode.pos.y + nodeSpacing, DARKPURPLE);
		}
		if (currentNode.linkRight != nullptr)
		{
			DrawLine(currentNode.pos.x, currentNode.pos.y, currentNode.pos.x + nodeSpacing, currentNode.pos.y, DARKPURPLE);
		}
		if (currentNode.linkBottom != nullptr)
		{
			DrawLine(currentNode.pos.x, currentNode.pos.y, currentNode.pos.x, currentNode.pos.y - nodeSpacing, DARKPURPLE);
		}
		if (currentNode.linkLeft != nullptr)
		{
			DrawLine(currentNode.pos.x, currentNode.pos.y, currentNode.pos.x - nodeSpacing, currentNode.pos.y, DARKPURPLE);
		}
	}
}
