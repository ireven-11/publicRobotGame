#pragma once
#include<memory>
#include<unordered_map>
#include<functional>

class Player;

enum class ReinforcingType
{
	RIGHT_FIRE_RATE,
	RIGHT_FIRE_SPEED,
	RIGHT_ATTACK_POWER,
	RIGHT_ATTACK_RANGE,
	LEFT_FIRE_RATE,
	LEFT_FIRE_SPEED,
	LEFT_ATTACK_POWER,
	LEFT_ATTACK_RANGE,
	RIGHT_CHANGE_TO_BOMBBUG,
	LEFT_CHANGE_TO_BOMBBUG,
	RIGHT_CHANGE_TO_CANNON,
	LEFT_CHANGE_TO_CANNON,
	COUNT	//enumclassの要素数を数える用。必ず最後に置く
};

class PlayerStatus;
class BattleSoundManager;

class ReinforcementSystem
{
public:
	ReinforcementSystem(const std::shared_ptr<PlayerStatus>& playerStatus);
	~ReinforcementSystem();

	void init();
	void update(const std::shared_ptr<BattleSoundManager>& sound);
	void start();

	const bool getIsActive()const noexcept { return isActive_; }
	const std::vector<ReinforcingType> getChoicedReinforcement()const noexcept { return choicedReinforcement_; };
	const int getSelectorIndex()const noexcept { return selectorIndex_; }

private:
	void choiceReinforcingOptions();
	void selectReinforcement(const std::shared_ptr<BattleSoundManager>& sound);
	void decideReinforcement(const std::shared_ptr<BattleSoundManager>& sound);

	bool isActive_;
	int selectorIndex_;
	std::unordered_map<ReinforcingType, std::function<void()>> reinforce_;
	std::vector<std::function<void()>> reinforcingOptions_;
	std::vector<ReinforcingType> choicedReinforcement_;

	const int max_options_number = 3;
};