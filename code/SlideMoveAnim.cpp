#include"DxLibForIreven.h"
#include"playerStatus.h"
#include"InputHandler.h"
#include<memory>
#include"IAnimation.h"
#include"SlideMoveAnim.h"
#include"WalkEvent.h"

SlideMoveAnim::SlideMoveAnim()
{
	init();

	InputHandler::instance().getWalkEvent()->addStartingEvent([this]() { enter(); });
	InputHandler::instance().getWalkEvent()->addFinishingEvent([this]() { exit(); });
}

SlideMoveAnim::~SlideMoveAnim()
{
	MV1DeleteModel(modelHandle_);
}

void SlideMoveAnim::init()
{
	MV1DetachAnim(modelHandle_, backAnimAttach_);
	MV1DetachAnim(modelHandle_, frontAnimAttach_);
	MV1DetachAnim(modelHandle_, leftAnimAttach_);
	MV1DetachAnim(modelHandle_, rightAnimAttach_);

	modelHandle_		= 0;
	onUpdate_			= false;
	canRelease_			= false;
	currentTotalTime_	= 0.0f;
	returnIdleCounter_	= 0.0f;
	canReturnIdle_		= false;

	//dxlibのアタッチのエラーが-1なので初期値を-1にする
	frontAnimAttach_	= -1;
	backAnimAttach_		= -1;
	rightAnimAttach_	= -1;
	leftAnimAttach_		= -1;

	currentTime_		= 0.0f;
	blendRate_			= 0.5f;
	curretnSitckAngle_	= 0.0f;

	prevAnimAttachFrontBack_	= -1;
	prevAnimAttachRightLeft_	= -1;
	prevBlendCounter_			= 0.0f;
	prevBlendRate_				= 0.0f;
	prevStickPostion_			= VGet(0.0f, 0.0f, 0.0f);
	currentStickPostion_		= VGet(0.0f, 0.0f, 0.0f);;

	prevAttachFront_	= -1;
	prevAttachBack_		= -1;
	prevAttachRight_	= -1;
	prevAttachLeft_		= -1;
}

void SlideMoveAnim::enter()
{
	onUpdate_ = true;
}

void SlideMoveAnim::exit()
{
	//currentTime_が0.0fのときはスティックを動かしてない状態が続いてるので処理しない
	if (currentTime_ == 0.0f) return;

	canReturnIdle_ = true;
}

void SlideMoveAnim::update(const std::shared_ptr<PlayerStatus>& status)
{
	//モデルハンドルを入れる
	if (modelHandle_ == 0)
	{
		modelHandle_ = status->getModelHandle();
	}

	//スティックの入力が一瞬0になり、アニメーションがおかしくなる対策
	if (canReturnIdle_)
	{
		stickZeroGrace();

		return;	//ほかの処理はしないようにする
	}
	else
	{
		//スティックの情報をセット
		setStickInfo();
	}

	if (!onUpdate_) return;

	attach();

	blend();

	playingAnimation();

	//デバッグ
	/*DrawFormatString(0, 200, GetColor(255, 255, 255), "スティック角度：%f", curretnSitckAngle_);
	DrawFormatString(0, 400, GetColor(255, 255, 255), "ブレンド率：%f", blendRate_);
	DrawFormatString(0, 450, GetColor(255, 255, 255), "前ブレンド率：%f", prevBlendRate_);
	DrawFormatString(0, 500, GetColor(255, 255, 255), "prevBack：%d", prevAttachBack_);
	DrawFormatString(0, 550, GetColor(255, 255, 255), "prevFront：%d", prevAttachFront_);
	DrawFormatString(0, 600, GetColor(255, 255, 255), "prevRight：%d", prevAttachRight_);
	DrawFormatString(0, 650, GetColor(255, 255, 255), "prevLeft：%d", prevAttachLeft_);
	DrawFormatString(0, 700, GetColor(255, 255, 255), "back：%d", backAnimAttach_);
	DrawFormatString(0, 750, GetColor(255, 255, 255), "front：%d", frontAnimAttach_);
	DrawFormatString(0, 800, GetColor(255, 255, 255), "right：%d", rightAnimAttach_);
	DrawFormatString(0, 850, GetColor(255, 255, 255), "left：%d", leftAnimAttach_);*/
}

