#pragma once
#include<memory>

class ReinforcementUIModel;
class ReinforcementUIView;
class ReinforcementSystem;

class ReinforcementUIPresenter
{
public:
	ReinforcementUIPresenter();
	~ReinforcementUIPresenter();

	void draw(const std::shared_ptr<ReinforcementSystem>& reinforcementSystem);

private:
	std::shared_ptr<ReinforcementUIModel> model_;
	std::shared_ptr<ReinforcementUIView> view_;
};