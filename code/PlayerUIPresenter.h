#pragma once
#include<memory>
#include<string>

class PlayerUIView;
class PlayerUIModel;
class PlayerStatus;

class PlayerUIPresenter
{
public:
	PlayerUIPresenter();
	~PlayerUIPresenter();

	void update();
	void draw(const std::shared_ptr<PlayerStatus>& status);

private:
	std::shared_ptr<PlayerUIView> view_;
	std::shared_ptr<PlayerUIModel> model_;
};