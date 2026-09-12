#pragma once

//オブジェクトの各方向を表すVECTORをまとめたaxis
class ObjectAxis
{
public:
	ObjectAxis()
	{
		rightAxis_	= VGet(1.0f, 0.0f, 0.0f);
		upAxis_		= VGet(0.0f, 1.0f, 0.0f);
		fowardAxis_ = VGet(0.0f, 0.0f, 1.0f);
	}
	~ObjectAxis()
	{
	}

	void rotationX(const float rotationAngle)
	{
		upAxis_		= VTransform(upAxis_, MGetRotX(rotationAngle));
		rightAxis_	= VTransform(rightAxis_, MGetRotX(rotationAngle));
		fowardAxis_ = VTransform(fowardAxis_, MGetRotX(rotationAngle));
	}
	void rotationY(const float rotationAngle)
	{
		upAxis_		= VTransform(upAxis_, MGetRotY(rotationAngle));
		rightAxis_	= VTransform(rightAxis_, MGetRotY(rotationAngle));
		fowardAxis_ = VTransform(fowardAxis_, MGetRotY(rotationAngle));
	}
	void rotationZ(const float rotationAngle)
	{
		upAxis_		= VTransform(upAxis_, MGetRotZ(rotationAngle));
		rightAxis_	= VTransform(rightAxis_, MGetRotZ(rotationAngle));
		fowardAxis_ = VTransform(fowardAxis_, MGetRotZ(rotationAngle));
	}

	const VECTOR getRightAxis()const noexcept { return rightAxis_; }
	const VECTOR getUpAxis()const noexcept { return upAxis_; }
	const VECTOR getFowardAxis()const noexcept { return fowardAxis_; }

private:
	VECTOR rightAxis_;
	VECTOR upAxis_;
	VECTOR fowardAxis_;
};