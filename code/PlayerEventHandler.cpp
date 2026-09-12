#include"LevelUpEvent.h"
#include"OnDamageEvent.h"
#include"PlayerEventHandler.h"

PlayerEventHandler::PlayerEventHandler()
	:levelUpEvent_(std::make_shared<LevelUpEvent>()),
	onDamageEvent_(std::make_shared<OnDamageEvent>())
{
}

PlayerEventHandler::~PlayerEventHandler()
{
	levelUpEvent_	= nullptr;
	onDamageEvent_	= nullptr;
}