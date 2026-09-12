#include"DxLib.h"
#include"LeftAttackEvent.h"
#include"InputHandler.h"
#include"DInput.h"

LeftAttackEvent::LeftAttackEvent()
{
	leftAttackingTrigger_	= []() {return InputHandler::instance().getDInput()->getButtonHoldNow(PROCON::BUTTON::ZL, 0.0f); };
	startFiringGunTrigger_	= [this]() {return InputHandler::instance().getDInput()->getButtonHoldNow(PROCON::BUTTON::ZL, one_seconds_dxlib * 0.25f); };
}

LeftAttackEvent::~LeftAttackEvent()
{
}

void LeftAttackEvent::addStartingEvent(const std::function<void()>& event)
{
	startingEvents_.emplace_back(event);
}

void LeftAttackEvent::addFinishingEvent(const std::function<void()>& event)
{
	finishingEvents_.emplace_back(event);
}

void LeftAttackEvent::addPlayingEvent(const std::function<void()>& event)
{
	playingEvents_.emplace_back(event);
}

void LeftAttackEvent::occurStartingEvent()
{
	if (!InputHandler::instance().getDInput()->getButtonPressedMoment(PROCON::BUTTON::ZL)) return;

	for (const auto& event : startingEvents_)
	{
		event();
	}
}

void LeftAttackEvent::occurFinishingEvent()
{
	if (!InputHandler::instance().getDInput()->getButtonReleasedMoment(PROCON::BUTTON::ZL)) return;

	for (const auto& event : finishingEvents_)
	{
		event();
	}
}

void LeftAttackEvent::occurPlayingEvent()
{
	if (!InputHandler::instance().getDInput()->getButtonHoldNow(PROCON::BUTTON::ZL, 0.0f)) return;

	for (const auto& event : playingEvents_)
	{
		event();
	}
}

void LeftAttackEvent::clearEvent()
{
	startingEvents_.clear();
	playingEvents_.clear();
	finishingEvents_.clear();
}