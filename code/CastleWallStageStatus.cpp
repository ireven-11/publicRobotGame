#include"DxLibForIreven.h"
#include"CastleWallStageStatus.h"
#include"StageEventHandler.h"
#include"StageClearEvent.h"
#include"StageOverEvent.h"

CastleWallStageStatus::CastleWallStageStatus()
{
	//マップモデルを読み込み
	statusData_				= LoadDataJson("statusData/stageData/castleWallStageStatus.json");
	std::string modelPath	= statusData_["model_path"].get<std::string>();
	modelHandle_			= MV1LoadModel(modelPath.c_str());	//dxlibの関数はstringを使えないのでc_str()で変換する
	float modelScale		= statusData_["model_scale"].get<float>();
	MV1SetScale(modelHandle_, VGet(modelScale, modelScale * expand_map_y, modelScale));

	//スカイドームを読み込み
	std::string skyBoxPath = statusData_["skybox_path"].get<std::string>();
	skyBox_ = MV1LoadModel(skyBoxPath.c_str());
	float skyBoxScale = statusData_["skybox_scale"].get<float>();
	MV1SetScale(skyBox_, VGet(skyBoxScale, skyBoxScale, skyBoxScale));

	missionText_ = Utf8ToSJIS(statusData_["mission_text"].get<std::string>());

	init();
}

CastleWallStageStatus::~CastleWallStageStatus()
{
	MV1DeleteModel(modelHandle_);
}

void CastleWallStageStatus::init()
{
	position_				= VGet(0.0f, statusData_["adust_ground_model_position_y"].get<float>(), 0.0f);
	skyBoxPosition_			= VGet(0.0f, 0.0f, 0.0f);
	isClear_				= false;
	isOver_					= false;
	lookAtStartingPosition_ = VGet(0.0f, 0.0f, 0.0f);
}

void CastleWallStageStatus::update()
{
	MV1SetPosition(modelHandle_, position_);
	MV1SetPosition(skyBox_, skyBoxPosition_);
}

void CastleWallStageStatus::clear()
{
	if (isClear_) return;

	if (isOver_) return;

	isClear_ = true;

	StageEventHandler::instance().getStageClearEvent()->occurStartingEvent();
}

void CastleWallStageStatus::over()
{
	if (isClear_) return;

	if (isOver_) return;

	isOver_ = true;

	StageEventHandler::instance().getStageOverEvent()->occurStartingEvent();
}