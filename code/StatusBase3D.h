#pragma once
#include"nlohmannJson.h"
#include"ObjectAxis.h"

class ObjectAxis;

class StatusBase3D
{
public:
	virtual ~StatusBase3D() = default;

	const int				getModelHandle()const noexcept { return modelHandle_; }
	const VECTOR			getPosition()const noexcept { return position_; }
	const nlohmann::json	getStatusData()const noexcept { return statusData_; }
	//const ObjectAxis		getObjectAxis()const noexcept { return axis_; }

protected:
	nlohmann::json statusData_;

	VECTOR		position_;
	int			modelHandle_;
	//ObjectAxis	axis_;
};