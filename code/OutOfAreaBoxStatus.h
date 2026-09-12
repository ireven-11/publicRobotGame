#pragma once

class MapObjectStatus;

class OutOfAreaBoxStatus : public MapObjectStatus
{
public:
	OutOfAreaBoxStatus() = default;
	OutOfAreaBoxStatus(const VECTOR initPosition, const float mapModelScale, const VECTOR standardSize);
	~OutOfAreaBoxStatus();

	void init();
	void update();
	
private:
	
	const VECTOR init_position;
	const float adjust_position = 0.5f;
};