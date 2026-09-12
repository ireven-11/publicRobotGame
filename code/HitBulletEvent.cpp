#include"DxLib.h"
#include"HitBulletEvent.h"

HitBulletEvent::HitBulletEvent()
{
}

HitBulletEvent::~HitBulletEvent()
{
	startingEvents_.clear();
	finishingEvents_.clear();
	playingEvents_.clear();
}


void HitBulletEvent::addStartingEvent(const std::function<void()>& event)
{
	startingEvents_.emplace_back(event);
}

void HitBulletEvent::addFinishingEvent(const std::function<void()>& event)
{
	finishingEvents_.emplace_back(event);
}

void HitBulletEvent::addPlayingEvent(const std::function<void()>& event)
{
	playingEvents_.emplace_back(event);
}

void HitBulletEvent::occurStartingEvent()
{
	for (const auto& event : startingEvents_)
	{
		event();
	}
}

void HitBulletEvent::occurFinishingEvent()
{
	for (const auto& event : finishingEvents_)
	{
		event();
	}
}

void HitBulletEvent::occurPlayingEvent()
{
	for (const auto& event : playingEvents_)
	{
		event();
	}
}

void HitBulletEvent::clearEvent()
{
	startingEvents_.clear();
	playingEvents_.clear();
	finishingEvents_.clear();
}