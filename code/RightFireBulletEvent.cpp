#include"DxLib.h"
#include"RightFireBulletEvent.h"

RightFireBulletEvent::RightFireBulletEvent()
{
}

RightFireBulletEvent::~RightFireBulletEvent()
{
	startingEvents_.clear();
	finishingEvents_.clear();
	playingEvents_.clear();
}


void RightFireBulletEvent::addStartingEvent(const std::function<void()>& event)
{
	startingEvents_.emplace_back(event);
}

void RightFireBulletEvent::addFinishingEvent(const std::function<void()>& event)
{
	finishingEvents_.emplace_back(event);
}

void RightFireBulletEvent::addPlayingEvent(const std::function<void()>& event)
{
	playingEvents_.emplace_back(event);
}

void RightFireBulletEvent::occurStartingEvent()
{
	for (const auto& event : startingEvents_)
	{
		event();
	}
}

void RightFireBulletEvent::occurFinishingEvent()
{
	for (const auto& event : finishingEvents_)
	{
		event();
	}
}

void RightFireBulletEvent::occurPlayingEvent()
{
	for (const auto& event : playingEvents_)
	{
		event();
	}
}

void RightFireBulletEvent::clearEvent()
{
	startingEvents_.clear();
	playingEvents_.clear();
	finishingEvents_.clear();
}