#include"DxLibForIreven.h"
#include"EnemyStatus.h"
#include"DroneStatus.h"
#include<algorithm>
#include<cmath>
#include"ColliderEventHander.h"
#include"DestroyObjectEvent.h"

//スタティックメンバーを初期化
int DroneStatus::droneModel_ = 0;

DroneStatus::DroneStatus(const VECTOR initPosition, const VECTOR towerPosition, const std::vector<VECTOR>& destinations)
	: EnemyStatus(VGet(initPosition.x, initPosition.y, initPosition.z)),
	tower_position(towerPosition)
{
	statusData_					= LoadDataJson("statusData/enemyData/droneStatus.json");
	const std::string modelPath	= statusData_["model_path"].get<std::string>();

	//最初の一回だけ読み込み
	if (droneModel_ == 0)
	{
		droneModel_ = MV1LoadModel(modelPath.c_str());	//dxlibの関数はstringを使えないのでc_str()で変換する
	}

	//スタティックのモデルを再利用して読み込み
	modelHandle_ = MV1DuplicateModel(droneModel_);

	const float modelScale = statusData_["model_scale"].get<float>();
	MV1SetScale(modelHandle_, VGet(modelScale, modelScale, modelScale));

	colliderRadius_ = statusData_["collision_radius"].get<float>() * modelScale;

	//目的地をセット
	for (const auto& destination : destinations)
	{
		movingDestinationPosition_.emplace_back(destination);
	}
	movingDestinationPosition_.emplace_back(tower_position);

	init();
}

DroneStatus::~DroneStatus()
{
	MV1DeleteModel(modelHandle_);
}

void DroneStatus::init()
{
	position_					= VGet(init_position.x, init_position.y + statusData_["adjust_spawn_position_y"].get<float>(), init_position.z);
	canDestroy_					= false;
	hp_							= statusData_["max_hp"].get<float>();
	aimingPosition_				= VGet(0.0f, 0.0f, 0.0f);
	isRayObstructed_			= false;
	muzzlePosition_				= VGet(0.0f, 0.0f, 0.0f);
	moveDirection_				= VGet(0.0f, 0.0f, 0.0f);
	speed_						= statusData_["move_speed"].get<float>();
	hasClearShotToTarget_		= false;
	colliderTopPosition_		= VGet(0.0f, 0.0f, 0.0f);
	colliderBottomPosition_		= VGet(0.0f, 0.0f, 0.0f);
	modelRotationTimeY_			= 0.0f;
	isTargetInAttackRange_		= false;
	modelAngleX_				= 0.0f;
	movingDestinationIndex_		= 0;
	onDamage_					= false;
	dropingExp_					= statusData_["drop_exp"].get<float>();

	movingDestinationPosition_.emplace_back(tower_position);
}

void DroneStatus::pushBack(const VECTOR pushBackVector)
{
	position_ = VAdd(position_, pushBackVector);

	//地面の下に行かないようにする
	position_.y = (std::max)(position_.y, 0.0f);
}

void DroneStatus::decreaseHp(const float damage)
{
	hp_ -= damage;
}

void DroneStatus::destroy()
{
	//hpが0になったら死ぬ
	if (hp_ > 0) return;

	ColliderEventHander::instance().getDestroyObjectEvent()->occurStartingEvent();
	canDestroy_ = true;
}

void DroneStatus::moveToFacingDirection()
{
	//モデルの向いてる方向に移動
	const VECTOR standardDirection	= VGet(0.0f, 0.0f, 1.0f);
	moveDirection_					= VTransform(standardDirection, MGetRotY(modelAngleY_));
	moveDirection_					= VNorm(moveDirection_);
	position_						= VAdd(position_, VScale(moveDirection_, speed_));
}

void DroneStatus::findTarget(const std::shared_ptr<ITargetableObject>& target)
{
	//既にターゲットを見つけているなら処理しない
	if (isTargetInAttackRange_) return;

	//遮蔽が被ってるならターゲットを見つけられない
	if (isRayObstructed_) return;

	const VECTOR targetPosition = target->getTargetedPosition();
	
	//射程範囲外ならターゲットを見つけられない
	isTargetInAttackRange_ = CalculateDistance<float>(muzzlePosition_, targetPosition) < statusData_["attack_range_radius"].get<float>();
	if (!isTargetInAttackRange_) return;

	//敵の方向を向く用の座標をセット
	facingPosition = targetPosition;
}

void DroneStatus::resetTarget()
{
	isTargetInAttackRange_ = false;
}

