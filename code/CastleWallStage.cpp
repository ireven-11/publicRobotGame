#include"DxLibForIreven.h"
#include"Timer.h"
#include"IStage.h"
#include"CastleWallStageStatus.h"
#include"CastleWallStageWave.h"
#include"StageStatus.h"
#include"ITargetableObject.h"
#include"EnemyBase.h"
#include"Tank.h"
#include"Drone.h"
#include"TowerEvent.h"
#include"CastleWallStage.h"
#include"OutOfAreaBox.h"
#include"MapObjectStatus.h"
#include"Tower.h"
#include"HousingComplex.h"
#include"TowerStatus.h"
#include"PlayerEventHandler.h"
#include"LevelUpEvent.h"

CastleWallStage::CastleWallStage()
	:status_(std::make_shared<CastleWallStageStatus>()),
	timer_(std::make_shared<Timer>()),
	towerEvent_(std::make_shared<TowerEvent>())
{
	const auto statusData = status_->getStatusData();

	wave_ = std::make_shared<CastleWallStageWave>(statusData);

	const char positionX = 0;
	const char positionY = 1;
	const char positionZ = 2;

	//団地を生成
	const auto housingComplexData = statusData["housing_complex_data"];
	for (int i = 0; i < housingComplexData.size(); i++)
	{
		const auto		dataI = housingComplexData[i];
		const auto		positionDataI = dataI["init_position"];
		const VECTOR	position = VGet(positionDataI[positionX].get<float>(),
			positionDataI[positionY].get<float>(), positionDataI[positionZ].get<float>());

		mapObjects_.emplace_back(std::make_shared<HousingComplex>(position, dataI["rotation_angle_y"]));
	}

	//マップの範囲外を表すボックスを生成
	for (int i = 0; i < statusData["out_of_area_box_position"].size(); i++)
	{
		const VECTOR initPosition = VGet(statusData["out_of_area_box_position"][i][positionX].get<float>(),
			statusData["out_of_area_box_position"][i][positionY].get<float>(), statusData["out_of_area_box_position"][i][positionZ].get<float>());

		const VECTOR standardSize = VGet(statusData["out_of_area_box_size"][i][positionX].get<float>(),
			statusData["out_of_area_box_size"][i][positionY].get<float>(), statusData["out_of_area_box_size"][i][positionZ].get<float>());

		mapObjects_.emplace_back(std::make_shared<OutOfAreaBox>(initPosition, statusData["model_scale"].get<float>(), standardSize));
	}

	font	= CreateFontToHandle("マキナス 4 Flat", font_size, font_thick, DX_FONTTYPE_ANTIALIASING_16X16);
	font2	= CreateFontToHandle("マキナス 4 Flat", font_size * 2, font_thick, DX_FONTTYPE_ANTIALIASING_16X16);
	clock	= LoadGraph("graph/clock.png");

	//イベントを登録
	PlayerEventHandler::instance().getLevelUpEvent()->addStartingEvent([this]() {timer_->startPause(); });
	PlayerEventHandler::instance().getLevelUpEvent()->addPlayingEvent([this]() {timer_->pause(); });
	PlayerEventHandler::instance().getLevelUpEvent()->addFinishingEvent([this]() {timer_->finishPause(); });
}

CastleWallStage::~CastleWallStage()
{
	status_		= nullptr;
	wave_		= nullptr;
	timer_		= nullptr;
	mapObjects_.clear();
	enemies_.clear();
	clearConditions_.clear();
	overConditions_.clear();
	DeleteGraph(clock);
}

void CastleWallStage::init()
{
	status_->init();

	wave_->init();

	initAllTimer();

	enemies_.clear();

	//タワーがなければ生成
	const VECTOR towerSpawnPosition = VGet(status_->getStatusData()["tower_init_pos_x"].get<float>(), 0.0f, status_->getStatusData()["tower_init_pos_z"].get<float>());
	if (mapObjects_[0] != std::dynamic_pointer_cast<Tower>(mapObjects_[0]))
	{
		//タワーを生成(必ずマップオブジェクトの最初になるようにする)
		mapObjects_.emplace(mapObjects_.begin(), 
			std::make_shared<Tower>(towerSpawnPosition, [this]() {towerEvent_->occurHpThreeQuartersEvent(); },
				[this]() {towerEvent_->occurHpOneHalfEvent(); }, [this]() {towerEvent_->occurHpOneQuartersEvent(); }));

		towerPosition_ = towerSpawnPosition;
	}
	status_->setLookAtStartingPosition(towerSpawnPosition);

	for (const auto& mapObject : mapObjects_)
	{
		mapObject->getStatus()->init();
	}
}

