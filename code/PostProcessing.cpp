#include"DxLibForIreven.h"
#include<string>
#include"PostProcessing.h"
#include<cmath>

PostProcessing::PostProcessing(const std::string& graphPath)
{
	graphHandle_ = LoadGraph(graphPath.c_str());

	init();
}

PostProcessing::~PostProcessing()
{
	DeleteGraph(graphHandle_);
	DeleteGraph(screenHandle_);
}

void PostProcessing::init()
{
	currentType_		= PostEffectType::NONE;
	alphaLerpTime_		= 0.0f;
	addLerpTimeValue_	= 0.0f;
}

void PostProcessing::update()
{
	if (currentType_ == PostEffectType::NONE) return;

	alphaLerpTime_ += addLerpTimeValue_;
	if (alphaLerpTime_ >= max_alpha_lerp_time)
	{
		init();
	}
}

void PostProcessing::draw()
{
	const short maxShortValue = 255;

	if (currentType_ == PostEffectType::GRAPH)
	{
		//らーぷでだんだん画像の透過率を変更して描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, maxShortValue * (max_alpha_lerp_time - alphaLerpTime_));
		DrawExtendGraph(0, 0, init_screen_width, init_screen_height, graphHandle_, true);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, maxShortValue);
	}
	else if (currentType_ == PostEffectType::GAUSS)
	{

	}
}

/// <summary>
/// ポストエフェクトを開始する
/// ※ポストエフェクトの種類を設定するとポストエフェクトを開始する設計になってる
/// </summary>
/// <param name="type">ポストエフェクトの種類</param>
/// <param name="addLerpTimeValue">ポストエフェクトをらーぷさせる値（らーぷしたくなければaddの値を0にすればok）</param>
void PostProcessing::start(const PostEffectType type, const float addLerpTimeValue)
{
	if (type == PostEffectType::NONE) return;

	currentType_		= type;
	alphaLerpTime_		= 0.0f;
	addLerpTimeValue_	= addLerpTimeValue;
}

/// <summary>
/// ポストエフェクトを強制終了する（らーぷさせない場合はこれを呼んでポストエフェクトを終了させる）
/// </summary>
void PostProcessing::finish()
{
	init();
}