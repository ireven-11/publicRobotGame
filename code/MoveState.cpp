#include"DxLibForIreven.h"
#include"EnemyBase.h"
#include"MoveState.h"

MoveState::MoveState()
{

}

MoveState::~MoveState()
{

}

void MoveState::enter(const std::shared_ptr<EnemyBase>& enemy)
{

}

void MoveState::exit(const std::shared_ptr<EnemyBase>& enemy)
{

}

void MoveState::update(const std::shared_ptr<EnemyBase>& enemy)
{
    const auto status = enemy->getEnemyStatus();
    
    //目的地の方向に向いて移動
    status->rotationToDestinationDirection();
    status->moveToFacingDirection();

    //射線がかぶる対策
    status->shiftToSide();

    //目的地を次にする
    status->moveNextDestination();

    //ターゲットが射程外なら移動したまま
    if (!status->getIsTargetInAttackRange()) return;

    //射線に遮蔽がかぶってたら移動したまま
    if (status->getIsRayObstructed()) return;

    enemy->toAttackState();
}

void MoveState::draw(const std::shared_ptr<EnemyBase>& enemy)
{

}