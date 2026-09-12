#include"DxLib.h"
#include"TowerEvent.h"

TowerEvent::TowerEvent()
{
}

TowerEvent::~TowerEvent()
{
	hpThreeQuartersEvent_.clear();
	hpThreeQuartersEvent_.clear();
	hpOneHalfEvent_.clear();
	hpOneQuartersEvent_.clear();
}

void TowerEvent::addHpThreeQuartersEvent(const std::function<void()>& event)
{
	hpThreeQuartersEvent_.emplace_back(event);
}

void TowerEvent::addHpOneHalfEvent(const std::function<void()>& event)
{
	hpOneHalfEvent_.emplace_back(event);
}

void TowerEvent::addHpOneQuartersEvent(const std::function<void()>& event)
{
	hpOneQuartersEvent_.emplace_back(event);
}

void TowerEvent::occurHpThreeQuartersEvent()
{
	for (const auto& event : hpThreeQuartersEvent_)
	{
		event();
	}
}

void TowerEvent::occurHpOneHalfEvent()
{
	for (const auto& event : hpOneHalfEvent_)
	{
		event();
	}
}

void TowerEvent::occurHpOneQuartersEvent()
{
	for (const auto& event : hpOneQuartersEvent_)
	{
		event();
	}
}