void SlideMoveAnim::attach()
{
	if (curretnSitckAngle_ > 0.0f)
	{
		attachBack();
	}
	else if (curretnSitckAngle_ < 0.0f)
	{
		attachFront();
	}

	if (curretnSitckAngle_ < DX_PI_F / 2 && curretnSitckAngle_ > -DX_PI_F / 2)
	{
		attachRight();
	}
	else if (back_pi < curretnSitckAngle_ && curretnSitckAngle_ < left_pi
		|| -left_pi < curretnSitckAngle_ && curretnSitckAngle_ < front_pi
		|| curretnSitckAngle_ == left_pi)
	{
		attachLeft();
	}
}

void SlideMoveAnim::playingAnimation()
{
	currentTime_ += anim_speed;
	currentTime_ = (std::min)(currentTime_, max_slide_time);

	if (backAnimAttach_ != -1)
	{
		MV1SetAttachAnimTime(modelHandle_, backAnimAttach_, currentTime_);
	}
	if (frontAnimAttach_ != -1)
	{
		MV1SetAttachAnimTime(modelHandle_, frontAnimAttach_, currentTime_);
	}
	if (rightAnimAttach_ != -1)
	{
		MV1SetAttachAnimTime(modelHandle_, rightAnimAttach_, currentTime_);
	}
	if (leftAnimAttach_ != -1)
	{
		MV1SetAttachAnimTime(modelHandle_, leftAnimAttach_, currentTime_);
	}
}

void SlideMoveAnim::attachFront()
{
	if (frontAnimAttach_ >= 0) return;

	//切り返しを行ってる時だけ
	if (VDot(prevStickPostion_, currentStickPostion_) < 0.0f)
	{
		prevBlendRate_				= blendRate_;
		prevBlendCounter_			= 0.0f;
		prevAnimAttachFrontBack_	= backAnimAttach_;
		prevAttachBack_				= backAnimAttach_;
	}
	else
	{
		//一緒になることがないアニメーションをデタッチ
		MV1DetachAnim(modelHandle_, backAnimAttach_);
		backAnimAttach_ = -1;
	}
	
	frontAnimAttach_ = MV1AttachAnim(modelHandle_, front_anim_index, -1, FALSE);
}

void SlideMoveAnim::attachBack()
{
	if (backAnimAttach_ >= 0) return;

	//切り返しを行ってる時だけ
	if (VDot(prevStickPostion_, currentStickPostion_) < 0.0f)
	{
		prevBlendRate_				= blendRate_;
		prevBlendCounter_			= 0.0f;
		prevAnimAttachFrontBack_	= frontAnimAttach_;
		prevAttachFront_			= frontAnimAttach_;
	}
	else
	{
		//一緒になることがないアニメーションをデタッチ
		MV1DetachAnim(modelHandle_, frontAnimAttach_);
		frontAnimAttach_ = -1;
	}
	
	backAnimAttach_ = MV1AttachAnim(modelHandle_, back_anim_index, -1, FALSE);
}

void SlideMoveAnim::attachRight()
{
	if (rightAnimAttach_ >= 0) return;

	//切り返しを行ってる時だけ
	if (VDot(prevStickPostion_, currentStickPostion_) < 0.0f)
	{
		prevBlendRate_				= blendRate_;
		prevBlendCounter_			= 0.0f;
		prevAnimAttachRightLeft_	= leftAnimAttach_;
		prevAttachLeft_				= leftAnimAttach_;
	}
	else
	{
		//一緒になることがないアニメーションをデタッチ
		MV1DetachAnim(modelHandle_, leftAnimAttach_);
		leftAnimAttach_ = -1;
	}
	
	rightAnimAttach_ = MV1AttachAnim(modelHandle_, right_anim_index, -1, FALSE);
}

void SlideMoveAnim::attachLeft()
{
	if (leftAnimAttach_ >= 0) return;

	//切り返しを行ってる時だけ
	if (VDot(prevStickPostion_, currentStickPostion_) < 0.0f)
	{
		prevBlendRate_				= blendRate_;
		prevBlendCounter_			= 0.0f;
		prevAnimAttachRightLeft_	= rightAnimAttach_;
		prevAttachRight_			= rightAnimAttach_;
	}
	else
	{
		//一緒になることがないアニメーションをデタッチ
		MV1DetachAnim(modelHandle_, rightAnimAttach_);
		rightAnimAttach_ = -1;
	}
	
	leftAnimAttach_ = MV1AttachAnim(modelHandle_, left_anim_index, -1, FALSE);
}

