#include"DxLib.h"
#include"StageStatus.h"
#include"CameraBrain.h"
#include"EffekseerForDXLib.h"
#include"PostProcessing.h"

CameraBrain::CameraBrain(const std::shared_ptr<PlayerStatus>& status):
	effectTimer_(std::make_shared<Timer>())
{
	init(status);

	//ゼットバッファを有効にする
	SetUseZBufferFlag(true);

	//Ｚバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	//カメラ情報リセット防止
	SetUseSetDrawScreenSettingReset(false);
}

CameraBrain::~CameraBrain()
{
	currentCamera_ = nullptr;
}

void CameraBrain::init(const std::shared_ptr<PlayerStatus>& status)
{
	currentCamera_ = normalCamera_(status);
	currentCamera_->init();
	effectTimer_->init();
}

void CameraBrain::update(const std::shared_ptr<PlayerStatus>& playerStatus, const std::shared_ptr<StageStatus> stageStatus)
{
	SetCameraNearFar(camera_near, camera_far);

	effectTimer_->update();
	if (effectTimer_->hasFinishedCountDown())
	{
		currentCamera_ = normalCamera_(playerStatus);
		currentCamera_->update(playerStatus->getPosition(), std::dynamic_pointer_cast<PlayerStatus>(playerStatus)->getModelAngleY());
	}
	else
	{
		currentCamera_->update(stageStatus->getLookAtStartingPosition());
	}
	
	//DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();
}

void CameraBrain::postEffect()
{
	if (currentCamera_->getPostProcessing() == nullptr) return;

	currentCamera_->getPostProcessing()->draw();
}

void CameraBrain::changeCamera(const std::shared_ptr<ICamera>& nextCamera)
{
	currentCamera_ = nextCamera;
}

void CameraBrain::startEffectCamera()
{
	effectTimer_->startCountDown(rotation_effect_offset_seconds, 0);
	changeCamera(rotationEffectCamera_());
}