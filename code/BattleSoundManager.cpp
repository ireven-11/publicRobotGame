#include"DxLib.h"
#include"BattleSoundManager.h"
#include"InputHandler.h"
#include"DashEvent.h"
#include"BulletEventHandler.h"
#include"RightFireBulletEvent.h"
#include"LeftFireBulletEvent.h"
#include"WalkEvent.h"
#include"RaiseEvent.h"
#include"DescentEvent.h"
#include"ColliderEventHander.h"
#include"OnDamageEvent.h"
#include"DestroyObjectEvent.h"

BattleSoundManager::BattleSoundManager()
{
	soundData_ = LoadDataJson("statusData/soundData/battleSoundStatus.json");

	const auto sound2d		= soundData_["2d_sound"];
	const auto playerSound	= sound2d["player_sound"];
	const auto serifuSound	= sound2d["serifu"];

	sound2DHandles_["dash"]				= LoadSoundMem(playerSound["start_dash_se"]["path"].get<std::string>().c_str());
	sound2DHandles_["fireNormalBullet"]	= LoadSoundMem(playerSound["fire_normal_bullet_se"]["path"].get<std::string>().c_str());
	sound2DHandles_["move"]				= LoadSoundMem(playerSound["move_se"]["path"].get<std::string>().c_str());
	sound2DHandles_["raise"]			= LoadSoundMem(playerSound["raise_se"]["path"].get<std::string>().c_str());
	sound2DHandles_["descent"]			= LoadSoundMem(playerSound["descent_se"]["path"].get<std::string>().c_str());
	sound2DHandles_["hitBullet"]		= LoadSoundMem(playerSound["hit_bullet_se"]["path"].get<std::string>().c_str());

	sound2DHandles_["bgm"]					= LoadSoundMem(sound2d["bgm_path"].get<std::string>().c_str());
	sound2DHandles_["clear"]				= LoadSoundMem(sound2d["clear_jingle_path"].get<std::string>().c_str());
	sound2DHandles_["over"]					= LoadSoundMem(sound2d["over_jingle_paht"].get<std::string>().c_str());
	sound2DHandles_["decide"]				= LoadSoundMem(sound2d["decide_se_path"].get<std::string>().c_str());
	sound2DHandles_["startMessage"]			= LoadSoundMem(sound2d["start_message_se_path"].get<std::string>().c_str());
	sound2DHandles_["explosion"]			= LoadSoundMem(sound2d["explosion_se_path"].get<std::string>().c_str());
	sound2DHandles_["powerUp"]				= LoadSoundMem(sound2d["power_up_se_path"].get<std::string>().c_str());
	sound2DHandles_["selectReinforcement"]	= LoadSoundMem(sound2d["select_reinforcement_se_path"].get<std::string>().c_str());	//仮でタイトルで使ってると音同じやつを使ってます
	sound2DHandles_["decideReinforcement"]	= LoadSoundMem(sound2d["decide_reinforcement_se_path"].get<std::string>().c_str());	//仮でタイトルで使ってると音同じやつを使ってます

	SetCreateSoundDataType(DX_SOUNDDATATYPE_FILE);

	sound2DHandles_["destroyDefencingTarget"]			= LoadSoundMem(serifuSound["destroy_defencing_target_path"].get<std::string>().c_str());
	sound2DHandles_["destroyMyMachine"]					= LoadSoundMem(serifuSound["destroy_my_machine_path"].get<std::string>().c_str());
	sound2DHandles_["goHome"]							= LoadSoundMem(serifuSound["go_home_path"].get<std::string>().c_str());
	sound2DHandles_["introduction"]						= LoadSoundMem(serifuSound["introduction_path"].get<std::string>().c_str());
	sound2DHandles_["otukaresamadesita"]				= LoadSoundMem(serifuSound["otukaresamadesita_path"].get<std::string>().c_str());
	sound2DHandles_["successDefencing"]					= LoadSoundMem(serifuSound["success_defencing_path"].get<std::string>().c_str());
	sound2DHandles_["warnMarching"]						= LoadSoundMem(serifuSound["warn_marching_path"].get<std::string>().c_str());
	sound2DHandles_["defencingTargetHpThreeQuarters"]	= LoadSoundMem(serifuSound["defencing_target_hp_three_quarters_path"].get<std::string>().c_str());
	sound2DHandles_["defencingTargetHpOneHalf"]			= LoadSoundMem(serifuSound["defencing_target_hp_one_half_path"].get<std::string>().c_str());
	sound2DHandles_["defencingTargetHpOneQuarters"]		= LoadSoundMem(serifuSound["defencing_target_hp_one_quarters_path"].get<std::string>().c_str());
	sound2DHandles_["levelUp"]							= LoadSoundMem(serifuSound["level_up_path"].get<std::string>().c_str());

	SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS);

	adjustVolume();

	//サウンドの処理をイベントに登録
	eventRegister();
}

BattleSoundManager::~BattleSoundManager()
{
	InitSoundMem();
	sound2DHandles_.clear();
}

