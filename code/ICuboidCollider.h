#pragma once

class ICuboidCollider
{
public:
	virtual ~ICuboidCollider() = default;

	virtual const VECTOR getCenterPosition()const noexcept	= 0;
	virtual const VECTOR getSizeXYZ()const noexcept	= 0;
	virtual const ObjectAxis getObjectAxis()const noexcept = 0;
};