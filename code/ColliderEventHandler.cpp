#include"DxLib.h"
#include"HitBulletEvent.h"
#include"DestroyObjectEvent.h"
#include"OnDamageEvent.h"
#include"ColliderEventHander.h"

ColliderEventHander::ColliderEventHander()
	:hitBulletEvent_(std::make_shared<HitBulletEvent>()),
	destroyObjectEvent_(std::make_shared<DestroyObjectEvent>()),
	onDamageEvent_(std::make_shared<OnDamageEvent>())
{
}

ColliderEventHander::~ColliderEventHander()
{
	hitBulletEvent_		 = nullptr;
	destroyObjectEvent_	 = nullptr;
	onDamageEvent_		 = nullptr;
}