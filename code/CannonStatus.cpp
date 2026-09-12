#include"DxLibForIreven.h"
#include"WeaponStatus.h"
#include"CannonStatus.h"

int CannonStatus::cannonModel_ = 0;

CannonStatus::CannonStatus()
{
	statusData_				= LoadDataJson("statusData/weaponData/cannonStatus.json");
	std::string modelPath	= statusData_["model_path"].get<std::string>();

	//最初の一回だけスタティックモデルを読み込み
	if (cannonModel_ == 0)
	{
		cannonModel_ = MV1LoadModel(modelPath.c_str());	//dxlibの関数はstringを使えないのでc_str()で変換する
	}

	//スタティックモデルを再利用して読みこみ
	modelHandle_ = MV1DuplicateModel(cannonModel_);

	muzzleBoneNumber_ = MV1SearchFrame(modelHandle_, "muzzle");

	init();
}

CannonStatus::~CannonStatus()
{
}

void CannonStatus::init()
{
	startFindingTarget();
	position_			= VGet(0.0f, 0.0f, 0.0f);
	muzzlePosition_		= MV1GetFramePosition(modelHandle_, muzzleBoneNumber_);
	fireRate_			= statusData_["fire_rate"].get<float>(); 
	fireBulletSpeed_	= statusData_["init_speed"].get<float>();
	attackPower_		= statusData_["attack_power"].get<float>();
	attackRange_		= statusData_["attack_range"].get<float>();
}

void CannonStatus::startFindingTarget()
{
	//初期のターゲットはカメラの方向を注視点にする
	isFindingTarget_			= false;
	currentTargetingPosition_	= GetCameraTarget();

	auto tempOffsetVector	= VNorm(GetCameraFrontVector());
	tempOffsetVector		= VScale(tempOffsetVector, init_fire_target_offset);

	currentTargetingPosition_ = VAdd(currentTargetingPosition_, tempOffsetVector);
}

void CannonStatus::setTargetingPosition(const VECTOR targetPosition, const float screenAttackRangeRadius, const float adustingAttackRange)
{
	//スクリーンの一定内の座標にいないなら早期リターン
	const VECTOR	screenTargetPosition			= ConvWorldPosToScreenPos(targetPosition);
	const float		screenCenterToTargetDistance	= CalculateDistance<float>(init_screen_center_position, screenTargetPosition);
	if (screenCenterToTargetDistance > screenAttackRangeRadius) return;

	//射程範内にいないなら早期リターン
	const float	distanceToTarget = CalculateDistance<float>(targetPosition, muzzlePosition_);
	if (distanceToTarget > attackRange_ + adustingAttackRange) return;

	//ターゲット候補がまだ誰もいないのであれば
	if (!isFindingTarget_)
	{
		currentTargetingPosition_	= targetPosition;
		isFindingTarget_			= true;

		return;
	}
	
	//どちらのターゲットが近いかを判定してから正しいターゲットを決定
	const float	currentDistanceToTarget = CalculateDistance<float>(currentTargetingPosition_, muzzlePosition_);
	if (currentDistanceToTarget < distanceToTarget) return;

	currentTargetingPosition_ = targetPosition;
}

void CannonStatus::setMuzzlePosition()
{
	muzzlePosition_ = MV1GetFramePosition(modelHandle_, muzzleBoneNumber_);
}

void CannonStatus::reinforceFireBulletSpeed()
{
	fireBulletSpeed_ += statusData_["init_speed"].get<float>() * statusData_["reinforce_multiplier"].get<float>();
}

void CannonStatus::reinforceFireRate()
{
	fireRate_ *= statusData_["reinforve_fire_bullet_multiplier"].get<float>();
}

void CannonStatus::reinforceAttackPower()
{
	attackPower_ += statusData_["attack_power"].get<float>() * statusData_["reinforce_multiplier"].get<float>();
}

void CannonStatus::reinforceAttackRange()
{
	attackRange_ += statusData_["attack_range"].get<float>() * statusData_["reinforce_multiplier"].get<float>();
}

void CannonStatus::reinforceAllStatus()
{
	reinforceAttackPower();
	reinforceAttackRange();
	reinforceFireBulletSpeed();
	reinforceFireRate();
}