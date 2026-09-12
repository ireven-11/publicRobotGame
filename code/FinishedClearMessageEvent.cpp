#include"DxLib.h"
#include"FinishedClearMessageEvent.h"

FinishedClearMessageEvent::FinishedClearMessageEvent()
{
}

FinishedClearMessageEvent::~FinishedClearMessageEvent()
{
	clearEvent();
}

void FinishedClearMessageEvent::addStartingEvent(const std::function<void()>& event)
{
	startingEvents_.emplace_back(event);
}

void FinishedClearMessageEvent::addFinishingEvent(const std::function<void()>& event)
{
	finishingEvents_.emplace_back(event);
}

void FinishedClearMessageEvent::addPlayingEvent(const std::function<void()>& event)
{
	playingEvents_.emplace_back(event);
}

void FinishedClearMessageEvent::occurStartingEvent()
{
	for (const auto& event : startingEvents_)
	{
		event();
	}
}

void FinishedClearMessageEvent::occurFinishingEvent()
{
	for (const auto& event : finishingEvents_)
	{
		event();
	}
}

void FinishedClearMessageEvent::occurPlayingEvent()
{
	for (const auto& event : playingEvents_)
	{
		event();
	}
}

void FinishedClearMessageEvent::clearEvent()
{
	startingEvents_.clear();
	playingEvents_.clear();
	finishingEvents_.clear();
}