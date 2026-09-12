#include"DxLib.h"
#include"PlayerStatus.h"
#include"InputHandler.h"
#include"Dash.h"
#include"WalkEvent.h"

Dash::Dash()
{
	
}

Dash::~Dash()
{
}

void Dash::update(const std::shared_ptr<StatusBase3D>& status, const bool canUpdate)
{
	const std::shared_ptr<PlayerStatus> playerStatus = std::dynamic_pointer_cast<PlayerStatus>(status);

	if (canUpdate)
	{
		enter(status);

		//前フレームの移動ベクトルを保存
		prevMoveDirection_ = moveDirection_;

		//移動方向を決定
		//切り返ししてなかったら
		if (!isTurn_)
		{
			playerStatus->addDashSpeed();

			//移動の初期値をセット
			moveDirection_ = left_direction;

			//移動方向を決定
			float moveAngle		= static_cast<float>(InputHandler::instance().getWalkEvent()->getStickAngleTrigger()()) + playerStatus->getModelAngleY();
			MATRIX moveRotation = MGetRotY(moveAngle);
			moveDirection_ = VTransform(moveDirection_, moveRotation);
			moveDirection_ = VNorm(moveDirection_);
		}

		//移動の切り替えしを検知
		float dot = VDot(prevMoveDirection_, moveDirection_);
		if (dot < 0.0f || isTurn_)
		{
			//減速する
			isTurn_ = true;

			//切り返し終わり
			if (playerStatus->getSpeed() <= 0.0f)
			{
				isTurn_ = false;
			}

			playerStatus->movePosition(prevMoveDirection_);
		}
	}
	else
	{
		exit(status);
	}

	//座標の移動は必ずする
	playerStatus->movePosition(moveDirection_);
}