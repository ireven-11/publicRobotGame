#pragma once

class ICapsuleCollider
{
public:
	virtual ~ICapsuleCollider() = default;

	virtual const VECTOR	getCapsuleBottomPosition()const noexcept	= 0;
	virtual const VECTOR	getCapsuleTopPosition()const noexcept		= 0;
	virtual const float		getCapsuleRadius()const noexcept			= 0;
};