#include"EffekseerForDXLib.h"
#include"ExplosionEffect.h"

ExplosionEffect::ExplosionEffect()
{
	data_ = LoadDataJson("statusData/effectData/ExplosionEffectStatus.json");
	for (int i = 0; i < data_["max_pool_value"].get<int>(); i++)
	{
		statuses_.emplace_back();
		statuses_[i].effectHandle = LoadEffekseerEffect(data_["model_path"].get<std::string>().c_str());
	}
	
	init();
}

ExplosionEffect::~ExplosionEffect()
{
	stop();
	for (const auto& status : statuses_)
	{
		DeleteEffekseerEffect(status.effectHandle);
	}
	statuses_.clear();
}

void ExplosionEffect::init()
{
	for (auto& status : statuses_)
	{
		status.isUsing			= false;
		status.position			= VGet(0.0f, 0.0f, 0.0f);
		status.playingHandle	= -1;
	}

	setDefualtScale();
}

void ExplosionEffect::update()
{
	stopFinishingEffect();	//範囲for文の中でstopを呼ぶのは処理の無駄な気がするのでstopの中に範囲for文と条件を書いてまいふれ呼び出すことにした。（多態性の問題でstopに引数は追加できなかった）
}

void ExplosionEffect::draw()
{
	for (const auto& status : statuses_)
	{
		DrawEffekseer3D_Draw(status.playingHandle);
	}
}

void ExplosionEffect::play()
{
	const float playingSpeed = data_["play_speed"].get<float>();
	
	for (auto& status : statuses_)
	{
		//既に使用済みなら使用してないものを探す
		if (status.isUsing) continue;

		status.playingHandle	= PlayEffekseer3DEffect(status.effectHandle);
		status.isUsing			= true;
		SetScalePlayingEffekseer3DEffect(status.playingHandle, modelNormalScale_, modelNormalScale_, modelNormalScale_);
		SetSpeedPlayingEffekseer3DEffect(status.playingHandle, playingSpeed);
		
		return;
	}
}

void ExplosionEffect::stop()
{
	for (auto& status : statuses_)
	{
		StopEffekseer3DEffect(status.playingHandle);
		status.playingHandle	= -1;	//dxlibのエラー値に合わせてる
		status.isUsing			= false;
	}
}

void ExplosionEffect::setPosition(const VECTOR playingPosition)
{
	for (auto& status : statuses_)
	{
		if (!status.isUsing) continue;

		status.position = playingPosition;
		SetPosPlayingEffekseer3DEffect(status.playingHandle, status.position.x, status.position.y, status.position.z);
	}
}

void ExplosionEffect::setRotationAngle(const VECTOR rotation)
{

}

void ExplosionEffect::stopFinishingEffect()
{
	for (auto& status : statuses_)
	{
		//エフェクトの再生が終わったかつ再生をしていたなら
		if (IsEffekseer3DEffectPlaying(status.playingHandle) == -1 && status.playingHandle != -1)
		{
			StopEffekseer3DEffect(status.playingHandle);
			status.playingHandle	= -1;	//dxlibのエラー値に合わせてる
			status.isUsing			= false;
		}
	}
}

void ExplosionEffect::setScale(const float scale)
{
	modelNormalScale_ = scale;
}

void ExplosionEffect::setSpeed(const float speed)
{

}

void ExplosionEffect::setDefualtScale()
{
	modelNormalScale_ = data_["model_scale"].get<float>();
}