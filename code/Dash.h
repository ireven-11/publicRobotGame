#pragma once
#include"Walk.h"

class Dash : public Walk
{
public:
	Dash();
	~Dash();

	void update(const std::shared_ptr<StatusBase3D>& status, const bool canUpdate)override;
	
private:

};