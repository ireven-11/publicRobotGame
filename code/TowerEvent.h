#pragma once
#include<functional>
#include<vector>

class TowerEvent
{
public:
	TowerEvent();
	~TowerEvent();

	void addHpThreeQuartersEvent(const std::function<void()>& event);
	void addHpOneHalfEvent(const std::function<void()>& event);
	void addHpOneQuartersEvent(const std::function<void()>& event);
	void occurHpThreeQuartersEvent();
	void occurHpOneHalfEvent();
	void occurHpOneQuartersEvent();

	void clearEvent();

private:
	std::vector<std::function<void()>> hpThreeQuartersEvent_;
	std::vector<std::function<void()>> hpOneHalfEvent_;
	std::vector<std::function<void()>> hpOneQuartersEvent_;
};