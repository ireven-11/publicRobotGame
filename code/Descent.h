#pragma once

class ActionBase;

class Descent : public ActionBase
{
public:
	Descent();
	~Descent();

	void update(const std::shared_ptr<StatusBase3D>& status, const bool canUpdate);
	
private:

};