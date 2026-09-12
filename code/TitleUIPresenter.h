#pragma once
#include<memory>
#include<string>
#include"TitleUIData.h"
#include"TitleUIModel.h"

class TitleUIView;
class TitleUIModel;

class TitleUIPresenter
{
public:
	TitleUIPresenter();
	~TitleUIPresenter();

	void update();
	void draw();

	const bool getIsMovingSelector()const noexcept { return isMovingSelector_; }
	const int getSelectedButtonIndex()const noexcept { return model_->getSelectorData().index; }

private:
	std::shared_ptr<TitleUIView> view_;
	std::shared_ptr<TitleUIModel> model_;
	bool isMovingSelector_;
};