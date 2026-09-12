#include"DxLib.h"
#include"EnemyBase.h"
#include"EffectManager.h"
#include"EffekseerForDXLib.h"
#include"MuzzleFlashEffect.h"
#include"SmokeEffect.h"
#include"PlayerStatus.h"
#include"IWeapon.h"
#include"WeaponStatus.h"
#include"BulletEventHandler.h"
#include"RightFireBulletEvent.h"
#include"LeftFireBulletEvent.h"
#include"InputHandler.h"
#include"WalkEvent.h"
#include"EngineEffect.h"
#include"ExplosionEffect.h"
#include"ColliderEventHander.h"
#include"DestroyObjectEvent.h"
#include"EnemyStatus.h"
#include"HitEffect.h"
#include"HitBulletEvent.h"
#include"OnDamageEvent.h"
#include"StageEventHandler.h"
#include"StageOverEvent.h"

EffectManager::EffectManager()
{
	//DirectX9を使用するようにする。(DirectX11も可)
	//Effekseerを使用するには必ず設定する。
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	//Effekseerを初期化する。
	//引数には画面に表示する最大パーティクル数を設定する。
	if (Effekseer_Init(max_effekseer_particles) == -1) return;

	//フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
	//Effekseerを使用する場合は必ず設定する。
	SetChangeScreenModeGraphicsSystemResetFlag(false);

	//DXライブラリのデバイスロストした時のコールバックを設定する。
	//ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する。
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	//Zバッファを有効にする。
	//Effekseerを使用する場合、2DゲームでもZバッファを使用する。
	SetUseZBuffer3D(true);

	//Zバッファへの書き込みを有効にする。
	//Effekseerを使用する場合、2DゲームでもZバッファを使用する。
	SetWriteZBuffer3D(true);

	//エフェクトを追加
	effects_["rightWeaponMuzzleFlash"]	= std::make_shared<MuzzleFlashEffect>();
	effects_["leftWeaponMuzzleFlash"]	= std::make_shared<MuzzleFlashEffect>();
	effects_["smoke"]					= std::make_shared<SmokeEffect>();
	effects_["rightEngine"]				= std::make_shared<EngineEffect>();
	effects_["leftEngine"]				= std::make_shared<EngineEffect>();
	effects_["explosion"]				= std::make_shared<ExplosionEffect>();
	effects_["hit"]						= std::make_shared<HitEffect>();

	eventRegister();
}

EffectManager::~EffectManager()
{
	GetEffekseer3DManager()->StopAllEffects();
	effects_.clear();

    //Effekseerを終了する。
    Effkseer_End();
}

void EffectManager::eventRegister()
{
	BulletEventHandler::instance().getRightFireBulletEvent()->addStartingEvent([this] {effects_["rightWeaponMuzzleFlash"]->play(); });
	BulletEventHandler::instance().getLeftFireBulletEvent()->addStartingEvent([this] {effects_["leftWeaponMuzzleFlash"]->play(); });
	InputHandler::instance().getWalkEvent()->addStartingEvent([this] {effects_["smoke"]->play(); });
	InputHandler::instance().getWalkEvent()->addFinishingEvent([this] {effects_["smoke"]->stop(); });
	InputHandler::instance().getWalkEvent()->addStartingEvent([this] {effects_["rightEngine"]->play(); });
	InputHandler::instance().getWalkEvent()->addFinishingEvent([this] {effects_["rightEngine"]->stop(); });
	InputHandler::instance().getWalkEvent()->addStartingEvent([this] {effects_["leftEngine"]->play(); });
	InputHandler::instance().getWalkEvent()->addFinishingEvent([this] {effects_["leftEngine"]->stop(); });
	ColliderEventHander::instance().getDestroyObjectEvent()->addStartingEvent([this] {effects_["explosion"]->play(); });
	ColliderEventHander::instance().getOnDamageEvent()->addStartingEvent([this] {effects_["hit"]->play(); });
}

void EffectManager::init()
{
	for (auto iteretor = effects_.begin(); iteretor != effects_.end(); ++iteretor)
	{
		iteretor->second->init();
	}
}

void EffectManager::update()
{
	for (auto iteretor = effects_.begin(); iteretor != effects_.end(); ++iteretor)
	{
		iteretor->second->update();
	}

	UpdateEffekseer3D();
}

void EffectManager::draw()
{
	DrawEffekseer3D_Begin();
	for (auto iteretor = effects_.begin(); iteretor != effects_.end(); ++iteretor)
	{
		iteretor->second->draw();
	}
	DrawEffekseer3D_End();
}

void EffectManager::setEffectPosition(const std::shared_ptr<PlayerStatus>& playerStatus, const std::vector<std::shared_ptr<EnemyBase>>& enemies)
{
	effects_["rightWeaponMuzzleFlash"]->setPosition(playerStatus->getRightWeapon()->getWeaponStatus()->getMuzzlePosition());
	effects_["leftWeaponMuzzleFlash"]->setPosition(playerStatus->getLeftWeapon()->getWeaponStatus()->getMuzzlePosition());
	effects_["smoke"]->setPosition(playerStatus->getPosition());
	effects_["rightEngine"]->setPosition(playerStatus->getRightBoosterPosition());
	effects_["leftEngine"]->setPosition(playerStatus->getLeftBoosterPosition());
	for (const auto& enemy : enemies)
	{
		if (!enemy->getEnemyStatus()->getCanDestroy()) continue;

		effects_["explosion"]->setPosition(enemy->getStatus()->getPosition());
	}
	for (const auto& enemy : enemies)
	{
		if (!enemy->getEnemyStatus()->getOnDamage()) continue;

		effects_["hit"]->setPosition(enemy->getStatus()->getPosition());
	}
}

void EffectManager::setEffectRotationAngle(const std::shared_ptr<PlayerStatus>& playerStatus)
{
	effects_["rightEngine"]->setRotationAngle(playerStatus->getRightBoosterRotationAngle());
	effects_["leftEngine"]->setRotationAngle(playerStatus->getLeftBoosterRotationAngle());
}

void EffectManager::stopEffects()
{
	for (auto iteretor = effects_.begin(); iteretor != effects_.end(); ++iteretor)
	{
		iteretor->second->stop();
	}
}