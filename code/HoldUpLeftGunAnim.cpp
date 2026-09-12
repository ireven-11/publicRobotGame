#include"DxLibForIreven.h"
#include"playerStatus.h"
#include"InputHandler.h"
#include<memory>
#include"IAnimation.h"
#include"HoldUpLeftGunAnim.h"
#include"LeftAttackEvent.h"
#include"IWeapon.h"
#include"WeaponStatus.h"

HoldUpLeftGunAnim::HoldUpLeftGunAnim()
{
	init();
	
	InputHandler::instance().getLeftAttackEvent()->addStartingEvent([this]() { enter(); });
	InputHandler::instance().getLeftAttackEvent()->addFinishingEvent([this]() { exit(); });
}

HoldUpLeftGunAnim::~HoldUpLeftGunAnim()
{
}

void HoldUpLeftGunAnim::init()
{
	prevraiseRotation_	= 0.0f;
	prevDownRotation_	= 0.0f;

	reset();
}

void HoldUpLeftGunAnim::enter()
{
	reset();
	onUpdate_ = true;
}

void HoldUpLeftGunAnim::exit()
{
	reset();
	canRelease_ = true;
}

void HoldUpLeftGunAnim::update(const std::shared_ptr<PlayerStatus>& status)
{
	downArm(status);

	if (!onUpdate_) return;

	//銃を中央を向かせる
	if (InputHandler::instance().getLeftAttackEvent()->getStartFiringGunTrigger()())
	{
		faceArmToTargetPosition(status);
	}

	//ボーンの回転値を計算して腕をあげる
	double zRota = 0.0;
	if (dropGunTime_ < max_lerp_time)
	{
		dropGunTime_		+= drop_gun_speed;	//毎フレーム少しずつ加算
		zRota				= std::lerp(prevDownRotation_, ready_gun_max_rota_z, dropGunTime_);
		prevraiseRotation_	= static_cast<float>(zRota);

		//モデルの回転させたいボーンに回転値をセット
		MATRIX Mrotate1 = MV1GetFrameRotateMatrix(status->getModelHandle(), status->getModelBoneIndex()["Rb_LowerArm_L"], 0, 0, zRota);
		MV1SetFrameUserLocalMatrix(status->getModelHandle(), status->getModelBoneIndex()["Rb_LowerArm_L"], Mrotate1);
	}
}

void HoldUpLeftGunAnim::downArm(const std::shared_ptr<PlayerStatus>& status)
{
	//腕を下げる
	if (canRelease_)
	{
		//終了アニメーション（銃を下す）をする
		double zRota = 0.0;
		if (dropGunTime_ < max_lerp_time)
		{
			dropGunTime_	+= drop_gun_speed;	//毎フレーム少しずつ加算
			zRota			= std::lerp(prevraiseRotation_, init_rotation, dropGunTime_);
		}
		else
		{
			//終わったら抜けて解放
			reset();
			return;
		}

		MATRIX Mrotate1		= MV1GetFrameRotateMatrix(status->getModelHandle(), status->getModelBoneIndex()["Rb_LowerArm_L"], 0, 0, zRota);
		prevDownRotation_	= static_cast<float>(zRota);
		MV1SetFrameUserLocalMatrix(status->getModelHandle(), status->getModelBoneIndex()["Rb_LowerArm_L"], Mrotate1);
	}
}

void HoldUpLeftGunAnim::faceArmToTargetPosition(const std::shared_ptr<PlayerStatus>& status)
{
	const VECTOR axis_up	= VGet(-1.0f, 0.0f, 0.0f);
	const VECTOR axis_aim	= VGet(0.0f, 1.0f, 0.0f);

	const VECTOR targetPosition = status->getLeftWeapon()->getWeaponStatus()->getCurrentTargetingPosition();

	OneBoneIK(status->getModelHandle(), status->getModelBoneIndex()["Rb_LowerArm_L"], targetPosition,
		axis_up, axis_aim, VGet(DX_PI_F * 0.25f, DX_PI_F * 0.25f, DX_PI_F * 0.25f));
}

void HoldUpLeftGunAnim::reset()
{
	onUpdate_			= false;
	dropGunTime_		= 0.0f;
	canRelease_			= false;
}