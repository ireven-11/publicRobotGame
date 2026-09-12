#pragma once
#include"StatusBase3D.h"
#include"StageEventHandler.h"

class StageStatus : public StatusBase3D
{
public:
	virtual ~StageStatus() = default;

	virtual void init()		= 0;
	virtual void update()	= 0;
	virtual void clear()	= 0;
	virtual void over()		= 0;
	
	void setLookAtStartingPosition(const VECTOR position) { lookAtStartingPosition_ = position; }

	const bool getIsClear()const noexcept { return isClear_; }
	const bool getIsOver()const noexcept { return isOver_; }
	const VECTOR getLookAtStartingPosition()const noexcept { return lookAtStartingPosition_; }
	const std::string getMissionText()const noexcept { return missionText_; }

protected:
	bool isClear_;
	bool isOver_;
	VECTOR lookAtStartingPosition_;
	std::string missionText_;
};