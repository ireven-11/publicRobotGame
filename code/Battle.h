#pragma once
#include"IScene.h"
#include"SceneBase.h"
#include"SceneContext.h"
#include<vector>

class ColliderManager;
class IStage;
class Player;
class EnemyBase;
class BulletManager;
class EffectManager;
class BattleSoundManager;
class PlayerUIPresenter;
class InstructionUIPresenter;
class SupportAI;
class ExplosionManager;
class ExpManager;
class ReinforcementSystem;
class ReinforcementUIPresenter;

class Battle : public SceneBase<SceneContext>
{
public:
	Battle(std::shared_ptr<SceneContext> context);
	~Battle();

	void init();
	void update();
	void draw();
	void enter();
	void exit();

private:
	void proceed();
	void addActiveBullets();
	void stageEventRegister();
	void startResultEffect();
	void clearEffect();
	void overEffect();
	void setCommonSerifu();
	
	std::shared_ptr<Player> player_;
	std::unique_ptr<ColliderManager> collider_;
	std::unique_ptr<IStage> stage_;
	std::unique_ptr<BulletManager> bulletManager_;
	std::unique_ptr<EffectManager> effectManager_;
	std::shared_ptr<BattleSoundManager> soundManager_;
	std::unique_ptr<PlayerUIPresenter> playerUI_;
	std::unique_ptr<InstructionUIPresenter> instructionUI_;
	std::shared_ptr<SupportAI> supportAI_;
	std::shared_ptr<ExplosionManager> explosionManager_;
	std::unique_ptr<ExpManager> expManager_;
	std::shared_ptr<ReinforcementSystem> reinforcementSystem_;
	std::unique_ptr<ReinforcementUIPresenter> reinforcementUI_;
	bool isStartingClearEffect_;
	bool isStartingOverEffect_;
	int clearGraph_;
	int overGraph_;
	int font;
	int graphAlpha_;

	const float result_graph_scale	= 0.75f;
	const int font_size				= 45;
	const int text_color			= GetColor(225, 225, 225);
	const int font_thick			= 5;
	const std::string return_to_title = "Aボタンでタイトルへ";
};