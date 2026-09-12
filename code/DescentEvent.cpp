#include"DxLib.h"
#include"DescentEvent.h"
#include"InputHandler.h"
#include"DInput.h"

DescentEvent::DescentEvent()
{
	descentTrigger_			= []() {return InputHandler::instance().getDInput()->getButtonHoldNow(PROCON::BUTTON::LEFTSTICK_PRESS, 0.0f); };
	startDescentTrigger_	= []() {return InputHandler::instance().getDInput()->getButtonPressedMoment(PROCON::BUTTON::LEFTSTICK_PRESS); };
	finishDescentTrigger_	= []() {return InputHandler::instance().getDInput()->getButtonReleasedMoment(PROCON::BUTTON::LEFTSTICK_PRESS); };
}

DescentEvent::~DescentEvent()
{
	descentTrigger_			= nullptr;
	startDescentTrigger_	= nullptr;
	finishDescentTrigger_	= nullptr;
}

void DescentEvent::addStartingEvent(const std::function<void()>& event)
{
	startingEvents_.emplace_back(event);
}

void DescentEvent::addFinishingEvent(const std::function<void()>& event)
{
	finishingEvents_.emplace_back(event);
}

void DescentEvent::addPlayingEvent(const std::function<void()>& event)
{
	playingEvents_.emplace_back(event);
}

void DescentEvent::occurStartingEvent()
{
	if (!startDescentTrigger_()) return;

	for (const auto& event : startingEvents_)
	{
		event();
	}
}

void DescentEvent::occurFinishingEvent()
{
	if (!finishDescentTrigger_()) return;

	for (const auto& event : finishingEvents_)
	{
		event();
	}
}

void DescentEvent::occurPlayingEvent()
{
	if (!descentTrigger_()) return;

	for (const auto& event : playingEvents_)
	{
		event();
	}
}

void DescentEvent::clearEvent()
{
	startingEvents_.clear();
	playingEvents_.clear();
	finishingEvents_.clear();
}