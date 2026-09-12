#pragma once
#include"IEffect.h"
#include<unordered_map>
#include<memory>
#include<string>

class PlayerStatus;
class EnemyBase;

class EffectManager
{
public:
	EffectManager();
	~EffectManager();

	void init();
	void update();
	void draw();
	void setEffectPosition(const std::shared_ptr<PlayerStatus>& playerStatus, const std::vector<std::shared_ptr<EnemyBase>>& enemies);
	void setEffectRotationAngle(const std::shared_ptr<PlayerStatus>& playerStatus);
	void stopEffects();

	std::unordered_map<std::string, std::shared_ptr<IEffect>> getEffects()const noexcept { return effects_; }

private:
	void eventRegister();

	std::unordered_map<std::string,std::shared_ptr<IEffect>> effects_;

	const int max_effekseer_particles = 8000;//エフェクシアの最大パーティクル数
};