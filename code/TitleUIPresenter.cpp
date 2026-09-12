#include"DxLib.h"
#include"TitleUIView.h"
#include"TitleUIPresenter.h"
#include<vector>
#include<string>
#include"InputHandler.h"
#include"DInput.h"

TitleUIPresenter::TitleUIPresenter()
	:model_(std::make_shared<TitleUIModel>()),
	view_(std::make_shared<TitleUIView>())
{
}

TitleUIPresenter::~TitleUIPresenter()
{
	view_	= nullptr;
	model_	= nullptr;
}

void TitleUIPresenter::update()
{
	isMovingSelector_ = false;

	//スティックを倒してなかったらこの先の処理をしない
	if (!InputHandler::instance().getDInput()->getStickTiledMoment(PROCON::STICK::LEFT_Y)) return;

	//スティックを上に倒したとき
	if (InputHandler::instance().getDInput()->getProconStickXY(PROCON::STICK::LEFT_Y) < 0)
	{
		model_->movePreviousSelector();
	}
	else if (InputHandler::instance().getDInput()->getProconStickXY(PROCON::STICK::LEFT_Y) > 0)
	{
		model_->moveNextSelector();
	}

	isMovingSelector_ = true;
}

void TitleUIPresenter::draw()
{
	view_->draw(model_->getButtonDatas(), model_->getSelectorData());
}