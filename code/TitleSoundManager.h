#pragma once
#include<unordered_map>
#include<string>
#include"I2DSoundManager.h"
#include"SoundManagerBase.h"

class TitleSoundManager : public SoundManagerBase, public I2DSoundManager
{
public:
	TitleSoundManager();
	~TitleSoundManager();

	void init();
	void start2DSound(const std::string& soundName, const bool isLoop = false, const bool canAllowDuplicate = false);
	void stop2DSound(const std::string& soundName);
	void stopAllSound();

private:
	void adjustVolume();
	void eventRegister();

	std::unordered_map<std::string, int> soundHandles_;
};