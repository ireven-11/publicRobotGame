#include"DxLibForIreven.h"
#include<algorithm>
#include"InputHandler.h"
#include"PostProcessing.h"
#include"CameraShake.h"
#include"NormalCamera.h"
#include"CameraEvent.h"
#include"DashEvent.h"
#include"WalkEvent.h"
#include"PlayerEventHandler.h"
#include"OnDamageEvent.h"

NormalCamera::NormalCamera(const std::function<void(VECTOR, VECTOR)>& playerFunction)
	: giveCameraPosition_(playerFunction),
	cameraShake_(std::make_shared<CameraShake>())
{
	currentPostEffect_ = dashPostEffect_();

	init();

	eventRegister();
}

NormalCamera::~NormalCamera()
{
	currentPostEffect_	= nullptr;
	giveCameraPosition_ = nullptr;
	cameraShake_		= nullptr;
}

void NormalCamera::eventRegister()
{
	//カメラが置いて行かれる処理をスタートさせる
	InputHandler::instance().getDashEvent()->addStartingEvent([this]() {currentPostEffect_ = dashPostEffect_(); });
	InputHandler::instance().getDashEvent()->addStartingEvent([this]() {leavingTime_ = 0.0f; });
	InputHandler::instance().getDashEvent()->addStartingEvent([this]() {isLeavingBehind_ = true; });
	InputHandler::instance().getDashEvent()->addStartingEvent([this]() {dashPostEffect_()->start(PostEffectType::GRAPH, add_leaving_time); });

	//ダメージのポストエフェクト
	PlayerEventHandler::instance().getOnDamageEvent()->addStartingEvent([this]() {currentPostEffect_ = damagePostEffect_(); });
	PlayerEventHandler::instance().getOnDamageEvent()->addStartingEvent([this]() {damagePostEffect_()->start(PostEffectType::GRAPH, add_leaving_time); });
	PlayerEventHandler::instance().getOnDamageEvent()->addStartingEvent([this]() {cameraShake_->start(shake_speed, shake_widht, shake_height, max_shake_count); });
}

void NormalCamera::init()
{
	position_					= position_offset;
	targetPosition_				= VGet(0.0f, 0.0f, 0.0f);
	verticalAngle_				= 0.0f;
	isLeavingBehind_			= false;
	leavingTime_				= max_leaving_time;
	currentOffset_				= VGet(0.0f, 0.0f, 0.0f);
	currentTargetOffset_		= VGet(0.0f, 0.0f, 0.0f);
	currentPostEffect_->init();
	cameraShake_->init();
}

void NormalCamera::update(const VECTOR playerPosition, const float cameraAngle)
{
	//前の座標を記録しておく
	const VECTOR prevPosition = position_;

	//オフセットを更新
	if (isLeavingBehind_)
	{
		//カメラが置いて行かれる時の処理をする
		leftBehind();
	}
	
	//オフセットを更新
	currentOffset_			= position_offset;
	currentTargetOffset_	= target_position_offset;

	//オフセットを回転させる
	rotation(playerPosition, cameraAngle);
		
	//オフセットから注視点を更新
	targetPosition_ = VAdd(playerPosition, currentTargetOffset_);

	//オフセットとプレイヤーポジションから最終的に結果になる座標を計算する
	const VECTOR desiredPosition = VAdd(playerPosition, currentOffset_);

	//らーぷで前の座標と今の座標を補完する
	position_ = LerpVector(prevPosition, desiredPosition, leavingTime_);

	//カメラシェイク
	cameraShake_->update(targetPosition_);

	//カメラの注視点を設定
	SetCameraPositionAndTarget_UpVecY(position_, targetPosition_);

	//カメラの座標と注視点を渡してプレイヤーの向いてる方向を計算する
	giveCameraPosition_(targetPosition_, position_);

	//ポストエフェクトをする
	currentPostEffect_->update();
}

void NormalCamera::rotation(const VECTOR playerPosition, const float cameraAngle)
{
	const int stickValue = InputHandler::instance().getCameraEvent()->getStickYTrigger()();

	//入力による角度変更
	if (stickValue < 0)
	{
		verticalAngle_ += rotation_speed;
	}
	if (stickValue > 0)
	{
		verticalAngle_ -= rotation_speed;
	}
	verticalAngle_ = (std::min)(verticalAngle_, max_verticalAngle);
	verticalAngle_ = (std::max)(verticalAngle_, min_verticalAngle);

	const float angleY = cameraAngle + DX_PI_F;

	//回転行列を合成
	const MATRIX rotationY	= MGetRotY(angleY);
	const MATRIX rotationX	= MGetRotX(-verticalAngle_);
	const MATRIX rotation	= MMult(rotationX, rotationY); //順番が重要（ローカル視点のX→Y回転）

	//オフセットを回転させる
	currentOffset_			= VTransform(currentOffset_, rotation);
	currentTargetOffset_	= VTransform(currentTargetOffset_, rotation);
}

/// <summary>
/// カメラがおいて行かれる
/// </summary>
void NormalCamera::leftBehind()
{
	//ラープを進める
	leavingTime_ += add_leaving_time;
	if (leavingTime_ >= max_leaving_time)
	{
		leavingTime_		= max_leaving_time;
		isLeavingBehind_	= false;
	}
}