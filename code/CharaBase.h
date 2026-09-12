#pragma once
#include"IChara.h"
#include"CharaStatus.h"

class CharaBase : public IChara
{
public:
	virtual ~CharaBase()	= default;

	virtual void init()			= 0;
	virtual void update()		= 0;
	virtual void draw()const	= 0;

	const std::shared_ptr<CharaStatus> getStatus()const noexcept { return status_; }

protected:
	std::shared_ptr<CharaStatus> status_;
};