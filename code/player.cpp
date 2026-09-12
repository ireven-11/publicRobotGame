#include"DxLibForIreven.h"
#include"InputHandler.h"
#include"PlayerAnimator.h"
#include"PlayerStatus.h"
#include"PlayerAction.h"
#include"Player.h"
#include"IWeapon.h"
#include"RightAttackEvent.h"
#include"LeftAttackEvent.h"
#include"RaiseEvent.h"
#include"DescentEvent.h"
#include"CameraEvent.h"
#include"Physics.h"
#include"WalkEvent.h"
#include"DashEvent.h"

Player::Player() 
    : action_(std::make_shared<PlayerAction>())
{
    status_     = std::make_shared<PlayerStatus>();
    animator_   = std::make_shared<PlayerAnimator>(std::dynamic_pointer_cast<PlayerStatus>(status_));

    const std::shared_ptr<PlayerStatus> playerStatus    = std::dynamic_pointer_cast<PlayerStatus>(status_);
    const int tempModel                                 = playerStatus->getModelHandle();

    //生成された時点でマップの正しい位置に配置しておく（カメラ演出時にマップオブジェクトのupdateが走らないため）
    MV1SetPosition(status_->getModelHandle(), status_->getPosition());
    playerStatus->getLeftWeapon()->setTransform(MV1GetFrameLocalWorldMatrix(tempModel, playerStatus->getModelBoneIndex()["Gun_L"]));
    playerStatus->getRightWeapon()->setTransform(MV1GetFrameLocalWorldMatrix(tempModel, playerStatus->getModelBoneIndex()["Gun_R"]));
    //playerStatus->getShoulderWeapon()->setTransform(MGetIdent());
}

Player::~Player()
{
}

void Player::init()
{
    status_->init();
    animator_->init();

    //モデルを初期化
    const auto tempModel = status_->getModelHandle();
    MV1SetRotationXYZ(tempModel, VGet(0.0f, 0.0f, 0.0f));
    MV1SetPosition(tempModel, status_->getPosition());
}

void Player::update()
{
    const std::shared_ptr<PlayerStatus> playerStatus    = std::dynamic_pointer_cast<PlayerStatus>(status_);
    const int tempModel                                 = playerStatus->getModelHandle();

    //死んだら一切の行動ができない
    if (playerStatus->getHp() > 0)
    {
        event();
        action();
        modelRotation();
        animator_->update();
        playerStatus->levelUp();
    }

    MV1SetPosition(tempModel, status_->getPosition());

    playerStatus->setColliderPosition();

    playerStatus->getLeftWeapon()->setTransform(MV1GetFrameLocalWorldMatrix(tempModel, playerStatus->getModelBoneIndex()["Gun_L"]));
    playerStatus->getRightWeapon()->setTransform(MV1GetFrameLocalWorldMatrix(tempModel, playerStatus->getModelBoneIndex()["Gun_R"]));
    //playerStatus->getShoulderWeapon()->setTransform(MGetIdent());
}

void Player::draw()const
{
    const auto playerStatus = std::dynamic_pointer_cast<PlayerStatus>(status_);

    playerStatus->getLeftWeapon()->draw();
    playerStatus->getRightWeapon()->draw();
    //playerStatus->getShoulderWeapon()->draw();

    MV1DrawModel(status_->getModelHandle());
}

void Player::action()
{
    std::shared_ptr<PlayerStatus> playerStatus = std::dynamic_pointer_cast<PlayerStatus>(status_);

    playerStatus->decelerate();

    const bool isWalking = InputHandler::instance().getWalkEvent()->getWalkingTrigger()();
    action_->walk(status_, isWalking);

    const char isDashing = InputHandler::instance().getDashEvent()->getDashTrigger()();
    action_->dash(status_, isDashing && isWalking);

    action_->raise(status_, InputHandler::instance().getRaiseEvent()->getRaiseTrigger()());

    action_->descent(status_, InputHandler::instance().getDescentEvent()->getDescentTrigger()());

    action_->rightAttack(status_, InputHandler::instance().getRightAttackEvent()->getRightAttackingTrigger()());

    action_->leftAttack(status_, InputHandler::instance().getLeftAttackEvent()->getLeftAttackingTrigger()());

    action_->shoulderAttack(status_, false);
}

void Player::modelRotation()
{
    const int stickTiltValue    = InputHandler::instance().getCameraEvent()->getStickXTrigger()();
    const auto playerStatus     = std::dynamic_pointer_cast<PlayerStatus>(status_);

    if (stickTiltValue == 0) return;
    
    if (stickTiltValue < 0)
    {
        playerStatus->rightRotationModelAngleY();
    }
    else
    {
        playerStatus->leftRotationModelAngleY();
    }

    MV1SetRotationXYZ(status_->getModelHandle(), VGet(0.0f, status_->getModelAngleY(), 0.0f));
}

void Player::event()
{
    InputHandler::instance().getRightAttackEvent()->occurStartingEvent();
    InputHandler::instance().getRightAttackEvent()->occurPlayingEvent();
    InputHandler::instance().getRightAttackEvent()->occurFinishingEvent();

    InputHandler::instance().getLeftAttackEvent()->occurStartingEvent();
    InputHandler::instance().getLeftAttackEvent()->occurPlayingEvent();
    InputHandler::instance().getLeftAttackEvent()->occurFinishingEvent();

    InputHandler::instance().getDashEvent()->occurStartingEvent();
    InputHandler::instance().getDashEvent()->occurPlayingEvent();
    InputHandler::instance().getDashEvent()->occurFinishingEvent();

    InputHandler::instance().getWalkEvent()->occurStartingEvent();
    InputHandler::instance().getWalkEvent()->occurPlayingEvent();
    InputHandler::instance().getWalkEvent()->occurFinishingEvent();

    InputHandler::instance().getRaiseEvent()->occurStartingEvent();
    InputHandler::instance().getRaiseEvent()->occurPlayingEvent();
    InputHandler::instance().getRaiseEvent()->occurFinishingEvent();

    InputHandler::instance().getDescentEvent()->occurStartingEvent();
    InputHandler::instance().getDescentEvent()->occurPlayingEvent();
    InputHandler::instance().getDescentEvent()->occurFinishingEvent();
}