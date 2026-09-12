#pragma once
#include"IState.h"

class EnemyBase;

class MoveState : public IState
{
public:
	MoveState();
	~MoveState();

	void enter(const std::shared_ptr<EnemyBase>& enemy);
	void exit(const std::shared_ptr<EnemyBase>& enemy);
	void update(const std::shared_ptr<EnemyBase>& enemy);
	void draw(const std::shared_ptr<EnemyBase>& enemy);

private:

};