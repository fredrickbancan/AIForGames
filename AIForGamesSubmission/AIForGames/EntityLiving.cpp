#include "EntityLiving.h"
#include "Game.h"
#include <math.h>
EntityLiving::EntityLiving(float x, float y, float rotation) : pos({ x,y }), prevPos({ x,y }), rotation(rotation), prevRotation(rotation)
{
	boundingBox.translate(pos);
	moveAcel = .3F;
}

EntityLiving::EntityLiving() : pos({ 0,0 }), rotation(0), prevPos({0,0}), prevRotation(0)
{
	boundingBox.translate(pos);
	moveAcel = .3F;
}

void EntityLiving::onTick()
{
	prevPos = pos;

	if (rotation < 0)
	{
		rotation = 360 + rotation;
	}

	rotation = fmodf(rotation, 360.0F);
	prevRotation = rotation;
	prevFrontvector = frontVector;
	alignFrontVector();
	vel.x += acel.x;
	vel.y += acel.y;
	vel.x *= 1 - moveResistance;
	vel.y *= 1 - moveResistance;
	Game::get()->tryToMoveEntity(this, vel);
	acel.x = 0;
	acel.y = 0;
}
void EntityLiving::alignFrontVector()
{
	frontVector.x = (float)(cosf(radians(rotation - 90)));
	frontVector.y = (float)(sinf(radians(rotation - 90)));
	frontVector = Vector2Normalize(frontVector);
}
float EntityLiving::radians(float degrees)
{
	return degrees * (3.1415926535897F / 180.0F);
}
void EntityLiving::walkFowards()
{
	acel = Vector2Add(acel, Vector2MultiplyV(frontVector, {moveAcel, moveAcel}));
}
float EntityLiving::getLerpPosX() const
{
	return Game::get()->lerp(prevPos.x, pos.x);
}

float EntityLiving::getLerpPosY() const
{
	return Game::get()->lerp(prevPos.y, pos.y);
}

Vector2 EntityLiving::getLerpPos() const
{
	return Vector2{ Game::get()->lerp(prevPos.x, pos.x), Game::get()->lerp(prevPos.y, pos.y) };
}

float EntityLiving::getLerpRotation() const
{
	return Game::get()->lerp(prevRotation, rotation);
}

Vector2 EntityLiving::getLerpFrontVec() const
{
	return Vector2Normalize( Vector2{ Game::get()->lerp(prevFrontvector.x, frontVector.x), Game::get()->lerp(prevFrontvector.y, frontVector.y) });
}
