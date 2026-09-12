#pragma once
#include"EnemyBase.h"

class DroneStatus;

class Drone : public EnemyBase, public std::enable_shared_from_this<Drone>
{
public:
	Drone(const VECTOR initPosition, const VECTOR towerPosition, const std::vector<VECTOR>& destination);
	~Drone();

	void init();
	void update();
	void draw()const;
	void fire();
	void toMoveState();
	void toAttackState();

	const std::shared_ptr<EnemyStatus> getEnemyStatus()const noexcept;

private:

	const float adjust_fire_angle = DX_PI_F * 0.05f;
};