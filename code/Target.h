#pragma once
#include"IMapObject.h"

class MapObjectStatus;

class Target : public IMapObject
{
public:
	Target(const VECTOR initPosition);
	~Target();

	void update();
	void draw()const;

	const std::shared_ptr<MapObjectStatus> getStatus()const noexcept { return status_; }

private:
	std::shared_ptr<MapObjectStatus> status_;
};