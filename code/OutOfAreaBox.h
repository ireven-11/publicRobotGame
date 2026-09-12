#pragma once
#include"IMapObject.h"

class MapObjectStatus;

class OutOfAreaBox : public IMapObject
{
public:
	OutOfAreaBox(const VECTOR initposition, const float mapModelScale, const VECTOR standardSize);
	~OutOfAreaBox();

	void update();
	void draw()const;

	const std::shared_ptr<MapObjectStatus> getStatus()const noexcept { return status_; }

private:
	std::shared_ptr<MapObjectStatus> status_;
};