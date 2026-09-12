#pragma once
#include<memory>
#include<vector>
#include<functional>

class StageStatus;
class IMapObject;
class Timer;
class EnemyBase;

class IStage
{
public:
	virtual ~IStage() = default;

	virtual void init()														= 0;
	virtual void update()													= 0;
	virtual void draw()														= 0;
	virtual void drawUI()													= 0;
	virtual void addClearConditions(const std::function<bool()>& condition) = 0;
	virtual void addOverConditions(const std::function<bool()>& condition)	= 0;
	virtual void destoryObject()											= 0;

	virtual const std::shared_ptr<StageStatus> getStatus()const noexcept					= 0;
	virtual const std::vector<std::shared_ptr<IMapObject>> getMapObjects()const noexcept	= 0;
	virtual const std::shared_ptr<Timer> getTimer()const noexcept							= 0;
	virtual const std::vector<std::shared_ptr<EnemyBase>> getEnemies()const noexcept		= 0;
};