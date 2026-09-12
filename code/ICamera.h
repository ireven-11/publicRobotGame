#pragma once
#include"PostProcessing.h"
#include<memory>

class ICamera
{
public:
	virtual ~ICamera() = default;

	virtual void init()																= 0;
	virtual void update(const VECTOR targetPostion, const float cameraAngle = 0.0f)	= 0;

	virtual const std::shared_ptr<PostProcessing> getPostProcessing()const noexcept = 0;

protected:
	virtual void rotation(const VECTOR targetPostion, const float cameraAngle = 0.0f) = 0;
};