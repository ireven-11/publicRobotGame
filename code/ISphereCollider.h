#pragma once

class ISphereCollider
{
public:
	virtual ~ISphereCollider() = default;

	virtual const VECTOR	getSpherePosition()const noexcept = 0;
	virtual const float		getRadius()const noexcept = 0;
};