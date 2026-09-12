#include"DxLib.h"
#include"WalkEvent.h"
#include"InputHandler.h"
#include"DInput.h"

WalkEvent::WalkEvent()
{
	stickAngleTrigger_		= []() {return InputHandler::instance().getDInput()->getProconStickAngle(PROCON::STICK::LEFT_ANGLE); };
	stickXTrigger_			= []() {return InputHandler::instance().getDInput()->getProconStickXY(PROCON::STICK::LEFT_X); };
	stickYTrigger_			= []() {return InputHandler::instance().getDInput()->getProconStickXY(PROCON::STICK::LEFT_Y); };
	startWalkingTrigger_	= []() {return InputHandler::instance().getDInput()->getStickTiledMoment(PROCON::STICK::LEFT_X) ||
		InputHandler::instance().getDInput()->getStickTiledMoment(PROCON::STICK::LEFT_Y); };
	walkingTrigger_			= []() {return InputHandler::instance().getDInput()->getStickHoldNow(PROCON::STICK::LEFT_X, 0.0f) ||
		InputHandler::instance().getDInput()->getStickHoldNow(PROCON::STICK::LEFT_Y, 0.0f); };
	finishWalkingTrigger_	= []() {return InputHandler::instance().getDInput()->getStickReleasedMoment(PROCON::STICK::LEFT_X) &&
		InputHandler::instance().getDInput()->getProconStickXY(PROCON::STICK::LEFT_Y) == 0 ||
		InputHandler::instance().getDInput()->getProconStickXY(PROCON::STICK::LEFT_X) == 0 && 
		InputHandler::instance().getDInput()->getStickReleasedMoment(PROCON::STICK::LEFT_Y); };
}

WalkEvent::~WalkEvent()
{
	stickAngleTrigger_		= nullptr;
	stickXTrigger_			= nullptr;
	stickYTrigger_			= nullptr;
	startWalkingTrigger_	= nullptr;
	walkingTrigger_			= nullptr;
	finishWalkingTrigger_	= nullptr;
}

void WalkEvent::addStartingEvent(const std::function<void()>& event)
{
	startingEvents_.emplace_back(event);
}

void WalkEvent::addFinishingEvent(const std::function<void()>& event)
{
	finishingEvents_.emplace_back(event);
}

void WalkEvent::addPlayingEvent(const std::function<void()>& event)
{
	playingEvents_.emplace_back(event);
}

void WalkEvent::occurStartingEvent()
{
	if (!startWalkingTrigger_()) return;

	for (const auto& event : startingEvents_)
	{
		event();
	}
}

void WalkEvent::occurFinishingEvent()
{
	if (!finishWalkingTrigger_()) return;

	for (const auto& event : finishingEvents_)
	{
		event();
	}
}

void WalkEvent::occurPlayingEvent()
{
	if (!walkingTrigger_()) return;

	for (const auto& event : playingEvents_)
	{
		event();
	}
}

void WalkEvent::clearEvent()
{
	startingEvents_.clear();
	playingEvents_.clear();
	finishingEvents_.clear();
}