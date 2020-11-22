#pragma once
#include "AABB.h"
/*Base class for living entites*/
class EntityLiving
{
protected:
	/*each positional value has a previous tick value for interpolation*/
	float rotation, prevRotation, moveAcel;
	Vector2 pos, prevPos, vel{ 0,0 }, acel{ 0,0 }, frontVector{ 0,0 }, prevFrontvector{0,0};
	AABB boundingBox = AABB(-7, -7, 7, 7);
	/*aligns front vector to rotation of entity*/
	void alignFrontVector();

	float radians(float degrees);
	
	float rotationSpeed = 5.0F;
    float moveResistance = 0.1F;
public:
	EntityLiving(float x, float y, float rotation);
	EntityLiving();

	/*called every tick to do stuff to entity*/
	virtual void onTick();

	void setPos(float x, float y) { pos.x = x; pos.y = y; boundingBox.translate({x,y}); }
	void setPos(Vector2 newPos) { pos = newPos; boundingBox.translate(newPos); }

	void setRotation(float r) { rotation = r; }

	/*makes this entity walk in the direction it is facing at the speed of moveAcel*/
	void walkFowards();

	float getPosX() const { return pos.x; }
	float getPosY() const { return pos.y; }
	Vector2 getPos() const { return pos; }
	float getRotation() const { return rotation; }

	float getLerpPosX() const;//returns the posX of this entity linearly interpolated between ticks
	float getLerpPosY() const;//returns the posY of this entity linearly interpolated between ticks
	Vector2 getLerpPos() const;//returns the posY of this entity linearly interpolated between ticks
	float getLerpRotation() const;//returns the rotation of this entity linearly interpolated between ticks

	Vector2 getFrontVec() const { return frontVector; }
	Vector2 getLerpFrontVec() const;//returns the front vector of this entity, interpolated between ticks.

	AABB* getAABB() { return &boundingBox; };
};