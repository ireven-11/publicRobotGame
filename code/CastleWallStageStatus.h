#pragma once
#include"StageStatus.h"

struct DestinationNode
{
	VECTOR position_;
	
	void setNext(const DestinationNode& next)
	{
		position_ = next.position_;
	}
};

class CastleWallStageStatus : public StageStatus
{
public:
	CastleWallStageStatus();
	~CastleWallStageStatus();

	void init();
	void update();
	void clear();
	void over();

	const int		getSkyBoxHandle()const noexcept { return skyBox_; }
	const VECTOR	getSkyBoxPosition()const noexcept { return skyBoxPosition_; }

private:
	int		skyBox_;
	VECTOR	skyBoxPosition_;

	//test
	std::vector<DestinationNode> destination_;

	const float expand_map_y = 1.5f;
};