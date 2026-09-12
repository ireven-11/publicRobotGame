#include"DxLib.h"
#include"Timer.h"
#include"nlohmannJson.h"
#include"Tank.h"
#include"Drone.h"
#include"CastleWallStageWave.h"
#include"PlayerEventHandler.h"
#include"LevelUpEvent.h"

const char x = 0;
const char y = 1;
const char z = 2;

CastleWallStageWave::CastleWallStageWave(const nlohmann::json& data)
	:timer_(std::make_shared<Timer>()),
	wave_max_time(data["wave_max_time"].get<int>())
{
	enemyFactory_["tank"]	= [data](const VECTOR sapwnPosition, const std::vector<VECTOR>& destinations)
		{ return std::make_shared<Tank>(sapwnPosition, VGet(0.0f, 0.0f, data["tower_init_pos_z"].get<float>()), destinations); };
	enemyFactory_["drone"]	= [data](const VECTOR sapwnPosition, const std::vector<VECTOR>& destinations)
		{ return std::make_shared<Drone>(sapwnPosition, VGet(0.0f, 0.0f, data["tower_init_pos_z"].get<float>()), destinations); };

	init();

	//イベントを登録
	PlayerEventHandler::instance().getLevelUpEvent()->addStartingEvent([this]() {timer_->startPause(); });
	PlayerEventHandler::instance().getLevelUpEvent()->addPlayingEvent([this]() {timer_->pause(); });
	PlayerEventHandler::instance().getLevelUpEvent()->addFinishingEvent([this]() {timer_->finishPause(); });
}

CastleWallStageWave::~CastleWallStageWave()
{
	timer_ = nullptr;
	enemyFactory_.clear();
}


void CastleWallStageWave::init()
{
	timer_->init();
	timer_->startCountUp(0, wave_max_time);
}

void CastleWallStageWave::update(const nlohmann::json& data, std::vector<std::shared_ptr<EnemyBase>>& enemies)
{
	timer_->update();

	spawn(data, enemies);
}

void CastleWallStageWave::spawn(const nlohmann::json& data, std::vector<std::shared_ptr<EnemyBase>>& enemies)
{
	//前回スポーンさせた時間と今回スポーンさせる時間が同じなら先の処理をしない
	if(timer_->getCurrentSeconds() == timer_->getPrevSeconds()) return;

	const auto wave1SpawnData = data["wave_1_spawn_data"];

	for (int i = 0; i < wave1SpawnData.size(); i++)
	{
		const auto currentSpawnData = wave1SpawnData[i];

		//現在時間とスポーンさせる時間が一致してないなら先の処理をしない
		if (currentSpawnData["time"].get<int>() != timer_->getCurrentSeconds()) continue;

		//スポーンさせる座標を取得
		const auto positionData		= data[currentSpawnData["position"].get<std::string>()];	//jsonファイルの座標データの文字列で間接的に取得することで共通処理にした
		VECTOR spawnPosition		= VGet(positionData[x].get<float>(), positionData[y].get<float>(), positionData[z].get<float>());

		//目的地を取得
		std::vector<VECTOR> destinations;
		const auto destinationData = data[currentSpawnData["destination"].get<std::string>()];	//jsonファイルの座標データの文字列で間接的に取得することで共通処理にした
		for (int i = 0; i < destinationData.size(); i++)
		{
			const auto movingDestination = VGet(destinationData[i][x].get<float>(), destinationData[i][y].get<float>(), destinationData[i][z].get<float>());
			destinations.emplace_back(movingDestination);
		}

		//敵をスポーンさせる数の回数ループして生成
		const auto spawnEnmeyName = currentSpawnData["enemy_name"].get<std::string>();
		for (int i = 0; i < currentSpawnData["number"].get<int>(); i++)
		{
			//スポーンしたときの座標が重ならないように雑に補正
			++spawnPosition.x;

			//敵をスポーンさせる
			enemies.emplace_back(createEnemy(spawnEnmeyName, spawnPosition , destinations));
		}
	}
}

const std::shared_ptr<EnemyBase> CastleWallStageWave::createEnemy(const std::string& enemyName, const VECTOR sapwnPosition, const std::vector<VECTOR>& movingDestinations)
{
	//敵の名前がunordered_mapに存在するか確認する
	const auto isExisting = enemyFactory_.find(enemyName);
	if(isExisting == enemyFactory_.end()) return nullptr;

	//unordered_mapに登録したfactory関数を呼び出して敵を生成する
	return enemyFactory_[enemyName](sapwnPosition, movingDestinations);
}