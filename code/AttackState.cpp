#include"DxLib.h"
#include"Timer.h"
#include"EnemyBase.h"
#include"AttackState.h"
#include"EnemyStatus.h"

AttackState::AttackState()
    :firingTimer_(std::make_shared<Timer>())
{

}

AttackState::~AttackState()
{
    firingTimer_ = nullptr;
}

void AttackState::enter(const std::shared_ptr<EnemyBase>& enemy)
{
    //発射タイマーのカウントダウン開始
    firingTimer_->init();
    firingTimer_->startCountDown(fire_interval, 0);
}

void AttackState::exit(const std::shared_ptr<EnemyBase>& enemy)
{

}

void AttackState::update(const std::shared_ptr<EnemyBase>& enemy)
{
    firingTimer_->update();

    const auto status = enemy->getEnemyStatus();

    //ターゲットの方向を向くようにする
    status->faceToTarget();

    //射線が被ってたら移動する
    if (status->getIsRayObstructed())
    {
        enemy->toMoveState();
        return;
    }

    //射線上に何もなければ移動する
    if (!status->getIsRayObstructed() && !status->getHasClearShotToTarget())
    {
        enemy->toMoveState();
        return;
    }

    //発射タイマーのカウントダウンが終われば発射
    if (!firingTimer_->hasFinishedCountDown()) return;
    enemy->fire();

    //再びカウントダウンを開始
    firingTimer_->startCountDown(fire_interval, 0);
}

void AttackState::draw(const std::shared_ptr<EnemyBase>& enemy)
{

}