#pragma once
#include"IWeapon.h"

class NoWeapon : public IWeapon
{
public:
	NoWeapon();
	~NoWeapon();

	void update();
	void draw();
	void setTransform(const MATRIX& equipingMatrix);
};