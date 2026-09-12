#pragma once

class ActionBase;

class LeftAttack : public ActionBase
{
public:
	LeftAttack();
	~LeftAttack();

	void update(const std::shared_ptr<StatusBase3D>& status, const bool canUpdate);

private:

};