void DroneStatus::rotationToDestinationDirection()
{
	const auto movingDestinationPosition = movingDestinationPosition_[movingDestinationIndex_];
	modelAngleY_ = atan2(movingDestinationPosition.x - position_.x, movingDestinationPosition.z - position_.z);
	MV1SetRotationXYZ(modelHandle_, VGet(0.0f, modelAngleY_ + DX_PI_F * 0.5f, 0.0f));	//dxpifで補正をかけてる
}

void DroneStatus::setAimingPosition()
{
	const float		attackLineDistance		= statusData_["attack_range_radius"].get<float>();
	const VECTOR	maxAttackRangePosition	= VAdd(muzzlePosition_, VScale(getMuzzleDirection(), attackLineDistance));

	aimingPosition_ = maxAttackRangePosition;
}

void DroneStatus::setColliderPosition()
{
	colliderTopPosition_	= MV1GetFramePosition(modelHandle_, MV1SearchFrame(modelHandle_, "front"));
	colliderBottomPosition_ = MV1GetFramePosition(modelHandle_, MV1SearchFrame(modelHandle_, "back"));
}

void DroneStatus::setIsRayObstructed(const bool isOverlap)
{
	isRayObstructed_ = isOverlap;
}

void DroneStatus::setMuzzlePosition()
{
	const int muzzleFrameIndex = MV1SearchFrame(modelHandle_, "muzzle");
	muzzlePosition_ = MV1GetFramePosition(modelHandle_, muzzleFrameIndex);
}

void DroneStatus::shiftToSide()
{
	//todo:遮蔽をよけて移動する処理を書く（以下はtest）

	//射線が被ってたら横に移動する
	if (!isRayObstructed_) return;

	//横に向けて移動
	const VECTOR standardDirection	= VGet(0.0f, 0.0f, 1.0f);	//z軸方向を基準とする
	moveDirection_					= VTransform(standardDirection, MGetRotY(modelAngleY_ + DX_PI_F * 0.5f));
	moveDirection_					= VNorm(moveDirection_);
	position_						= VAdd(position_, VScale(moveDirection_, speed_));

	return;
}

void DroneStatus::faceToTarget()
{
	const float startAngleY	= modelAngleY_;
	const float endAngleY	= atan2(facingPosition.x - position_.x, facingPosition.z - position_.z);

	//線形補完で滑らかにモデルを回転させる
	modelAngleY_		= std::lerp(startAngleY, endAngleY, modelRotationTimeY_);
	modelRotationTimeY_	+= turn_around_speed;
	if (modelRotationTimeY_ >= 1.0f)
	{
		//lerpなので時間が1を超えたら元に戻す
		modelRotationTimeY_ = 0.0f;
	}

	MV1SetRotationXYZ(modelHandle_, VGet(0.0f, modelAngleY_ + DX_PI_F * 0.5f, 0.0f));	//dxpifで補正をかけてる

	//モデルのx軸回転させるとバグったので、代わりにy座標を移動させる
	if (facingPosition.y + chace_target_between_y < position_.y)
	{
		--position_.y;
	}
	else if (facingPosition.y - chace_target_between_y > position_.y)
	{
		++position_.y;
	}
}

const VECTOR DroneStatus::getMuzzleDirection(const VECTOR adjustMuzzleDirection)const noexcept
{
	const int muzzleFrameIndex		= MV1SearchFrame(modelHandle_, "muzzle");
	const MATRIX tempMatrix			= MV1GetFrameLocalWorldMatrix(modelHandle_, muzzleFrameIndex);
	const VECTOR muzzleDirection	= VNorm(VAdd(adjustMuzzleDirection, VGet(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2])));	//上方向を基準にしてる

	return muzzleDirection;
}

void DroneStatus::setHasClearShotToTarget(const bool hasClearShotToTarget)
{
	hasClearShotToTarget_ = hasClearShotToTarget;
}

void DroneStatus::moveNextDestination()
{
	//目的地にだとりついてなければ目的地を更新しない（誤差を許容するために距離で判定）
	if (CalculateDistance<float>(position_, movingDestinationPosition_[movingDestinationIndex_]) > distance_error) return;

	//インデックスを進めて次の目的地をセットする
	if (movingDestinationIndex_ >= movingDestinationPosition_.size() - 1) return;
	++movingDestinationIndex_;
}

void DroneStatus::setOnDamage(const bool isHiting)
{
	onDamage_ = isHiting;
}