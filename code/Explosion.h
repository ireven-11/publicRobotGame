#pragma once

class Explosion
{
public:
	Explosion(const float explosionRadius, const float attackPower);
	~Explosion();

	void update();
	void start(const VECTOR explosionPosition);

	const bool		getIsActive()const noexcept { return isActive_; }
	const VECTOR	getPosition()const noexcept { return position_; }
	const float		getRadius()const noexcept { return radius_; }
	const float		getAttackPower()const noexcept { return attackPower_; }

private:
	bool	isActive_;
	VECTOR	position_;
	float	radius_;
	float	attackPower_;
};