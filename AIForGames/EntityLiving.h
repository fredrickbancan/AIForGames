#pragma once
#include "AABB.h"
/*Base class for living entites*/
class EntityLiving
{
protected:
	/*each positional value has a previous tick value for interpolation*/
	float rotation, prevRotation, moveAcel;
	Vector2 pos, prevPos, vel{ 0,0 }, acel{ 0,0 }, frontVector{ 0,0 }, prevFrontvector{0,0};
	AABB boundingBox = AABB(-12.5F, -12.5F, 12.5F, 12.5F);
	/*aligns front vector to rotation of entity*/
	void alignFrontVector();

	float radians(float degrees);
	
public:
	static constexpr float moveResistance = 0.1F;
	EntityLiving(float x, float y, float rotation);
	EntityLiving();

	/*called every tick to do stuff to entity*/
	virtual void onTick();

	void setPos(float x, float y) { pos.x = x; pos.y = y; }
	void setPos(Vector2 newPos) { pos = newPos; }

	void setRotation(float r) { rotation = r; }

	float getPosX() const { return pos.x; }
	float getPosY() const { return pos.y; }
	Vector2 getPos() const { return pos; }
	float getRotation() const { return rotation; }

	float getLerpPosX() const;//returns the posX of this entity linearly interpolated between ticks
	float getLerpPosY() const;//returns the posY of this entity linearly interpolated between ticks
	Vector2 getLerpPos() const;//returns the posY of this entity linearly interpolated between ticks
	float getLerpRotation() const;//returns the rotation of this entity linearly interpolated between ticks

	Vector2 getFrontVec() const { return frontVector; }
	Vector2 getLerpFrontVec() const;

	AABB getAABB() { return boundingBox; };
};