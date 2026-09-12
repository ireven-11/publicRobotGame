#include"DxLibForIreven.h"
#include"TrainingStageStatus.h"
#include"StageEventHandler.h"
#include"StageClearEvent.h"
#include"StageOverEvent.h"

TrainingStageStatus::TrainingStageStatus()
{
	statusData_ = LoadDataJson("statusData/stageData/trainingStageStatus.json");
	std::string modelPath = statusData_["model_path"].get<std::string>();
	modelHandle_ = MV1LoadModel(modelPath.c_str());	//dxlibの関数はstringを使えないのでc_str()で変換する

	float modelScale = statusData_["model_scale"].get<float>();
	MV1SetScale(modelHandle_, VGet(modelScale, modelScale, modelScale));

	missionText_ = Utf8ToSJIS(statusData_["mission_text"].get<std::string>());

	init();
}

TrainingStageStatus::~TrainingStageStatus()
{
	MV1DeleteModel(modelHandle_);
}

void TrainingStageStatus::init()
{
	position_	= VGet(0.0f, 0.0f, 0.0f);
	isClear_	= false;
	isOver_		= false;
}

void TrainingStageStatus::update()
{
	//スカイボックスを回転させる
	static float rotationY = 0.0f;
	rotationY += statusData_["skyBox_rotation_spped"].get<float>();
	if (rotationY > DX_PI_F * 2)
	{
		rotationY = 0.0f;
	}
	MV1SetRotationXYZ(modelHandle_, VGet(0.0f, rotationY, 0.0f));

	MV1SetPosition(modelHandle_, VGet(0.0f, 0.0f, 0.0f));
}

void TrainingStageStatus::clear()
{
	if (isClear_) return;

	isClear_ = true;

	StageEventHandler::instance().getStageClearEvent()->occurStartingEvent();
}

void TrainingStageStatus::over()
{
	if (isOver_) return;

	isOver_ = true;

	StageEventHandler::instance().getStageOverEvent()->occurStartingEvent();
}