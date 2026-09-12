#include"DxLibForIreven.h"
#include"InputHandler.h"
#include"InstructionUIPresenter.h"
#include"Player.h"
#include"ColliderManager.h"
#include"IStage.h"
#include"BulletManager.h"
#include"EffectManager.h"
#include"ExpManager.h"
#include"BattleSoundManager.h"
#include"PlayerUIPresenter.h"
#include"SupportAI.h"
#include"ExplosionManager.h"
#include"ReinforcementSystem.h"
#include"ReinforcementUIPresenter.h"
#include"Battle.h"
#include"IWeapon.h"
#include"IBullet.h"
#include"BulletStatus.h"
#include"ITargetableObject.h"
#include"TrainingStage.h"
#include"ColliderEventHander.h"
#include"EnemyBase.h"
#include"DInput.h"
#include"Game.h"
#include"SupportAIUIModel.h"
#include"StageEventHandler.h"
#include"StageClearEvent.h"
#include"StageOverEvent.h"
#include"TowerEvent.h"
#include"ExplosionEffect.h"
#include"Explosion.h"
#include"CastleWallStage.h"
#include"StageStatus.h"
#include"PlayerEventHandler.h"
#include"LevelUpEvent.h"

Battle::Battle(std::shared_ptr<SceneContext> context)
	: SceneBase(context),
	collider_(std::make_unique<ColliderManager>()),
	bulletManager_(std::make_unique<BulletManager>()),
	effectManager_(std::make_unique<EffectManager>()),
	soundManager_(std::make_shared<BattleSoundManager>()),
	playerUI_(std::make_unique<PlayerUIPresenter>()),
	instructionUI_(std::make_unique<InstructionUIPresenter>()),
	supportAI_(std::make_unique<SupportAI>()),
	expManager_(std::make_unique<ExpManager>()),
	reinforcementUI_(std::make_unique<ReinforcementUIPresenter>())
{
	explosionManager_ = std::make_shared<ExplosionManager>(std::dynamic_pointer_cast<ExplosionEffect>(effectManager_->getEffects()["explosion"]),
		[this]() {soundManager_->start2DSound("explosion", false, true); });

	clearGraph_ = LoadGraph("graph/missionClear.png");
	overGraph_	= LoadGraph("graph/missionFailed.png");

	font = CreateFontToHandle("マキナス 4 Flat", font_size, font_thick);

	player_ = context->getPlayer();

	reinforcementSystem_ = std::make_shared<ReinforcementSystem>(std::dynamic_pointer_cast<PlayerStatus>(player_->getStatus()));

	stage_ = std::make_unique<CastleWallStage>();	//test

	//ゲームオーバー条件を追加
	stage_->addOverConditions([this]{return player_->getStatus()->getHp() <= 0;});

	//どのバトルでも共通のセリフのサウンドをサポートAIに渡す
	setCommonSerifu();

	init();

	//イベントを登録
	PlayerEventHandler::instance().getLevelUpEvent()->addStartingEvent([this]() {supportAI_->startSendingMessage(MessageType::LEVEL_UP); });
	PlayerEventHandler::instance().getLevelUpEvent()->addStartingEvent([this]() {soundManager_->start2DSound("powerUp", false, true); });
}

Battle::~Battle()
{
	player_				= nullptr;
	collider_			= nullptr;
	stage_				= nullptr;
	bulletManager_		= nullptr;
	effectManager_		= nullptr;
	soundManager_		= nullptr;
	playerUI_			= nullptr;
	instructionUI_		= nullptr;
	explosionManager_	= nullptr;
}

