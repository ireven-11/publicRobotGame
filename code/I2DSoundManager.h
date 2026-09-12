#pragma once
#include<string>

class I2DSoundManager
{
public:
	virtual ~I2DSoundManager() = default;

	virtual void start2DSound(const std::string& soundName, const bool isLoop = false, const bool canAllowDuplicate = false) = 0;
	virtual void stop2DSound(const std::string& soundName) = 0;
};