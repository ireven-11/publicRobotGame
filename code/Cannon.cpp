#include"DxLib.h"
#include"IWeapon.h"
#include"WeaponStatus.h"
#include"Cannon.h"
#include"CannonStatus.h"
#include"IBullet.h"
#include"BulletStatus.h"
#include"NormalBullet.h"
#include"RightFireBulletEvent.h"
#include"LeftFireBulletEvent.h"
#include"BulletEventHandler.h"

Cannon::Cannon(const EquipSlot equipSlot)
	:status_(std::make_shared<CannonStatus>()),
	currentEquipSlot_(equipSlot)
{
	for (auto i = 0; i < status_->getStatusData()["bullets_pool_value"].get<int>(); i++)
	{
		bullets_.emplace_back(std::make_shared<NormalBullet>(status_->getAttackPower(), BulletOwnerType::PLAYER));
	}

	init();
}

Cannon::~Cannon()
{
	bullets_.clear();
}

void Cannon::init()
{
	startTime_ = GetNowCount();
}

void Cannon::update()
{
	const int	now			= GetNowCount();
	const float fireRate	= status_->getFireRate() * one_seconds_dxlib;	//dxlibではint1000カウントで1秒なのでかける

	//まだ発射タイミングじゃない
	if ((now - startTime_) < fireRate) return;

	for (const auto& bullet : bullets_)
	{
		const std::shared_ptr<BulletStatus> bulletStatus = bullet->getStatus();

		//弾がすでに使われてたら処理しない
		if (bulletStatus->getIsUsing()) continue;

		const std::shared_ptr<CannonStatus> cannonStatus = std::static_pointer_cast<CannonStatus>(status_);

		const MATRIX tempMatrix			= MV1GetFrameLocalWorldMatrix(status_->getModelHandle(), cannonStatus->getMuzzleBoneNumber());
		const VECTOR rotationDirection	= VNorm(VGet(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]));	//上方向を基準にしてる

		//弾を使用＋発射準備
		bulletStatus->use(MV1GetFramePosition(status_->getModelHandle(), cannonStatus->getMuzzleBoneNumber()),
			status_->getFireBulletSpeed(), rotationDirection);

		//弾を撃った時のイベントを呼び出し
		occurEvent();

		startTime_ = now; //発射したら、今の時間を保存しておいて前の発射時間とする
		break;	//弾を同じフレームで連続して撃つことはない
	}
}

void Cannon::draw()
{
	MV1DrawModel(status_->getModelHandle());
}

void Cannon::setTransform(const MATRIX& equipingMatrix)
{
	//武器の大きさを調整する
	float tempScale		= status_->getStatusData()["model_scale"].get<float>();
	MATRIX scaleMatrix	= MGetScale(VGet(tempScale, tempScale, tempScale));	//大きさ用の行列を作成	
	MATRIX tempMatrix	= MMult(scaleMatrix, equipingMatrix);				//行列なのでかける順番が逆だとうまくいかないので注意

	//武器の向きを調整する
	MATRIX rotationX	= MGetRotX(DX_PI_F);
	MATRIX rotationZ	= MGetRotZ(DX_PI_F * 0.5f);
	tempMatrix			= MMult(rotationX, tempMatrix);
	tempMatrix			= MMult(rotationZ, tempMatrix);

	//行列で座標と回転を更新
	MV1SetMatrix(status_->getModelHandle(), tempMatrix);

	//マズルの座標もセットする
	status_->setMuzzlePosition();
}

void Cannon::occurEvent()
{
	switch (currentEquipSlot_)
	{
		case EquipSlot::NONE:
		{
			break;
		}
		case EquipSlot::RIGHT_HAND:
		{
			BulletEventHandler::instance().getRightFireBulletEvent()->occurStartingEvent();

			break;
		}
		case EquipSlot::LEFT_HAND:
		{
			BulletEventHandler::instance().getLeftFireBulletEvent()->occurStartingEvent();

			break;
		}
		case EquipSlot::RIGHT_SHOULDER:
		{
			break;
		}
		default:
		{
			break;
		}
	}
}