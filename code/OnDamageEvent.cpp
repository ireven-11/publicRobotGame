#include"DxLib.h"
#include"OnDamageEvent.h"

OnDamageEvent::OnDamageEvent()
{
}

OnDamageEvent::~OnDamageEvent()
{
	startingEvents_.clear();
	finishingEvents_.clear();
	playingEvents_.clear();
}

void OnDamageEvent::addStartingEvent(const std::function<void()>& event)
{
	startingEvents_.emplace_back(event);
}

void OnDamageEvent::addFinishingEvent(const std::function<void()>& event)
{
	finishingEvents_.emplace_back(event);
}

void OnDamageEvent::addPlayingEvent(const std::function<void()>& event)
{
	playingEvents_.emplace_back(event);
}

void OnDamageEvent::occurStartingEvent()
{
	for (const auto& event : startingEvents_)
	{
		event();
	}
}

void OnDamageEvent::occurFinishingEvent()
{
	for (const auto& event : finishingEvents_)
	{
		event();
	}
}

void OnDamageEvent::occurPlayingEvent()
{
	for (const auto& event : playingEvents_)
	{
		event();
	}
}

void OnDamageEvent::clearEvent()
{
	startingEvents_.clear();
	playingEvents_.clear();
	finishingEvents_.clear();
}