void CastleWallStage::update()
{
	//ゲームがリザルトでなければタイマーの処理をする
	if (!status_->getIsClear() && !status_->getIsOver())
	{
		timer_->update();
	}

	status_->update();
	
	wave_->update(status_->getStatusData(), enemies_);

	checkOver();
	checkClear();

	for (const auto& enemy : enemies_)
	{
		enemy->update();
	}

	for (const auto& mapObject : mapObjects_)
	{
		mapObject->update();
	}
}

void CastleWallStage::draw()
{
	MV1DrawModel(status_->getSkyBoxHandle());
	MV1DrawModel(status_->getModelHandle());

	for (const auto& enemy : enemies_)
	{
		enemy->draw();
	}

	for (const auto& mapObject : mapObjects_)
	{
		mapObject->draw();
	}
}

const std::shared_ptr<StageStatus> CastleWallStage::getStatus()const noexcept
{
	return std::dynamic_pointer_cast<StageStatus>(status_);
}

void CastleWallStage::checkOver()
{
	//条件のどれかが満たされているとゲームオーバーになる
	for(const auto& condition : overConditions_)
	{
		if (condition())
		{
			status_->over();
			return;
		}
	}

	const auto tower = std::dynamic_pointer_cast<ITargetableObject>(mapObjects_[0]->getStatus());	//towerがindexの0にいるのはinitで確約するようにする

	//念のためチェック
	if (!tower) return;

	//タワーの破壊条件を満たしていなければゲームオーバーにならない
	if (!tower->getCanDestroy()) return;

	status_->over();
}

void CastleWallStage::initAllTimer()
{
	timer_->init();
	timer_->startCountDown(status_->getStatusData()["limit_clear_time"].get<float>(), 0);
}

void CastleWallStage::checkClear()
{
	//タイマーが終了していなければクリア条件を満たしていてもクリアできない
	if (!timer_->hasFinishedCountDown()) return;

	//条件がすべて満たされていればクリア可能
	for (const auto& condition : clearConditions_)
	{
		if (!condition) return;
	}

	status_->clear();

	//敵を強制的に破壊する
	compulsoryDestoryEnemies();
}

void CastleWallStage::addClearConditions(const std::function<bool()>& condition)
{
	clearConditions_.emplace_back(condition);
}

void CastleWallStage::addOverConditions(const std::function<bool()>& condition)
{
	overConditions_.emplace_back(condition);
}

void CastleWallStage::destoryObject()
{
	//破壊
	eraceAndRemoveFromVector<IMapObject>(mapObjects_);
	eraceAndRemoveFromVector<EnemyBase>(enemies_);
}

void CastleWallStage::drawUI()
{
	const auto tower = std::dynamic_pointer_cast<TowerStatus>(mapObjects_[0]->getStatus());	//towerがindexの0にいるのはinitで確約するようにする

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, UCHAR_MAX * 0.5f);
	DrawBox(box_position.x, box_position.y, box_position.x + box_widht, box_position.y + box_height, box_color, true);
	DrawBox(mission_box_position.x, mission_box_position.y, mission_box_position.x + mission_box_widht, mission_box_position.y + mission_box_height, mission_box_color, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawStringFToHandle(text_position.x, text_position.y, status_->getMissionText().c_str(), text_color, font);
	const auto mission = "MISSION";
	const auto missionPosition = GetCenteredTextPosition(VGet(mission_box_position.x + mission_box_widht * 0.5f, mission_box_position.y + mission_box_height * 0.5f, 0.0f),
		mission, font);
	DrawStringFToHandle(missionPosition.x, missionPosition.y, mission, mission_text_color, font);

	if (tower && tower->getHp() > 0)
	{
		DrawFormatStringToHandle(text_position.x, text_position.y + text_offset, text_color, font, "タワーHP：%.0f", tower->getHp());
	}
	else
	{
		DrawStringFToHandle(text_position.x, text_position.y + text_offset, "タワーHP：0", text_color, font);
	}

	DrawRotaGraph(clock_position.x, clock_position.y, clock_scale, 0.0, clock, true);
	if (timer_->getCurrentSeconds() <= 0)
	{
		DrawFormatStringToHandle(clock_position.x - font_size - adjust_clock_seconds, clock_position.y - adjust_clock_seconds,
			GetColor(200, 25, 25), font2, "0");
	}
	else
	{
		DrawFormatStringToHandle(clock_position.x - font_size - adjust_clock_seconds, clock_position.y - adjust_clock_seconds,
			GetColor(200, 25, 25), font2, "%d", timer_->getCurrentSeconds());
	}
}

void CastleWallStage::compulsoryDestoryEnemies()
{
	//敵にダメージを与えて、強制的に破壊する
	for (const auto& enemy : enemies_)
	{
		const auto target = std::dynamic_pointer_cast<ITargetableObject>(enemy->getStatus());
		target->decreaseHp(destroying_damage);
		enemy->getEnemyStatus()->destroy();
	}
}