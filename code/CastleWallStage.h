#pragma once

class IStage;
class IMapObject;
class Timer;
class CastleWallStageStatus;
class ITargetableObject;
class EnemyBase;
class Drone;
class Tank;
class CastleWallStageWave;
class TowerEvent;

class CastleWallStage : public IStage
{
public:
	CastleWallStage();
	~CastleWallStage();

	void init();
	void update();
	void draw();
	void addClearConditions(const std::function<bool()>& condition);
	void addOverConditions(const std::function<bool()>& condition);
	void destoryObject();
	void drawUI();

	const std::shared_ptr<StageStatus> getStatus()const noexcept;
	const std::vector<std::shared_ptr<IMapObject>> getMapObjects()const noexcept { return mapObjects_; }
	const std::shared_ptr<Timer> getTimer()const noexcept { return timer_; }
	const std::vector<std::shared_ptr<EnemyBase>> getEnemies()const noexcept { return enemies_; }
	const std::shared_ptr<TowerEvent> getTowerEvent()const noexcept { return towerEvent_; }
	const VECTOR getTowerPosition()const noexcept { return towerPosition_; }
	const float getOverExplosionScale()const noexcept { return over_explosion_scale; }

private:
	template<class T>
	void eraceAndRemoveFromVector(std::vector<std::shared_ptr<T>>& vector)
	{
		//破壊
		vector.erase(//erace-removeイディオムで死亡フラグが立っているキャラをまとめて削除
			std::remove_if(	//削除したい領域を一旦後ろに送る
				vector.begin(),
				vector.end(),
				[](const std::shared_ptr<T>& vector)	//削除したい条件をラムダ式で書く
				{
					//ターゲットになるか確認
					auto targetableObject = std::dynamic_pointer_cast<ITargetableObject>(vector->getStatus());
					if (!targetableObject) return false;

					return targetableObject->getCanDestroy();
				}
			),
			vector.end());	//後ろに送ったいらないやつの最初から最後までeraseで削除
	}
	void checkOver();
	void initAllTimer();
	void checkClear();
	void compulsoryDestoryEnemies();

	VECTOR towerPosition_;
	int clock;
	int font;
	int font2;
	std::shared_ptr<CastleWallStageStatus> status_;
	std::vector<std::shared_ptr<IMapObject>> mapObjects_;
	std::shared_ptr<Timer> timer_;
	std::vector<std::shared_ptr<EnemyBase>> enemies_;
	std::shared_ptr<CastleWallStageWave> wave_;
	std::vector<std::function<bool()>> clearConditions_;	//ステージ以外のクリア条件を追加するための関数ポインタの配列
	std::vector<std::function<bool()>> overConditions_;		//ステージ以外のゲームオーバー条件を追加するための関数ポインタの配列
	std::shared_ptr<TowerEvent> towerEvent_;

	const float destroying_damage		= 1000000.0f;
	const int font_size					= 30;
	const int font_thick				= 5;
	const int text_color				= GetColor(230, 230, 230);
	const VECTOR text_position			= VGet(60.0f, 320.0f, 0.0f);
	const float text_offset				= 40.0f;
	const VECTOR box_position			= VGet(50.0f, 300.0f, 0.0f);
	const float box_widht				= 300.0f;
	const float box_height				= 100.0f;
	const int box_color					= GetColor(50, 50, 50);
	const int mission_box_color			= GetColor(200, 200, 200);
	const VECTOR mission_box_position	= VGet(65.0f, 250.0f, 0.0f);
	const float mission_box_widht		= 175.0f;
	const float mission_box_height		= 60.0f;
	const int mission_text_color		= GetColor(25, 25, 25);
	const VECTOR clock_position			= VGet(200.0f, 125.0f, 0.0f);
	const double clock_scale			= 0.4;
	const float adjust_clock_seconds	= 5.0f;
	const float over_explosion_scale	= 150.0f;
};