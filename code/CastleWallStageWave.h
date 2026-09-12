#pragma once
#include<functional>
#include<unordered_map>
#include<memory>

class Timer;

class CastleWallStageWave
{
public:
	CastleWallStageWave(const nlohmann::json& data);
	~CastleWallStageWave();

	void init();
	void update(const nlohmann::json& data, std::vector<std::shared_ptr<EnemyBase>>& enemies);

private:
	void spawn(const nlohmann::json& data, std::vector<std::shared_ptr<EnemyBase>>& enemies);
	const std::shared_ptr<EnemyBase> createEnemy(const std::string& enemyName, const VECTOR sapwnPosition, const std::vector<VECTOR>& movingDestinations);

	std::shared_ptr<Timer> timer_;
	std::unordered_map<std::string, std::function<std::shared_ptr<EnemyBase>(const VECTOR, const std::vector<VECTOR>)>> enemyFactory_;

	const int wave_max_time;
};