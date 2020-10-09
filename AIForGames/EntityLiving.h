#pragma once

/*Base class for living entites*/
class EntityLiving
{
private:
	/*each positional value has a previous tick value for interpolation*/
	float posX, posY, prevPosX, prevPosY, rotation, prevRotation, moveAcel, velX = 0, velY = 0, acelX = 0, acelY = 0;

public:
	static constexpr float moveResistance = 0.5F;
	EntityLiving(float x, float y, float rotation);
	EntityLiving();

	/*called every tick to do stuff to entity*/
	void onTick();

	void setPos(float x, float y) { posX = x; posY = y; }

	void setRotation(float r) { rotation = r; }

	float getPosX() const { return posX; }
	float getPosY() const { return posY; }
	float getRotation() const { return rotation; }

	float getLerpPosX() const;//returns the posX of this entity linearly interpolated between ticks
	float getLerpPosY() const;//returns the posY of this entity linearly interpolated between ticks
	float getLerpRotation() const;//returns the rotation of this entity linearly interpolated between ticks
};