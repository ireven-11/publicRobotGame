#pragma once
#include"CharaBase.h"
#include"EnemyStatus.h"
#include"MoveState.h"
#include"AttackState.h"

class IBullet;

class EnemyBase : public CharaBase
{
public:
	virtual ~EnemyBase() = default;

	virtual void init()				= 0;
	virtual void update()			= 0;
	virtual void draw()const		= 0;
	virtual void fire()				= 0;
	virtual void toMoveState()		= 0;
	virtual void toAttackState()	= 0;

	virtual const std::shared_ptr<EnemyStatus> getEnemyStatus()const noexcept = 0;

	const std::vector<std::shared_ptr<IBullet>> getBullets()const noexcept { return bullets_; }
	
protected:
	std::vector<std::shared_ptr<IBullet>> bullets_;
	std::shared_ptr<IState> currentState_;
	std::shared_ptr<MoveState> moveState_;
	std::shared_ptr<AttackState> attackState_;
};