#include"DxLib.h"
#include"RightFireBulletEvent.h"
#include"LeftFireBulletEvent.h"
#include"BulletEventHandler.h"

BulletEventHandler::BulletEventHandler()
	:rightFireBulletEvent_(std::make_shared<RightFireBulletEvent>()),
	leftFireBulletEvent_(std::make_shared<LeftFireBulletEvent>())
{
}

BulletEventHandler::~BulletEventHandler()
{
	rightFireBulletEvent_	= nullptr;
	leftFireBulletEvent_	= nullptr;
}