void BattleSoundManager::adjustVolume()
{
	const auto sound2d		= soundData_["2d_sound"];
	const auto playerSound	= sound2d["player_sound"];
	const auto serifuSound	= sound2d["serifu"];

	ChangeVolumeSoundMem(playerSound["start_dash_se"]["volume"].get<short>(), sound2DHandles_["dash"]);
	ChangeVolumeSoundMem(playerSound["fire_normal_bullet_se"]["volume"].get<short>(), sound2DHandles_["fireNormalBullet"]);
	ChangeVolumeSoundMem(playerSound["move_se"]["volume"].get<short>(), sound2DHandles_["move"]);
	ChangeVolumeSoundMem(playerSound["raise_se"]["volume"].get<short>(), sound2DHandles_["raise"]);
	ChangeVolumeSoundMem(playerSound["descent_se"]["volume"].get<short>(), sound2DHandles_["descent"]);
	ChangeVolumeSoundMem(playerSound["hit_bullet_se"]["volume"].get<short>(), sound2DHandles_["hitBullet"]);

	ChangeVolumeSoundMem(sound2d["bgm_volume"].get<short>(), sound2DHandles_["bgm"]);
	ChangeVolumeSoundMem(sound2d["clear_jingle_volume"].get<short>(), sound2DHandles_["clear"]);
	ChangeVolumeSoundMem(sound2d["over_jingle_volume"].get<short>(), sound2DHandles_["over"]);
	ChangeVolumeSoundMem(sound2d["decide_se_volume"].get<short>(), sound2DHandles_["decide"]);
	ChangeVolumeSoundMem(sound2d["start_message_se_volume"].get<short>(), sound2DHandles_["startMessage"]);
	ChangeVolumeSoundMem(sound2d["explosion_se_volume"].get<short>(), sound2DHandles_["explosion"]);
	ChangeVolumeSoundMem(sound2d["power_up_se_volume"].get<short>(), sound2DHandles_["powerUp"]);
	ChangeVolumeSoundMem(sound2d["select_reinforcement_se_volume"].get<short>(), sound2DHandles_["selectReinforcement"]);
	ChangeVolumeSoundMem(sound2d["decide_reinforcement_se_volume"].get<short>(), sound2DHandles_["decideReinforcement"]);
	
	ChangeVolumeSoundMem(serifuSound["volume"].get<short>(), sound2DHandles_["destroyDefencingTarget"]);
	ChangeVolumeSoundMem(serifuSound["volume"].get<short>(), sound2DHandles_["destroyMyMachine"]);
	ChangeVolumeSoundMem(serifuSound["volume"].get<short>(), sound2DHandles_["goHome"]);
	ChangeVolumeSoundMem(serifuSound["volume"].get<short>(), sound2DHandles_["introduction"]);
	ChangeVolumeSoundMem(serifuSound["volume"].get<short>(), sound2DHandles_["otukaresamadesita"]);
	ChangeVolumeSoundMem(serifuSound["volume"].get<short>(), sound2DHandles_["successDefencing"]);
	ChangeVolumeSoundMem(serifuSound["volume"].get<short>(), sound2DHandles_["warnMarching"]);
}

void BattleSoundManager::eventRegister()
{
	InputHandler::instance().getDashEvent()->addStartingEvent([this] {start2DSound("dash", false , true); });
	BulletEventHandler::instance().getRightFireBulletEvent()->addStartingEvent([this] {start2DSound("fireNormalBullet", false, true); });
	BulletEventHandler::instance().getLeftFireBulletEvent()->addStartingEvent([this] {start2DSound("fireNormalBullet", false, true); });
	InputHandler::instance().getWalkEvent()->addStartingEvent([this] {start2DSound("move", true); });
	InputHandler::instance().getWalkEvent()->addFinishingEvent([this] {stop2DSound("move"); });
	InputHandler::instance().getRaiseEvent()->addStartingEvent([this] {start2DSound("raise", true); });
	InputHandler::instance().getRaiseEvent()->addFinishingEvent([this] {stop2DSound("raise"); });
	InputHandler::instance().getDescentEvent()->addStartingEvent([this] {start2DSound("descent", true); });
	InputHandler::instance().getDescentEvent()->addFinishingEvent([this] {stop2DSound("descent"); });
	ColliderEventHander::instance().getOnDamageEvent()->addStartingEvent([this] {start2DSound("hitBullet", false, true); });
	ColliderEventHander::instance().getDestroyObjectEvent()->addStartingEvent([this]() {start2DSound("explosion", false, true); });
}

void BattleSoundManager::init()
{
	stopAllSound();
}

void BattleSoundManager::start2DSound(const std::string& soundName, const bool isLoop, const bool canAllowDuplicate)
{
	//検索した名前がなければ処理をしない
	const auto isExisting = sound2DHandles_.find(soundName);
	if (isExisting == sound2DHandles_.end()) return;

	const auto soundHandle = sound2DHandles_[soundName];

	//重複を許さない場合は音が流れてるかチェックして、流れてたら処理をしない
	if (CheckSoundMem(soundHandle) && !canAllowDuplicate) return;

	if (isLoop)
	{
		PlaySoundMem(soundHandle, DX_PLAYTYPE_LOOP);
	}
	else
	{
		PlaySoundMem(soundHandle, DX_PLAYTYPE_BACK);
	}
}

void BattleSoundManager::stop2DSound(const std::string& soundName)
{
	//検索した名前がなければ処理をしない
	const auto isExisting = sound2DHandles_.find(soundName);
	if (isExisting == sound2DHandles_.end()) return;

	StopSoundMem(sound2DHandles_[soundName]);
}

void BattleSoundManager::stopAllSound()
{
	for (auto iteretor = sound2DHandles_.begin(); iteretor != sound2DHandles_.end(); ++iteretor)
	{
		StopSoundMem(iteretor->second);
	}
}