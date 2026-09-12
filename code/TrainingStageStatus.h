#pragma once
#include"StageStatus.h"

class TrainingStageStatus : public StageStatus
{
public:
	TrainingStageStatus();
	~TrainingStageStatus();

	void init();
	void update();
	void clear();
	void over();

private:

};