void SlideMoveAnim::blend()
{
	//アニメーションの切り替えのブレンド率を調整
	float switchBlendRateFront	= 0.0f;
	float switchBlendRateRight	= 0.0f;
	float switchBlendRateBack	= 0.0f;
	float switchBlendRateLeft	= 0.0f;

	if (prevBlendCounter_ < 1.0f)
	{
		if (prevAttachBack_ != -1 || prevAttachFront_ != -1 || prevAttachRight_ != -1 || prevAttachLeft_ != -1)
		{
			prevBlendCounter_ += add_counter_value;

			//前のブレンドしてたアニメーションをデタッチ
			detachPrevAnim();

			switchBlendRateFront	= std::lerp(1.0f - prevBlendRate_, 0.0f, prevBlendCounter_);
			switchBlendRateRight	= std::lerp(prevBlendRate_, 0.0f, prevBlendCounter_);
			switchBlendRateBack		= std::lerp(1.0f - prevBlendRate_, 0.0f, prevBlendCounter_);
			switchBlendRateLeft		= std::lerp(prevBlendRate_, 0.0f, prevBlendCounter_);

			//切り替えアニメーションブレンド
			MV1SetAttachAnimBlendRate(modelHandle_, prevAnimAttachFrontBack_, switchBlendRateFront);
			MV1SetAttachAnimBlendRate(modelHandle_, prevAnimAttachRightLeft_, switchBlendRateRight);
			MV1SetAttachAnimBlendRate(modelHandle_, prevAnimAttachFrontBack_, switchBlendRateBack);
			MV1SetAttachAnimBlendRate(modelHandle_, prevAnimAttachRightLeft_, switchBlendRateLeft);
		}
		else
		{
			//一つのアニメーションしか流れないと確定しているときは他をデタッチ
			detachNonOneAnim();

			prevBlendCounter_ = 0.0f;
		}
	}
	
	//右後ろ移動
	if (right_pi < curretnSitckAngle_ && curretnSitckAngle_ < back_pi)
	{
		blendRate_ = NormalizeBetween(curretnSitckAngle_, right_pi, back_pi);

		MV1SetAttachAnimBlendRate(modelHandle_, rightAnimAttach_, 1.0f - blendRate_ - switchBlendRateLeft);
		MV1SetAttachAnimBlendRate(modelHandle_, backAnimAttach_, blendRate_ - switchBlendRateFront);
		return;
	}

	//左後ろ移動
	if (back_pi <= curretnSitckAngle_ && curretnSitckAngle_ <= left_pi)
	{
		blendRate_ = NormalizeBetween(curretnSitckAngle_, back_pi, left_pi);

		MV1SetAttachAnimBlendRate(modelHandle_, leftAnimAttach_, blendRate_ - switchBlendRateRight);
		MV1SetAttachAnimBlendRate(modelHandle_, backAnimAttach_, 1.0f - blendRate_ - switchBlendRateFront);
		return;
	}

	//右前移動
	if (front_pi <= curretnSitckAngle_ && curretnSitckAngle_ <= right_pi)
	{
		blendRate_ = NormalizeBetween(curretnSitckAngle_, front_pi, right_pi);

		MV1SetAttachAnimBlendRate(modelHandle_, rightAnimAttach_, blendRate_ - switchBlendRateLeft);
		MV1SetAttachAnimBlendRate(modelHandle_, frontAnimAttach_, 1.0f - blendRate_ - switchBlendRateBack);
		return;
	}

	//左前移動
	if (-left_pi <= curretnSitckAngle_ && curretnSitckAngle_ <= front_pi || curretnSitckAngle_ == left_pi)
	{
		blendRate_ = NormalizeBetween(curretnSitckAngle_, -left_pi, front_pi);

		MV1SetAttachAnimBlendRate(modelHandle_, leftAnimAttach_, 1.0f - blendRate_ - switchBlendRateRight);
		MV1SetAttachAnimBlendRate(modelHandle_, frontAnimAttach_, blendRate_ - switchBlendRateBack);
		return;
	}
}

