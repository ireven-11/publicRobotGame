#include"DxLib.h"
#include"RightAttackEvent.h"
#include"InputHandler.h"
#include"DInput.h"

RightAttackEvent::RightAttackEvent()
{
	rightAttackingTrigger_ = []() {return InputHandler::instance().getDInput()->getButtonHoldNow(PROCON::BUTTON::ZR, 0.0f); };
	startFiringGunTrigger_ = [this]() {return InputHandler::instance().getDInput()->getButtonHoldNow(PROCON::BUTTON::ZR, one_seconds_dxlib * 0.25f); };
}

RightAttackEvent::~RightAttackEvent()
{
}

void RightAttackEvent::addStartingEvent(const std::function<void()>& event)
{
	startingEvents_.emplace_back(event);
}

void RightAttackEvent::addFinishingEvent(const std::function<void()>& event)
{
	finishingEvents_.emplace_back(event);
}

void RightAttackEvent::addPlayingEvent(const std::function<void()>& event)
{
	playingEvents_.emplace_back(event);
}

void RightAttackEvent::occurStartingEvent()
{
	if (!InputHandler::instance().getDInput()->getButtonPressedMoment(PROCON::BUTTON::ZR)) return;

	for (const auto& event : startingEvents_)
	{
		event();
	}
}

void RightAttackEvent::occurFinishingEvent()
{
	if (!InputHandler::instance().getDInput()->getButtonReleasedMoment(PROCON::BUTTON::ZR)) return;

	for (const auto& event : finishingEvents_)
	{
		event();
	}
}

void RightAttackEvent::occurPlayingEvent()
{
	if (!InputHandler::instance().getDInput()->getButtonHoldNow(PROCON::BUTTON::ZR, 0.0f)) return;

	for (const auto& event : playingEvents_)
	{
		event();
	}
}

void RightAttackEvent::clearEvent()
{
	startingEvents_.clear();
	playingEvents_.clear();
	finishingEvents_.clear();
}