#include"DxLib.h"
#include"ExpItem.h"

ExpItem::ExpItem(const float expValue, const float moveSpeed, const VECTOR spawnPosition, const int modelHandle):
	exp_value(expValue),
	move_speed(moveSpeed)
{
	position_	= spawnPosition;
	canDestroy_ = false;

	//スタティックのモデルを再利用して読み込み
	modelHandle_ = MV1DuplicateModel(modelHandle);

	MV1SetScale(modelHandle_, VGet(model_scale, model_scale, model_scale));
}

ExpItem::~ExpItem()
{
}

void ExpItem::update(const VECTOR playerPosition)
{
	moveToPlayer(playerPosition);

	MV1SetPosition(modelHandle_, position_);
}

void ExpItem::draw()
{
	MV1DrawModel(modelHandle_);
}

void ExpItem::moveToPlayer(const VECTOR playerPosition)
{
	//プレイヤーへの方向ベクトルを求める
	VECTOR direction = VSub(playerPosition, position_);

	const float distance = VSize(direction);

	//すでにプレイヤー位置にいる場合は処理しない
	if (distance <= 0.0f) return;

	direction = VNorm(direction);

	//プレイヤーに向かって移動
	position_ = VAdd(position_, VScale(direction, move_speed));
}

void ExpItem::setCanDestroy(const bool canDestroy)
{
	canDestroy_ = canDestroy;
}