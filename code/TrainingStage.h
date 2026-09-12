#pragma once

class IStage;
class IMapObject;
class Score;
class Timer;

class TrainingStage : public IStage
{
public:
	TrainingStage();
	~TrainingStage();

	void init();
	void update();
	void draw();
	void spawner();
	void destoryObject();
	void drawUI();

	const std::shared_ptr<StageStatus> getStatus()const noexcept { return status_; }
	const std::vector<std::shared_ptr<IMapObject>> getMapObjects()const noexcept { return mapObjects_; }
	const std::shared_ptr<Timer> getTimer()const noexcept { return timer_; }
	const std::vector<std::shared_ptr<EnemyBase>> getEnemies()const noexcept { return std::vector<std::shared_ptr<EnemyBase>>(); }

private:
	void checkOver();
	void checkClear();

	std::shared_ptr<StageStatus> status_;
	std::vector<std::shared_ptr<IMapObject>> mapObjects_;
	std::shared_ptr<Score> score_;
	std::shared_ptr<Timer> timer_;
	int spawnCounter_;

	const short			max_object_value	= 10;
	const float			ground_grid_size	= 1000.0f;
	const unsigned int	ground_grid_div		= 100;
};