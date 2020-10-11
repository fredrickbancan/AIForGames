#include "EntityLiving.h"
#include "Game.h"
#include <math.h>
EntityLiving::EntityLiving(float x, float y, float rotation) : pos({ x,y }), prevPos({ x,y }), rotation(rotation), prevRotation(rotation)
{
	moveAcel = .3F;
}

EntityLiving::EntityLiving() : pos({ 0,0 }), rotation(0), prevPos({0,0}), prevRotation(0)
{
	moveAcel = .3F;
}

void EntityLiving::onTick()
{
	prevPos = pos;
	prevRotation = rotation;
	prevFrontvector = frontVector;
	//TODO: make rotation follow heading
	alignFrontVector();
	vel.x += acel.x;
	vel.y += acel.y;
	vel.x *= 1 - moveResistance;
	vel.y *= 1 - moveResistance;
	Game::get()->tryToMoveEntity(this, vel);
	acel.x = 0;
	acel.y = 0;
	boundingBox.translate(pos);
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
