#include"DxLib.h"
#include"CameraEvent.h"
#include"InputHandler.h"
#include"DInput.h"

CameraEvent::CameraEvent()
{
	stickXTrigger_ = []() {return InputHandler::instance().getDInput()->getProconStickXY(PROCON::STICK::RIGHT_X); };
	stickYTrigger_ = []() {return InputHandler::instance().getDInput()->getProconStickXY(PROCON::STICK::RIGHT_Y); };
}

CameraEvent::~CameraEvent()
{
	stickXTrigger_	= nullptr;
	stickYTrigger_ = nullptr;
}

void CameraEvent::addStartingEvent(const std::function<void()>& event)
{
	startingEvents_.emplace_back(event);
}

void CameraEvent::addFinishingEvent(const std::function<void()>& event)
{
	finishingEvents_.emplace_back(event);
}

void CameraEvent::addPlayingEvent(const std::function<void()>& event)
{
	playingEvents_.emplace_back(event);
}

void CameraEvent::occurStartingEvent()
{
	//if (!InputHandler::instance().getDInput()->getProconIsTiltingStick(false)) return;

	for (const auto& event : startingEvents_)
	{
		event();
	}
}

void CameraEvent::occurFinishingEvent()
{
	//if (InputHandler::instance().getDInput()->getProconIsTiltingStick(false)) return;

	for (const auto& event : finishingEvents_)
	{
		event();
	}
}

void CameraEvent::occurPlayingEvent()
{
	//if (!InputHandler::instance().getDInput()->getButtonHoldNow(PROCON::BUTTON::ZL, 0.0f)) return;

	for (const auto& event : playingEvents_)
	{
		event();
	}
}

void CameraEvent::clearEvent()
{
	startingEvents_.clear();
	playingEvents_.clear();
	finishingEvents_.clear();
}