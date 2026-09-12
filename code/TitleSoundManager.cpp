#include"DxLib.h"
#include"TitleSoundManager.h"

TitleSoundManager::TitleSoundManager()
{
	soundData_ = LoadDataJson("statusData/soundData/titleSoundStatus.json");

	soundHandles_["bgm"]			= LoadSoundMem(soundData_["bgm_path"].get<std::string>().c_str());
	soundHandles_["selectButton"]	= LoadSoundMem(soundData_["select_button_se_path"].get<std::string>().c_str());
	soundHandles_["decide"]			= LoadSoundMem(soundData_["decide_se_path"].get<std::string>().c_str());

	adjustVolume();

	//サウンドの処理をイベントに登録
	eventRegister();
}

TitleSoundManager::~TitleSoundManager()
{
	soundHandles_.clear();
	InitSoundMem();
}

void TitleSoundManager::adjustVolume()
{
	ChangeVolumeSoundMem(soundData_["bgm_volume"].get<short>(), soundHandles_["bgm"]);
	ChangeVolumeSoundMem(soundData_["select_button_se_volume"].get<short>(), soundHandles_["selectButton"]);
	ChangeVolumeSoundMem(soundData_["decide_se_volume"].get<short>(), soundHandles_["decide"]);
}

void TitleSoundManager::eventRegister()
{
	
}

void TitleSoundManager::init()
{
	stopAllSound();

	start2DSound("bgm", true);
}

void TitleSoundManager::start2DSound(const std::string& soundName, const bool isLoop, const bool canAllowDuplicate)
{
	//検索した名前がなければ処理をしない
	const auto isExisting = soundHandles_.find(soundName);
	if (isExisting == soundHandles_.end()) return;

	const auto soundHandle = soundHandles_[soundName];

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

void TitleSoundManager::stop2DSound(const std::string& soundName)
{
	//検索した名前がなければ処理をしない
	const auto isExisting = soundHandles_.find(soundName);
	if (isExisting == soundHandles_.end()) return;

	StopSoundMem(soundHandles_[soundName]);
}

void TitleSoundManager::stopAllSound()
{
	for (auto iteretor = soundHandles_.begin(); iteretor != soundHandles_.end(); ++iteretor)
	{
		StopSoundMem(iteretor->second);
	}
}