void Battle::stageEventRegister()
{
	const auto castleWallStage = dynamic_cast<CastleWallStage*>(stage_.get());

	//城壁ステージだったら
	if (castleWallStage)
	{
		//ステージの固有のイベントを設定
		StageEventHandler::instance().getStageClearEvent()->clearEvent();
		StageEventHandler::instance().getStageClearEvent()->addStartingEvent([this]() {supportAI_->startSendingMessage(MessageType::SUCSSES_DEFECNCE); });
		StageEventHandler::instance().getStageClearEvent()->addStartingEvent([this]() {soundManager_->start2DSound("startMessage"); });
		StageEventHandler::instance().getStageOverEvent()->clearEvent();
		StageEventHandler::instance().getStageOverEvent()->addStartingEvent([this]() {soundManager_->start2DSound("startMessage"); });
		StageEventHandler::instance().getStageOverEvent()->addStartingEvent([this, castleWallStage]()
			{
				if (player_->getStatus()->getHp() <= 0)
				{
					supportAI_->startSendingMessage(MessageType::DESTROYED_MY_MACHINE);
				}
				else
				{
					supportAI_->startSendingMessage(MessageType::INSTRUCT_DESTROY_TARGET);
					effectManager_->getEffects()["explosion"]->setScale(castleWallStage->getOverExplosionScale());
					effectManager_->getEffects()["explosion"]->play();
					effectManager_->getEffects()["explosion"]->setPosition(castleWallStage->getTowerPosition());

					context()->getCameraBrain()->startEffectCamera();
				}
			});

		//タワーのイベントを設定
		castleWallStage->getTowerEvent()->addHpThreeQuartersEvent([this]() {soundManager_->start2DSound("startMessage"); });
		castleWallStage->getTowerEvent()->addHpThreeQuartersEvent([this]() {supportAI_->startSendingMessage(MessageType::TOWER_HP_THREE_QUARTERS); });
		castleWallStage->getTowerEvent()->addHpOneHalfEvent([this]() {soundManager_->start2DSound("startMessage"); });
		castleWallStage->getTowerEvent()->addHpOneHalfEvent([this]() {supportAI_->startSendingMessage(MessageType::TOWER_HP_ONE_HALF); });
		castleWallStage->getTowerEvent()->addHpOneQuartersEvent([this]() {soundManager_->start2DSound("startMessage"); });
		castleWallStage->getTowerEvent()->addHpOneQuartersEvent([this]() {supportAI_->startSendingMessage(MessageType::TOWER_HP_ONE_QUARTERS); });

		//ステージ固有のセリフをサポートAIに渡す
		supportAI_->setMessageSound(MessageType::INSTRUCT_DESTROY_TARGET, [this]() {soundManager_->start2DSound("destroyDefencingTarget"); });
		supportAI_->setMessageSound(MessageType::SUCSSES_DEFECNCE, [this]() {soundManager_->start2DSound("successDefencing"); });
		supportAI_->setMessageSound(MessageType::WARN_MARCHING, [this]() {soundManager_->start2DSound("warnMarching"); });
		supportAI_->setMessageSound(MessageType::TOWER_HP_THREE_QUARTERS, [this]() {soundManager_->start2DSound("defencingTargetHpThreeQuarters"); });
		supportAI_->setMessageSound(MessageType::TOWER_HP_ONE_HALF, [this]() {soundManager_->start2DSound("defencingTargetHpOneHalf"); });
		supportAI_->setMessageSound(MessageType::TOWER_HP_ONE_QUARTERS, [this]() {soundManager_->start2DSound("defencingTargetHpOneQuarters"); });
	}
}

void Battle::init()
{
	currentState_			= SceneState::NORMAL;
	isStartingClearEffect_	= false;
	isStartingOverEffect_	= false;
	graphAlpha_				= 0;
}

