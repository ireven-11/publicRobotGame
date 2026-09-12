#include"DxLibForIreven.h"
#include"Timer.h"
#include"IStage.h"
#include"IMapObject.h"
#include"TrainingStageStatus.h"
#include"StageStatus.h"
#include"TrainingStage.h"
#include"Target.h"
#include"ITargetableObject.h"
#include"MapObjectStatus.h"
#include"Score.h"

TrainingStage::TrainingStage()
	:status_(std::make_shared<TrainingStageStatus>()),
	score_(std::make_shared<Score>()),
	timer_(std::make_shared<Timer>())
{
	for (auto i = 0; i < max_object_value; i++)
	{
		spawner();
	}

	status_->setLookAtStartingPosition(VGet(0.0f, 0.0f, 0.0f));
}

TrainingStage::~TrainingStage()
{
	status_ = nullptr;
	mapObjects_.clear();
}

void TrainingStage::init()
{
	status_->init();

	timer_->init();
	timer_->startCountDown(status_->getStatusData()["limit_clear_time"].get<int>(), 0);

	spawnCounter_ = 0;
}

void TrainingStage::update()
{
	timer_->update();

	status_->update();

	++spawnCounter_;
	if (spawnCounter_ % 300 == 0)
	{
		spawnCounter_ = 0;
		spawner();
	}

	//マップオブジェクト
	for (const auto& mapObject : mapObjects_)
	{
		mapObject->update();
	}

	checkClear();
}

void TrainingStage::draw()
{
	DrawGrid(ground_grid_size, ground_grid_div, GetColor(25, 25, 255));

	MV1DrawModel(status_->getModelHandle());

	for (const auto& mapObject : mapObjects_)
	{
		mapObject->draw();
	}

	//てすと
	score_->draw();
}

void TrainingStage::spawner()
{
	//てすと
	VECTOR rondomPosition = VGet(0.0f, 0.0f, 0.0f);
	rondomPosition.x = GetRand(750) - 500.0f;
	rondomPosition.z = GetRand(750) - 500.0f;

	mapObjects_.emplace_back(std::make_shared<Target>(rondomPosition));
}

void TrainingStage::checkOver()
{
	if (!timer_->hasFinishedCountDown()) return;

	status_->over();
}

void TrainingStage::checkClear()
{
	if (!timer_->hasFinishedCountDown()) return;

	status_->clear();
}

void TrainingStage::destoryObject()
{
	mapObjects_.erase(//erace-removeイディオムで死亡フラグが立っているキャラをまとめて削除
		std::remove_if(	//削除したい領域を一旦後ろに送る
			mapObjects_.begin(),
			mapObjects_.end(),
			[](const std::shared_ptr<IMapObject>& mapObjects_)	//削除したい条件をラムダ式で書く
			{
				//マップオブジェクトがターゲットになるか確認
				auto targetableObject = std::dynamic_pointer_cast<ITargetableObject>(mapObjects_->getStatus());
				if (!targetableObject) return false;

				return targetableObject->getCanDestroy();
			}
		),
		mapObjects_.end());	//後ろに送ったいらないやつの最初から最後までeraseで削除
}

void TrainingStage::drawUI()
{

}