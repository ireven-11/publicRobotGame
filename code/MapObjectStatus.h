#pragma once
#include"StatusBase3D.h"
#include"ICuboidCollider.h"

class MapObjectStatus : public StatusBase3D, public ICuboidCollider
{
public:
	virtual ~MapObjectStatus() = default;

	virtual void init()		= 0;
	virtual void update()	= 0;

	const VECTOR		getCenterPosition()const noexcept { return centerColliderPosition_; }
	const VECTOR		getSizeXYZ()const noexcept { return cuboidSizeXYZ_; }
	const ObjectAxis	getObjectAxis()const noexcept { return axis_; }

protected:
	VECTOR		centerColliderPosition_;
	VECTOR		cuboidSizeXYZ_;
	ObjectAxis  axis_;
};