void SlideMoveAnim::stickZeroGrace()
{
	++returnIdleCounter_;

	//入力があればアイドルには戻らない
	if (InputHandler::instance().getWalkEvent()->getWalkingTrigger()())	//最後に()がないとエラーになるので注意。最後の()でfunctionから実際の型を取り出す
	{
		canReturnIdle_		= false;
		returnIdleCounter_	= 0.0f;
	}

	//スティックを動かさずに一定時間たったらidleに戻す
	if (returnIdleCounter_ > max_return_idle_count)
	{
		currentTime_ = 0.0f;	//動いてなければスライド移動アニメーションは流れない
		MV1DetachAnim(modelHandle_, backAnimAttach_);
		MV1DetachAnim(modelHandle_, frontAnimAttach_);
		MV1DetachAnim(modelHandle_, leftAnimAttach_);
		MV1DetachAnim(modelHandle_, rightAnimAttach_);
		backAnimAttach_		= -1;
		frontAnimAttach_	= -1;
		leftAnimAttach_		= -1;
		rightAnimAttach_	= -1;

		onUpdate_			= false;
		canReturnIdle_		= false;
		returnIdleCounter_	= 0.0f;
	}
}

void SlideMoveAnim::setStickInfo()
{
	const std::shared_ptr<WalkEvent> walkEvent = InputHandler::instance().getWalkEvent();

	//スティック関係の値を保存
	curretnSitckAngle_		= static_cast<float>(walkEvent->getStickAngleTrigger()());	//最後に()がないとエラーになるので注意。最後の()でfunctionから実際の型を取り出す
	prevStickPostion_		= currentStickPostion_;
	currentStickPostion_	= VGet(static_cast<float>(walkEvent->getStickXTrigger()()),
		static_cast<float>(walkEvent->getStickYTrigger()()), 0.0);
}

void SlideMoveAnim::detachPrevAnim()
{
	//前のブレンドのカウントが1未満ならデタッチしない
	if (prevBlendCounter_ < 1.0f) return;

	//ブレンドのカウントは1を超えない
	prevBlendCounter_ = 1.0f;

	if (prevAttachFront_ != -1)
	{
		MV1DetachAnim(modelHandle_, prevAttachFront_);
		prevAttachFront_ = -1;
		frontAnimAttach_ = -1;
	}
	if (prevAttachBack_ != -1)
	{
		MV1DetachAnim(modelHandle_, prevAttachBack_);
		prevAttachBack_ = -1;
		backAnimAttach_ = -1;
	}
	if (prevAttachRight_ != -1)
	{
		MV1DetachAnim(modelHandle_, prevAttachRight_);
		prevAttachRight_ = -1;
		rightAnimAttach_ = -1;
	}
	if (prevAttachLeft_ != -1)
	{
		MV1DetachAnim(modelHandle_, prevAttachLeft_);
		prevAttachLeft_ = -1;
		leftAnimAttach_ = -1;
	}
}

void SlideMoveAnim::detachNonOneAnim()
{
	if (curretnSitckAngle_ == front_pi)
	{
		MV1DetachAnim(modelHandle_, backAnimAttach_);
		MV1DetachAnim(modelHandle_, rightAnimAttach_);
		MV1DetachAnim(modelHandle_, leftAnimAttach_);
		backAnimAttach_		= -1;
		rightAnimAttach_	= -1;
		leftAnimAttach_		= -1;
	}
	else if (curretnSitckAngle_ == back_pi)
	{
		MV1DetachAnim(modelHandle_, frontAnimAttach_);
		MV1DetachAnim(modelHandle_, rightAnimAttach_);
		MV1DetachAnim(modelHandle_, leftAnimAttach_);
		frontAnimAttach_	= -1;
		rightAnimAttach_	= -1;
		leftAnimAttach_		= -1;
	}
	else if (curretnSitckAngle_ == right_pi)
	{
		MV1DetachAnim(modelHandle_, backAnimAttach_);
		MV1DetachAnim(modelHandle_, frontAnimAttach_);
		MV1DetachAnim(modelHandle_, leftAnimAttach_);
		backAnimAttach_		= -1;
		frontAnimAttach_	= -1;
		leftAnimAttach_		= -1;
	}
	else if (curretnSitckAngle_ == left_pi)
	{
		MV1DetachAnim(modelHandle_, backAnimAttach_);
		MV1DetachAnim(modelHandle_, rightAnimAttach_);
		MV1DetachAnim(modelHandle_, frontAnimAttach_);
		backAnimAttach_		= -1;
		rightAnimAttach_	= -1;
		frontAnimAttach_	= -1;
	}
}