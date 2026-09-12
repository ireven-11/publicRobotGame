#include"DxLib.h"
#include"StageClearEvent.h"
#include"StageOverEvent.h"
#include"FinishedClearMessageEvent.h"
#include"FinishedOverMessageEvent.h"
#include"StageEventHandler.h"

StageEventHandler::StageEventHandler()
	:stageClearEvent_(std::make_shared<StageClearEvent>()),
	stageOverEvent_(std::make_shared<StageOverEvent>()),
	finishedClearMessageEvent_(std::make_shared<FinishedClearMessageEvent>()),
	finishedOverMessageEvent_(std::make_shared<FinishedOverMessageEvent>())
{
}

StageEventHandler::~StageEventHandler()
{
	
}