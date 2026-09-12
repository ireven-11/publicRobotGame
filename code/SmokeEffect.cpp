#include"EffekseerForDXLib.h"
#include"SmokeEffect.h"

SmokeEffect::SmokeEffect()
{
	data_			= LoadDataJson("statusData/effectData/smokeEffectStatus.json");
	effectHandle_	= LoadEffekseerEffect(data_["model_path"].get<std::string>().c_str());

	init();
}

SmokeEffect::~SmokeEffect()
{
	stop();
	DeleteEffekseerEffect(effectHandle_);
}

void SmokeEffect::init()
{
	position_		= VGet(0.0f, 0.0f, 0.0f);
	playingHandle_	= -1;
}

void SmokeEffect::update()
{
	////エフェクトの再生が終わったかつ再生をしていたなら再生を止める
	if (IsEffekseer3DEffectPlaying(playingHandle_) == -1 && playingHandle_ != -1)
	{
		play();
		return;
	}

	SetPosPlayingEffekseer3DEffect(playingHandle_, position_.x, position_.y, position_.z);
}

void SmokeEffect::draw()
{
	//地面にしかエフェクトを出さない
	if (position_.y > 0.0f) return;

	DrawEffekseer3D_Draw(playingHandle_);
}

void SmokeEffect::play()
{
	//再生するエフェクトの情報をセット
	const float scale			= data_["model_scale"].get<float>();
	const float playingSpeed	= data_["play_speed"].get<float>();
	playingHandle_				= PlayEffekseer3DEffect(effectHandle_);
	SetScalePlayingEffekseer3DEffect(playingHandle_, scale, scale, scale);
	SetSpeedPlayingEffekseer3DEffect(playingHandle_, playingSpeed);
}

void SmokeEffect::stop()
{
	StopEffekseer3DEffect(playingHandle_);
	playingHandle_ = -1;	//dxlibのエラー値に合わせてる
}

void SmokeEffect::setPosition(const VECTOR playingPosition)
{
	position_ = playingPosition;
}

void SmokeEffect::setRotationAngle(const VECTOR rotation)
{

}

void SmokeEffect::setScale(const float scale)
{

}

void SmokeEffect::setSpeed(const float speed)
{

}