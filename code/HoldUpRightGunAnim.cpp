#include"DxLibForIreven.h"
#include"playerStatus.h"
#include<memory>
#include"IAnimation.h"
#include"HoldUpRightGunAnim.h"
#include"InputHandler.h"
#include"RightAttackEvent.h"
#include"IWeapon.h"
#include"WeaponStatus.h"

HoldUpRightGunAnim::HoldUpRightGunAnim()
{
	init();
	
	InputHandler::instance().getRightAttackEvent()->addStartingEvent([this]() { enter(); });
	InputHandler::instance().getRightAttackEvent()->addFinishingEvent([this]() { exit(); });
}

HoldUpRightGunAnim::~HoldUpRightGunAnim()
{
}

void HoldUpRightGunAnim::init()
{
	prevRaiseRotation_	= 0.0f;
	prevDownRotation_	= 0.0f;

	reset();
}

void HoldUpRightGunAnim::enter()
{
	reset();
	onUpdate_ = true;
}

void HoldUpRightGunAnim::exit()
{
	reset();
	canRelease_ = true;
}

void HoldUpRightGunAnim::update(const std::shared_ptr<PlayerStatus>& status)
{
	downArm(status);

	if (!onUpdate_) return;

	//銃を中央を向かせる
	if (InputHandler::instance().getRightAttackEvent()->getStartFiringGunTrigger()())
	{
		faceArmToTargetPosition(status);
	}

	//ボーンの回転値を計算して武器を構える
	double zBoneRotation_ = 0.0;
	if (dropGunTime_ < max_lerp_time)
	{
		dropGunTime_		+= drop_gun_speed;	//毎フレーム少しずつ加算
		zBoneRotation_		= std::lerp(prevDownRotation_, ready_gun_max_rota_z, dropGunTime_);
		prevRaiseRotation_	= static_cast<float>(zBoneRotation_);

		//モデルの回転させたいボーンに回転値をセット
		MATRIX Mrotate1		= MV1GetFrameRotateMatrix(status->getModelHandle(), status->getModelBoneIndex()["Rb_LowerArm_R"], 0, 0, zBoneRotation_);
		MV1SetFrameUserLocalMatrix(status->getModelHandle(), status->getModelBoneIndex()["Rb_LowerArm_R"], Mrotate1);
	}
}

void HoldUpRightGunAnim::downArm(const std::shared_ptr<PlayerStatus>& status)
{
	//腕を下げる
	if (canRelease_)
	{
		//終了アニメーション（銃を下す）をする
		double zRota = 0.0;
		if (dropGunTime_ < max_lerp_time)
		{
			dropGunTime_	+= drop_gun_speed;	//毎フレーム少しずつ加算
			zRota			= std::lerp(prevRaiseRotation_, init_rotation, dropGunTime_);
		}
		else
		{
			//終わったら抜けて解放
			reset();
			return;
		}

		MATRIX Mrotate1		= MV1GetFrameRotateMatrix(status->getModelHandle(), status->getModelBoneIndex()["Rb_LowerArm_R"], 0, 0, zRota);
		prevDownRotation_	= static_cast<float>(zRota);
		MV1SetFrameUserLocalMatrix(status->getModelHandle(), status->getModelBoneIndex()["Rb_LowerArm_R"], Mrotate1);
	}
}

void HoldUpRightGunAnim::faceArmToTargetPosition(const std::shared_ptr<PlayerStatus>& status)
{
	const VECTOR axis_up	= VGet(1.0f, 0.0f, 0.0f);
	const VECTOR axis_aim	= VGet(0.0f, 1.0f, 0.0f);

	const VECTOR targetPosition = status->getRightWeapon()->getWeaponStatus()->getCurrentTargetingPosition();

	OneBoneIK(status->getModelHandle(), status->getModelBoneIndex()["Rb_LowerArm_R"], targetPosition,
		axis_up, axis_aim, VGet(DX_PI_F * 0.25f, DX_PI_F * 0.25f, DX_PI_F * 0.25f));
}

void HoldUpRightGunAnim::reset()
{
	onUpdate_		= false;
	dropGunTime_	= 0.0f;
	canRelease_		= false;
}