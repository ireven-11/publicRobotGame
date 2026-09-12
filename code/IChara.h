#pragma once
#include<memory>

class CharaStatus;

class IChara
{
public:
	virtual ~IChara() = default;

	virtual void init()			= 0;
	virtual void update()		= 0;
	virtual void draw()const	= 0;
	
	virtual const std::shared_ptr<CharaStatus> getStatus()const noexcept = 0;
};