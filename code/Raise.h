#pragma once

class ActionBase;

class Raise : public ActionBase
{
public:
	Raise();
	~Raise();

	void update(const std::shared_ptr<StatusBase3D>& status, const bool canUpdate);

private:

};