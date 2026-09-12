#include"DxLib.h"
#include"FinishedOverMessageEvent.h"

FinishedOverMessageEvent::FinishedOverMessageEvent()
{
}

FinishedOverMessageEvent::~FinishedOverMessageEvent()
{
	clearEvent();
}

void FinishedOverMessageEvent::addStartingEvent(const std::function<void()>& event)
{
	startingEvents_.emplace_back(event);
}

void FinishedOverMessageEvent::addFinishingEvent(const std::function<void()>& event)
{
	finishingEvents_.emplace_back(event);
}

void FinishedOverMessageEvent::addPlayingEvent(const std::function<void()>& event)
{
	playingEvents_.emplace_back(event);
}

void FinishedOverMessageEvent::occurStartingEvent()
{
	for (const auto& event : startingEvents_)
	{
		event();
	}
}

void FinishedOverMessageEvent::occurFinishingEvent()
{
	for (const auto& event : finishingEvents_)
	{
		event();
	}
}

void FinishedOverMessageEvent::occurPlayingEvent()
{
	for (const auto& event : playingEvents_)
	{
		event();
	}
}

void FinishedOverMessageEvent::clearEvent()
{
	startingEvents_.clear();
	playingEvents_.clear();
	finishingEvents_.clear();
}