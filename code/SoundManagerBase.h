#pragma once
#include<unordered_map>
#include"nlohmannJson.h"

class SoundManagerBase
{
public:
	SoundManagerBase() {};
	~SoundManagerBase() {};

	virtual void init()			= 0;
	virtual void stopAllSound() = 0;
	
protected:
	virtual void adjustVolume()		= 0;
	virtual void eventRegister()	= 0;

	nlohmann::json soundData_;
};