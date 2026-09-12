#pragma once

class MapObjectStatus;

class IMapObject
{
public:
	virtual ~IMapObject() = default;

	virtual void update()		= 0;
	virtual void draw()const	= 0;

	virtual const std::shared_ptr<MapObjectStatus> getStatus()const noexcept = 0;
};