#include"EffekseerForDXLib.h"
#include"MuzzleFlashEffect.h"

MuzzleFlashEffect::MuzzleFlashEffect()
{
	data_			= LoadDataJson("statusData/effectData/muzzleFlashEffectStatus.json");
	effectHandle_	= LoadEffekseerEffect(data_["model_path"].get<std::string>().c_str());

	init();
}

MuzzleFlashEffect::~MuzzleFlashEffect()
{
	stop();
	//DeleteEffekseerEffect(effectHandle_);
}

void MuzzleFlashEffect::init()
{
	position_		= VGet(0.0f, 0.0f, 0.0f);
	playingHandle_	= -1;
}

void MuzzleFlashEffect::update()
{
	////エフェクトの再生が終わったかつ再生をしていたなら再生を止める
	if (IsEffekseer3DEffectPlaying(playingHandle_) == -1 && playingHandle_ != -1)
	{
		stop();
		return;
	}

	SetPosPlayingEffekseer3DEffect(playingHandle_, position_.x, position_.y, position_.z);
}

void MuzzleFlashEffect::draw()
{
	DrawEffekseer3D_Draw(playingHandle_);
}

void MuzzleFlashEffect::play()
{
	//再生するエフェクトの情報をセット
	const float scale			= data_["model_scale"].get<float>();
	const float playingSpeed	= data_["play_speed"].get<float>();
	playingHandle_				= PlayEffekseer3DEffect(effectHandle_);
	SetScalePlayingEffekseer3DEffect(playingHandle_, scale, scale, scale);
	SetSpeedPlayingEffekseer3DEffect(playingHandle_, playingSpeed);
}

void MuzzleFlashEffect::stop()
{
	StopEffekseer3DEffect(playingHandle_);
	playingHandle_ = -1;	//dxlibのエラー値に合わせてる
}

void MuzzleFlashEffect::setPosition(const VECTOR playingPosition)
{
	position_ = playingPosition;
}

void MuzzleFlashEffect::setRotationAngle(const VECTOR rotation)
{

}

void MuzzleFlashEffect::setScale(const float scale)
{

}

void MuzzleFlashEffect::setSpeed(const float speed)
{

}