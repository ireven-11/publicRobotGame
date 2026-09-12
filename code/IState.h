#pragma once
#include<memory>

class EnemyBase;

class IState
{
public:
	virtual ~IState() = default;

	virtual void enter(const std::shared_ptr<EnemyBase>& enemy)	= 0;
	virtual void exit(const std::shared_ptr<EnemyBase>& enemy)	= 0;
	virtual void update(const std::shared_ptr<EnemyBase>& enemy)	= 0;
	virtual void draw(const std::shared_ptr<EnemyBase>& enemy)	= 0;
};