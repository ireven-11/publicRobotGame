#pragma once

class ActionBase;

class ShoulderAttack : public ActionBase
{
public:
	ShoulderAttack();
	~ShoulderAttack();

	void update(const std::shared_ptr<StatusBase3D>& status, const bool canUpdate);

private:

};