void Battle::update()
{
	const auto playerStatus = std::dynamic_pointer_cast<PlayerStatus>(player_->getStatus());
	const auto mapObjects	= stage_->getMapObjects();
	const auto enemies		= stage_->getEnemies();
	const auto camera		= context()->getCameraBrain();
	const auto stageStatus	= stage_->getStatus();

	startResultEffect();

	//開始時のカメラ演出が終わったらメッセージを出すようにする
	if (camera->getFinishedEffectCameraMoment() && !stage_->getStatus()->getIsClear() && !stage_->getStatus()->getIsOver())
	{
		//カメラ演出が終わったことを検知
		supportAI_->startSendingMessage(MessageType::INTRODUCTION);
		currentState_ = SceneState::NORMAL;
		soundManager_->start2DSound("startMessage");
	}
	supportAI_->update();

	//強化システム（強化中は先の処理をしない）
	reinforcementSystem_->update(soundManager_);
	if (reinforcementSystem_->getIsActive()) return;

	//開始演出が終わってなければは演出以外の処理をしない
	if (camera->getFinishedEffectCamera())
	{
		//当たり判定
		collider_->update(player_, enemies, mapObjects, bulletManager_->getActiveBullets(), explosionManager_);

		//ステージ
		stage_->update();

		//プレイヤー
		player_->update();

		//弾丸
		bulletManager_->init();	//managerにはactiveなbulletしか入れないので毎フレーム初期化してから弾丸を追加する
		addActiveBullets();
		bulletManager_->update();

		//ui
		playerUI_->update();
	}

	//エフェクト
	effectManager_->setEffectPosition(playerStatus, enemies);
	effectManager_->setEffectRotationAngle(playerStatus);
	effectManager_->update();

	//爆発マネージャー
	explosionManager_->update();
	explosionManager_->init();	//アクティブな爆発を更新が終わったタイミングで初期化する

	//カメラ
	camera->update(playerStatus, stageStatus);

	//経験値
	if (!stage_->getStatus()->getIsClear() && !stage_->getStatus()->getIsOver())
	{
		expManager_->update(playerStatus, enemies);
	}

	//オブジェクトを破壊
	stage_->destoryObject();	//updateの中で破壊するオブジェクトの値を参照したい可能性があるので最後に呼ぶ
}

void Battle::draw()
{
	stage_->draw();

	player_->draw();

	bulletManager_->draw();

	effectManager_->draw();

	expManager_->draw();

	context()->getCameraBrain()->postEffect();

	//通常状態の時だけ
	if (currentState_ == SceneState::NORMAL)
	{
		playerUI_->draw(std::dynamic_pointer_cast<PlayerStatus>(player_->getStatus()));
		stage_->drawUI();
		reinforcementUI_->draw(reinforcementSystem_);
	}

	//バトル開始カメラ演出中だけ
	if (!context()->getCameraBrain()->getFinishedEffectCamera())
	{
		instructionUI_->draw(stage_->getStatus());
	}

	//リザルト演出
	if (isStartingClearEffect_)
	{
		clearEffect();

		const auto drawingPosition	= GetCenteredTextPosition(VGet(init_screen_width * 0.5f, init_screen_height * 0.8f, 0.0f), return_to_title, font);
		DrawStringFToHandle(drawingPosition.x, UpDownPositionY(drawingPosition.y), return_to_title.c_str(), text_color, font);
	}
	else if (isStartingOverEffect_)
	{
		overEffect();

		const auto drawingPosition	= GetCenteredTextPosition(VGet(init_screen_width * 0.5f, init_screen_height * 0.8f, 0.0f), return_to_title, font);
		DrawStringFToHandle(drawingPosition.x, UpDownPositionY(drawingPosition.y), return_to_title.c_str(), text_color, font);
	}

	supportAI_->draw();
}

void Battle::proceed()
{
	if (!isStartingClearEffect_ && !isStartingOverEffect_) return;

	if (!InputHandler::instance().getDInput()->getButtonReleasedMoment(PROCON::BUTTON::A)) return;

	soundManager_->start2DSound("decide");
	PlayMovie("movie/transition.mp4", 1.0, DX_MOVIEPLAYTYPE_NORMAL);

	Game::instance().proceedToTitle();
}

