#include"DxLib.h"
#include"RaiseEvent.h"
#include"InputHandler.h"
#include"DInput.h"

RaiseEvent::RaiseEvent()
{
	raiseTrigger_		= []() {return InputHandler::instance().getDInput()->getButtonHoldNow(PROCON::BUTTON::B, 0.0f); };
	startRaiseTrigger_	= []() {return InputHandler::instance().getDInput()->getButtonPressedMoment(PROCON::BUTTON::B); };
	finishRaiseTrigger_ = []() {return InputHandler::instance().getDInput()->getButtonReleasedMoment(PROCON::BUTTON::B); };
}

RaiseEvent::~RaiseEvent()
{
	raiseTrigger_		= nullptr;
	startRaiseTrigger_	= nullptr;
	finishRaiseTrigger_ = nullptr;
}

void RaiseEvent::addStartingEvent(const std::function<void()>& event)
{
	startingEvents_.emplace_back(event);
}

void RaiseEvent::addFinishingEvent(const std::function<void()>& event)
{
	finishingEvents_.emplace_back(event);
}

void RaiseEvent::addPlayingEvent(const std::function<void()>& event)
{
	playingEvents_.emplace_back(event);
}

void RaiseEvent::occurStartingEvent()
{
	if (!startRaiseTrigger_()) return;

	for (const auto& event : startingEvents_)
	{
		event();
	}
}

void RaiseEvent::occurFinishingEvent()
{
	if (!finishRaiseTrigger_()) return;

	for (const auto& event : finishingEvents_)
	{
		event();
	}
}

void RaiseEvent::occurPlayingEvent()
{
	if (!raiseTrigger_()) return;

	for (const auto& event : playingEvents_)
	{
		event();
	}
}

void RaiseEvent::clearEvent()
{
	startingEvents_.clear();
	playingEvents_.clear();
	finishingEvents_.clear();
}