#include"EffekseerForDXLib.h"
#include"EngineEffect.h"
#include"InputHandler.h"
#include"DashEvent.h"
#include"RaiseEvent.h"
#include"DescentEvent.h"

EngineEffect::EngineEffect()
{
	data_			= LoadDataJson("statusData/effectData/engineEffectStatus.json");
	effectHandle_	= LoadEffekseerEffect(data_["model_path"].get<std::string>().c_str());

	init();
}

EngineEffect::~EngineEffect()
{
	stop();
	//DeleteEffekseerEffect(effectHandle_);
}

void EngineEffect::init()
{
	stop();
	position_		= VGet(0.0f, 0.0f, 0.0f);
	playingHandle_	= -1;
	rotation_		= VGet(0.0f, 0.0f, 0.0f);
}

void EngineEffect::update()
{
	////エフェクトの再生が終わったかつ再生をしていたなら再生を止める
	if (IsEffekseer3DEffectPlaying(playingHandle_) == -1 && playingHandle_ != -1)
	{
		play();
		return;
	}

	changeColor();

	SetPosPlayingEffekseer3DEffect(playingHandle_, position_.x, position_.y, position_.z);
	SetRotationPlayingEffekseer3DEffect(playingHandle_, rotation_.x, rotation_.y, rotation_.z - DX_PI_F * 0.5f);
}

void EngineEffect::draw()
{
	DrawEffekseer3D_Draw(playingHandle_);
}

void EngineEffect::play()
{
	//再生するエフェクトの情報をセット
	const float scale			= data_["model_scale"].get<float>();
	const float playingSpeed	= data_["play_speed"].get<float>();
	playingHandle_				= PlayEffekseer3DEffect(effectHandle_);
	SetScalePlayingEffekseer3DEffect(playingHandle_, scale, scale, scale);
	SetSpeedPlayingEffekseer3DEffect(playingHandle_, playingSpeed);
}

void EngineEffect::stop()
{
	StopEffekseer3DEffect(playingHandle_);
	playingHandle_ = -1;	//dxlibのエラー値に合わせてる
}

void EngineEffect::setPosition(const VECTOR playingPosition)
{
	position_ = playingPosition;
}

void EngineEffect::setRotationAngle(const VECTOR rotation)
{
	rotation_ = rotation;
}

void EngineEffect::changeColor()
{
	const char rad		= 0;
	const char green	= 1;
	const char blue		= 2;

	if (InputHandler::instance().getRaiseEvent()->getRaiseTrigger()())
	{
		const auto raiseColor = data_["raise_engine_color"];
		SetColorPlayingEffekseer3DEffect(playingHandle_, raiseColor[rad], raiseColor[green], raiseColor[blue], 255);
	}
	else if(InputHandler::instance().getDescentEvent()->getDescentTrigger()())
	{
		const auto descentColor = data_["descent_engine_color"];
		SetColorPlayingEffekseer3DEffect(playingHandle_, descentColor[rad], descentColor[green], descentColor[blue], 255);
	}
	else
	{
		const auto normalColor = data_["normal_engine_color"];
		SetColorPlayingEffekseer3DEffect(playingHandle_, normalColor[rad], normalColor[green], normalColor[blue], 255);
	}
}

void EngineEffect::setScale(const float scale)
{

}

void EngineEffect::setSpeed(const float speed)
{

}