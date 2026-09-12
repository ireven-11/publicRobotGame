#pragma once
#pragma once

class MapObjectStatus;

class HousingComplexStatus : public MapObjectStatus
{
public:
	HousingComplexStatus() = default;
	HousingComplexStatus(const VECTOR initPosition, const float rotationAngleY);
	~HousingComplexStatus();

	void init();
	void update();
	
	const float	getAdjustingAttackDistance()const noexcept;

private:
	static int	HousingComplexModel_;

	const VECTOR	init_position;
	const float		rotation_angle_y;
	const float		adjust_HousingComplex_collider_position = 0.75f;
};