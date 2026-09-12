#include"DxLibForIreven.h"
#include"EnemyStatus.h"
#include"IBullet.h"
#include"Tank.h"
#include"TankStatus.h"
#include"ISphereCollider.h"
#include"NormalBullet.h"
#include"BulletStatus.h"

Tank::Tank(const VECTOR initPosition, const VECTOR towerPosition, const std::vector<VECTOR>& destination)
{
	status_			= std::make_shared<TankStatus>(initPosition, towerPosition, destination);
	moveState_		= std::make_shared<MoveState>();
	attackState_	= std::make_shared<AttackState>();
	bullets_.emplace_back(std::make_shared<NormalBullet>(status_->getStatusData()["attack_power"].get<float>(), BulletOwnerType::ENEMY));

	//初期のステートは移動
	currentState_ = moveState_;
}

Tank::~Tank()
{
	status_			= nullptr;
	moveState_		= nullptr;
	attackState_	= nullptr;
	currentState_	= nullptr;
	bullets_.clear();
}

void Tank::init()
{

}

void Tank::update()
{
	const auto enemyStatus = getEnemyStatus();

	currentState_->update(shared_from_this());

	//座標の情報をセット
	enemyStatus->setMuzzlePosition();
	enemyStatus->setAimingPosition();
	MV1SetPosition(status_->getModelHandle(), status_->getPosition());
}

void Tank::draw()const
{
	const auto spherCollider	= std::dynamic_pointer_cast<ISphereCollider>(status_);
	const auto enemyStatus		= getEnemyStatus();

	MV1DrawModel(status_->getModelHandle());

	//DrawSphere3D(status_->getPosition(), spherCollider->getRadius(), 32, GetColor(255, 255, 255), GetColor(255, 255, 255), false);	//test

	for (const auto& destination : getEnemyStatus()->getMovingDestnationPosition())
	{
		//DrawSphere3D(destination, 10.0f, 8, GetColor(255, 255, 255), GetColor(255, 255, 255), false);	//test
	}

	//動いてない時しか表示しない
	if (currentState_ == moveState_) return;
	
	bool isExistingBullet = false;
	for (const auto& bullet : bullets_)
	{
		//弾がすでに使われてない（存在してない）なら処理しない
		if (!bullet->getStatus()->getIsUsing()) continue;

		isExistingBullet = true;
		break;
	}

	//弾が存在してるときは色を変える
	if (isExistingBullet)
	{
		DrawLine3D(enemyStatus->getMuzzlePosition(), enemyStatus->getAimingPosition(), GetColor(255, 225, 25));
	}
	else
	{
		DrawLine3D(enemyStatus->getMuzzlePosition(), enemyStatus->getAimingPosition(), GetColor(255, 25, 25));
	}
}

const std::shared_ptr<EnemyStatus> Tank::getEnemyStatus()const noexcept
{
	return std::dynamic_pointer_cast<EnemyStatus>(status_);
}

void Tank::fire()
{
	const auto enemyStatus = getEnemyStatus();

	//射線が被ってない時しか発射しない
	if (enemyStatus->getIsRayObstructed()) return;
	 
	for (const auto& bullet : bullets_)
	{
		const std::shared_ptr<BulletStatus> bulletStatus = bullet->getStatus();

		//弾がすでに使われてたら処理しない
		if (bulletStatus->getIsUsing()) return;

		//弾を発射する
		const float initBulletSpeed = status_->getStatusData()["init_bullet_speed"].get<float>();
		bulletStatus->use(enemyStatus->getMuzzlePosition(), initBulletSpeed, enemyStatus->getMuzzleDirection(VGet(0.0f, adjust_fire_angle, 0.0f)));	//発射角度に補正をかける
	}
}

void Tank::toMoveState()
{
	//同じステートには移行しない
	if (currentState_ == moveState_) return;

	currentState_->exit(shared_from_this());
	currentState_ = moveState_;
	currentState_->enter(shared_from_this());
}

void Tank::toAttackState()
{
	//同じステートには移行しない
	if (currentState_ == attackState_) return;

	//射線が通ってなければ攻撃しない
	if (!getEnemyStatus()->getHasClearShotToTarget()) return;

	currentState_->exit(shared_from_this());
	currentState_ = attackState_;
	currentState_->enter(shared_from_this());
}