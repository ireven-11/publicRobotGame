#pragma once
#include"IState.h"

class EnemyBase;
class Timer;

class AttackState : public IState
{
public:
	AttackState();
	~AttackState();

	void enter(const std::shared_ptr<EnemyBase>& enemy);
	void exit(const std::shared_ptr<EnemyBase>& enemy);
	void update(const std::shared_ptr<EnemyBase>& enemy);
	void draw(const std::shared_ptr<EnemyBase>& enemy);

private:
	std::shared_ptr<Timer> firingTimer_;

	const short fire_interval = 3;
};