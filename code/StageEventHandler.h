#pragma once
#include"Singleton.h"

class StageClearEvent;
class StageOverEvent;
class FinishedOverMessageEvent;
class FinishedClearMessageEvent;

class StageEventHandler : public Singleton<StageEventHandler>
{
public:
	~StageEventHandler();

	const std::shared_ptr<StageClearEvent> getStageClearEvent()const noexcept { return stageClearEvent_; }
	const std::shared_ptr<StageOverEvent> getStageOverEvent()const noexcept { return stageOverEvent_; }

private:
	friend class Singleton<StageEventHandler>;
	StageEventHandler();

	std::shared_ptr<StageClearEvent> stageClearEvent_;
	std::shared_ptr<StageOverEvent> stageOverEvent_;
	std::shared_ptr<FinishedClearMessageEvent> finishedClearMessageEvent_;
	std::shared_ptr<FinishedOverMessageEvent> finishedOverMessageEvent_;
};