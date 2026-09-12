#pragma once
#include<functional>

class IEvent
{
public:
	virtual ~IEvent() = default;

	virtual void addStartingEvent(const std::function<void()>& event)	= 0;
	virtual void addFinishingEvent(const std::function<void()>& event)	= 0;
	virtual void addPlayingEvent(const std::function<void()>& event)	= 0;
	virtual void occurStartingEvent()									= 0;
	virtual void occurFinishingEvent()									= 0;
	virtual void occurPlayingEvent()									= 0;
	virtual void clearEvent()											= 0;
};