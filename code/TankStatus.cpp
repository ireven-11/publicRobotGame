#include"DxLibForIreven.h"
#include"EnemyStatus.h"
#include"TankStatus.h"
#include<algorithm>
#include<cmath>
#include"ColliderEventHander.h"
#include"DestroyObjectEvent.h"

//スタティックメンバーを初期化
int TankStatus::tankModel_ = 0;

TankStatus::TankStatus(const VECTOR initPosition, const VECTOR towerPosition, const std::vector<VECTOR>& destinations)
	: EnemyStatus(initPosition),
	tower_position(towerPosition)
{
	statusData_				= LoadDataJson("statusData/enemyData/tankStatus.json");
	std::string modelPath	= statusData_["model_path"].get<std::string>();

	//最初の一回だけ読み込み
	if (tankModel_ == 0)
	{
		tankModel_ = MV1LoadModel(modelPath.c_str());	//dxlibの関数はstringを使えないのでc_str()で変換する
	}

	//スタティックのモデルを再利用して読み込み
	modelHandle_ = MV1DuplicateModel(tankModel_);

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

TankStatus::~TankStatus()
{
	MV1DeleteModel(modelHandle_);
}

void TankStatus::init()
{
	position_					= init_position;
	canDestroy_					= false;
	hp_							= statusData_["max_hp"].get<float>();
	aimingPosition_				= VGet(0.0f, 0.0f, 0.0f);
	isRayObstructed_			= false;
	muzzlePosition_				= VGet(0.0f, 0.0f, 0.0f);
	moveDirection_				= VGet(0.0f, 0.0f, 0.0f);
	speed_						= statusData_["move_speed"].get<float>();
	hasClearShotToTarget_		= false;
	movingDestinationIndex_		= 0;
	onDamage_					= false;
	dropingExp_					= statusData_["drop_exp"].get<float>();
}

void TankStatus::pushBack(const VECTOR pushBackVector)
{
	position_ = VAdd(position_, pushBackVector);

	position_.y = 0.0f;
}

void TankStatus::decreaseHp(const float damage)
{
	hp_ -= damage;
}

void TankStatus::destroy()
{
	//hpが0になったら死ぬ
	if (hp_ > 0) return;

	ColliderEventHander::instance().getDestroyObjectEvent()->occurStartingEvent();
	canDestroy_ = true;
}

void TankStatus::moveToFacingDirection()
{
	//モデルの向いてる方向に移動
	const VECTOR standardDirection	= VGet(0.0f, 0.0f, 1.0f);
	moveDirection_					= VTransform(standardDirection, MGetRotY(modelAngleY_));
	moveDirection_					= VNorm(moveDirection_);
	position_						= VAdd(position_, VScale(moveDirection_, speed_));
}

void TankStatus::findTarget(const std::shared_ptr<ITargetableObject>& target)
{
	//既にターゲットを見つけているなら処理しない
	if (isTargetInAttackRange_) return;

	//遮蔽が被ってるならターゲットを見つけられない
	if (isRayObstructed_) return;

	VECTOR targetPosition	= target->getTargetedPosition();
	targetPosition.y		= muzzlePosition_.y;	//マズルの高さにしか攻撃できないのでターゲットのyをまずるの高さにセット

	//射程範囲外ならターゲットを見つけられない
	isTargetInAttackRange_ = CalculateDistance<float>(muzzlePosition_, targetPosition) < statusData_["attack_range_radius"].get<float>();
	if (!isTargetInAttackRange_) return;

	//敵の方向を向く用の座標をセット
	facingPosition = targetPosition;
}

void TankStatus::resetTarget()
{
	isTargetInAttackRange_ = false;
}

void TankStatus::rotationToDestinationDirection()
{
	const auto movingDestinationPosition	= movingDestinationPosition_[movingDestinationIndex_];
	modelAngleY_							= atan2(movingDestinationPosition.x - position_.x, movingDestinationPosition.z - position_.z);
	MV1SetRotationXYZ(modelHandle_, VGet(0.0f, modelAngleY_ + DX_PI_F, 0.0f));	//dxpifで補正をかけてる
}

void TankStatus::setAimingPosition()
{
	const float attackLineDistance		= statusData_["attack_range_radius"].get<float>();
	const VECTOR maxAttackRangePosition = VAdd(muzzlePosition_, VScale(getMuzzleDirection(), attackLineDistance));

	aimingPosition_ = maxAttackRangePosition;
}

void TankStatus::setColliderPosition()
{
}

void TankStatus::setIsRayObstructed(const bool isOverlap)
{
	isRayObstructed_ = isOverlap;
}

void TankStatus::setMuzzlePosition()
{
	const int muzzleFrameIndex	= MV1SearchFrame(modelHandle_, "muzzle");
	muzzlePosition_				= MV1GetFramePosition(modelHandle_, muzzleFrameIndex);
}

void TankStatus::shiftToSide()
{
	//todo:遮蔽をよけて移動する処理を書く（以下はtest）

	//射線が被ってたら横に移動する
	if (!isRayObstructed_ || hasClearShotToTarget_) return;
	
	//横に向けて移動
	const VECTOR standardDirection	= VGet(0.0f, 0.0f, 1.0f);	//z軸方向を基準とする
	moveDirection_					= VTransform(standardDirection, MGetRotY(modelAngleY_ + DX_PI_F * 0.5f));
	moveDirection_					= VNorm(moveDirection_);
	position_						= VAdd(position_, VScale(moveDirection_, speed_));
}

void TankStatus::faceToTarget()
{
	modelAngleY_ = atan2(facingPosition.x - position_.x, facingPosition.z - position_.z);
	MV1SetRotationXYZ(modelHandle_, VGet(0.0f, modelAngleY_ + DX_PI_F, 0.0f));	//dxpifで補正をかけてる
}

const VECTOR TankStatus::getMuzzleDirection(const VECTOR adjustMuzzleDirection)const noexcept
{
	const int muzzleFrameIndex		= MV1SearchFrame(modelHandle_, "muzzle");
	const MATRIX tempMatrix			= MV1GetFrameLocalWorldMatrix(modelHandle_, muzzleFrameIndex);
	const VECTOR muzzleDirection	= VNorm(VAdd(adjustMuzzleDirection, VGet(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2])));	//上方向を基準にしてる

	return muzzleDirection;
}

void TankStatus::setHasClearShotToTarget(const bool hasClearShotToTarget)
{
	hasClearShotToTarget_ = hasClearShotToTarget;
}

void TankStatus::moveNextDestination()
{
	//目的地にだとりついてなければ目的地を更新しない（誤差を許容するために距離で判定）
	if (CalculateDistance<float>(position_, movingDestinationPosition_[movingDestinationIndex_]) > distance_error) return;

	//インデックスを進めて次の目的地をセットする
	if (movingDestinationIndex_ >= movingDestinationPosition_.size() - 1) return;
	++movingDestinationIndex_;
}

void TankStatus::setOnDamage(const bool isHiting)
{
	onDamage_ = isHiting;
}