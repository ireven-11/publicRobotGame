#pragma once
#include"IMapObject.h"

class MapObjectStatus;

class Tower : public IMapObject
{
public:
	Tower(const VECTOR initposition, const std::function<void()>& hpThreeQuarters, const std::function<void()>& hpOneHalf, const std::function<void()>& hpOneQuarters);
	~Tower();

	void update();
	void draw()const;

	const std::shared_ptr<MapObjectStatus> getStatus()const noexcept { return status_; }

private:
	std::shared_ptr<MapObjectStatus> status_;
};