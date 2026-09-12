#pragma once
#include"IMapObject.h"

class MapObjectStatus;

class HousingComplex : public IMapObject
{
public:
	HousingComplex(const VECTOR initPosition, const float rotationAngleY);
	~HousingComplex();

	void update();
	void draw()const;

	const std::shared_ptr<MapObjectStatus> getStatus()const noexcept { return status_; }

private:
	std::shared_ptr<MapObjectStatus> status_;
};