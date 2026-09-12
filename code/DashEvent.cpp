#include"DxLib.h"
#include"DashEvent.h"
#include"InputHandler.h"
#include"DInput.h"

DashEvent::DashEvent()
{
	dashTrigger_		= []() {return InputHandler::instance().getDInput()->getButtonHoldNow(PROCON::BUTTON::R, 0.0f); };
	startDashTrigger_	= []() {return InputHandler::instance().getDInput()->getButtonPressedMoment(PROCON::BUTTON::R); };
	finishDashTrigger_	= []() {return InputHandler::instance().getDInput()->getButtonReleasedMoment(PROCON::BUTTON::R); };
}

DashEvent::~DashEvent()
{
	dashTrigger_		= nullptr;
	startDashTrigger_	= nullptr;
	finishDashTrigger_	= nullptr;
}

void DashEvent::addStartingEvent(const std::function<void()>& event)
{
	startingEvents_.emplace_back(event);
}

void DashEvent::addFinishingEvent(const std::function<void()>& event)
{
	finishingEvents_.emplace_back(event);
}

void DashEvent::addPlayingEvent(const std::function<void()>& event)
{
	playingEvents_.emplace_back(event);
}

void DashEvent::occurStartingEvent()
{
	if (!startDashTrigger_()) return;

	for (const auto& event : startingEvents_)
	{
		event();
	}
}

void DashEvent::occurFinishingEvent()
{
	if (!finishDashTrigger_()) return;

	for (const auto& event : finishingEvents_)
	{
		event();
	}
}

void DashEvent::occurPlayingEvent()
{
	if (!dashTrigger_()) return;

	for (const auto& event : playingEvents_)
	{
		event();
	}
}

void DashEvent::clearEvent()
{
	startingEvents_.clear();
	playingEvents_.clear();
	finishingEvents_.clear();
}