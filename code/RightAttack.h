#pragma once

class ActionBase;

class RightAttack : public ActionBase
{
public:
	RightAttack();
	~RightAttack();

	void update(const std::shared_ptr<StatusBase3D>& status, const bool canUpdate);

private:

};