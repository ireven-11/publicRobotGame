#pragma once
#include"EnemyBase.h"

class TankStatus;

class Tank : public EnemyBase, public std::enable_shared_from_this<Tank>
{
public:
	Tank(const VECTOR initPosition, const VECTOR towerPosition, const std::vector<VECTOR>& destination);
	~Tank();

	void init();
	void update();
	void draw()const;
	void fire();
	void toMoveState();
	void toAttackState();

	const std::shared_ptr<EnemyStatus> getEnemyStatus()const noexcept;

private:
	
	const float adjust_fire_angle = DX_PI_F * 0.2f;
};