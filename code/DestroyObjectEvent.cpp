#include"DxLib.h"
#include<vector>
#include"DestroyObjectEvent.h"

DestroyObjectEvent::DestroyObjectEvent()
{
}

DestroyObjectEvent::~DestroyObjectEvent()
{
	startingEvents_.clear();
	finishingEvents_.clear();
	playingEvents_.clear();
}

/// <summary>
/// オブジェクトが壊れたタイミングでイベントを起こす
/// 壊れたとこで呼ぶ処理
/// </summary>
/// <param name="event"></param>
void DestroyObjectEvent::addStartingEvent(const std::function<void()>& event)
{
	startingEvents_.emplace_back(event);
}

void DestroyObjectEvent::addFinishingEvent(const std::function<void()>& event)
{
	finishingEvents_.emplace_back(event);
}

void DestroyObjectEvent::addPlayingEvent(const std::function<void()>& event)
{
	playingEvents_.emplace_back(event);
}

void DestroyObjectEvent::occurStartingEvent()
{
	for (const auto& event : startingEvents_)
	{
		event();
	}
}

void DestroyObjectEvent::occurFinishingEvent()
{
	for (const auto& event : finishingEvents_)
	{
		event();
	}
}

void DestroyObjectEvent::occurPlayingEvent()
{
	for (const auto& event : playingEvents_)
	{
		event();
	}
}

void DestroyObjectEvent::clearEvent()
{
	startingEvents_.clear();
	playingEvents_.clear();
	finishingEvents_.clear();
}