void Battle::enter()
{
	init();

	soundManager_->init();

	effectManager_->stopEffects();
	effectManager_->init();

	stageEventRegister();	//stageを参照して、イベントを登録してるのでstageのinitはこの後にやってください

	stage_->init();

	const auto player = context()->getPlayer();
	player->init();

	reinforcementSystem_->init();

	context()->getCameraBrain()->init(std::dynamic_pointer_cast<PlayerStatus>(player->getStatus()));
	context()->getCameraBrain()->startEffectCamera();

	soundManager_->start2DSound("bgm", true);

	supportAI_->init();

	currentState_ = SceneState::EFFECT;
}

void Battle::exit()
{
	soundManager_->stopAllSound();

	expManager_->init();
}

void Battle::addActiveBullets()
{
	const auto playerStatus = std::dynamic_pointer_cast<PlayerStatus>(player_->getStatus());
	const auto enemies		= stage_->getEnemies();

	std::vector<std::shared_ptr<IBullet>> enemyBullets;
	const std::vector<std::shared_ptr<IBullet>> rightWeaponBullets	= playerStatus->getRightWeapon()->getBullets();
	const std::vector<std::shared_ptr<IBullet>> leftWeaponBullets	= playerStatus->getLeftWeapon()->getBullets();
	for (const auto& enemy : enemies)
	{
		for (const auto& enemyBullet : enemy->getBullets())
		{
			enemyBullets.emplace_back(enemyBullet);
		}
	}
	for (const auto& rightWeaponBullet : rightWeaponBullets)
	{
		if (!rightWeaponBullet->getStatus()->getIsUsing()) continue;

		bulletManager_->addActiveBullet(rightWeaponBullet);
	}
	for (const auto& leftWeaponBullet : leftWeaponBullets)
	{
		if (!leftWeaponBullet->getStatus()->getIsUsing()) continue;

		bulletManager_->addActiveBullet(leftWeaponBullet);
	}
	for (const auto& enemyBullet : enemyBullets)
	{
		if (!enemyBullet->getStatus()->getIsUsing()) continue;

		bulletManager_->addActiveBullet(enemyBullet);
	}
}

void Battle::startResultEffect()
{
	if (isStartingClearEffect_ || isStartingOverEffect_) return;

	if (supportAI_->getIsFinishedLastmessage())
	{
		if (stage_->getStatus()->getIsClear())
		{
			isStartingClearEffect_ = true;
			soundManager_->start2DSound("clear", true);
		}
		else if (stage_->getStatus()->getIsOver())
		{
			isStartingOverEffect_ = true;
			soundManager_->start2DSound("over", true);
		}

		soundManager_->stop2DSound("bgm");
		currentState_ = SceneState::EFFECT;
	}
}

void Battle::clearEffect()
{
	++graphAlpha_;
	if (graphAlpha_ >= UCHAR_MAX)
	{
		graphAlpha_ = UCHAR_MAX;
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, graphAlpha_);
	DrawRotaGraphF(init_screen_width * 0.5f, init_screen_height * 0.5f, result_graph_scale, 0.0, clearGraph_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Battle::overEffect()
{
	++graphAlpha_;
	if (graphAlpha_ >= UCHAR_MAX)
	{
		graphAlpha_ = UCHAR_MAX;
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, graphAlpha_);
	DrawRotaGraphF(init_screen_width * 0.5f, init_screen_height * 0.5f, result_graph_scale, 0.0, overGraph_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Battle::setCommonSerifu()
{
	supportAI_->setMessageSound(MessageType::DESTROYED_MY_MACHINE, [this]() {soundManager_->start2DSound("destroyMyMachine"); });
	supportAI_->setMessageSound(MessageType::GO_HOME, [this]() {soundManager_->start2DSound("goHome"); });
	supportAI_->setMessageSound(MessageType::INTRODUCTION, [this]() {soundManager_->start2DSound("introduction"); });
	supportAI_->setMessageSound(MessageType::OTUKARESAMADESITA, [this]() {soundManager_->start2DSound("otukaresamadesita"); });
	supportAI_->setMessageSound(MessageType::LEVEL_UP, [this]() {soundManager_->start2DSound("levelUp"); });
}