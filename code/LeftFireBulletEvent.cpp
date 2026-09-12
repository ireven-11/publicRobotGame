#include"DxLib.h"
#include"LeftFireBulletEvent.h"

LeftFireBulletEvent::LeftFireBulletEvent()
{
}

LeftFireBulletEvent::~LeftFireBulletEvent()
{
	startingEvents_.clear();
	finishingEvents_.clear();
	playingEvents_.clear();
}


void LeftFireBulletEvent::addStartingEvent(const std::function<void()>& event)
{
	startingEvents_.emplace_back(event);
}

void LeftFireBulletEvent::addFinishingEvent(const std::function<void()>& event)
{
	finishingEvents_.emplace_back(event);
}

void LeftFireBulletEvent::addPlayingEvent(const std::function<void()>& event)
{
	playingEvents_.emplace_back(event);
}

void LeftFireBulletEvent::occurStartingEvent()
{
	for (const auto& event : startingEvents_)
	{
		event();
	}
}

void LeftFireBulletEvent::occurFinishingEvent()
{
	for (const auto& event : finishingEvents_)
	{
		event();
	}
}

void LeftFireBulletEvent::occurPlayingEvent()
{
	for (const auto& event : playingEvents_)
	{
		event();
	}
}

void LeftFireBulletEvent::clearEvent()
{
	startingEvents_.clear();
	playingEvents_.clear();
	finishingEvents_.clear();
}