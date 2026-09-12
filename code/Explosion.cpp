#include"DxLib.h"
#include"Explosion.h"

Explosion::Explosion(const float explosionRadius, const float attackPower)
{
	radius_			= explosionRadius;
	attackPower_	= attackPower;
}

Explosion::~Explosion()
{
}

void Explosion::update()
{
	//1ƒtƒŒ[ƒ€‚Å”š”j‚ªI‚í‚é‚æ‚¤‚É‚·‚é
	isActive_ = false;
}

void Explosion::start(const VECTOR explosionPosition)
{
	position_ = explosionPosition;
	isActive_ = true;
}