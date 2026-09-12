#pragma once

class ActionBase;

class Interact : public ActionBase
{
public:
	Interact();
	~Interact();

	void update(const std::shared_ptr<StatusBase3D>& status, const bool canUpdate);

private:

};