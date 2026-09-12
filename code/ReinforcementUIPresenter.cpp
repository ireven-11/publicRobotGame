#include"DxLibForIreven.h"
#include"ReinforcementSystem.h"
#include"ReinforcementUIModel.h"
#include"ReinforcementUIView.h"
#include"ReinforcementUIPresenter.h"

ReinforcementUIPresenter::ReinforcementUIPresenter():
	model_(std::make_shared<ReinforcementUIModel>()),
	view_(std::make_shared<ReinforcementUIView>())
{
}

ReinforcementUIPresenter::~ReinforcementUIPresenter()
{
	model_	= nullptr;
	view_	= nullptr;
}

void ReinforcementUIPresenter::draw(const std::shared_ptr<ReinforcementSystem>& reinforcementSystem)
{
	if (!reinforcementSystem->getIsActive()) return;

	view_->drawBackLayer(zero_vector, init_screen_width, init_screen_height, model_->getBackLayerColor());

	const auto choicedReinforcementIndex = reinforcementSystem->getChoicedReinforcement();
	for (size_t i = 0; i < choicedReinforcementIndex.size(); i++)
	{
		const auto offsetX	= model_->getReinforcementGraphOffset() * i;
		VECTOR tempPosition = model_->getReinforcementGraphPosition();
		tempPosition.x		+= offsetX;

		//‘I‘ð‚µ‚Ä‚¢‚é‚à‚Ì‚ð“®‚©‚·
		if (i == reinforcementSystem->getSelectorIndex())
		{
			tempPosition.y = UpDownPositionY(tempPosition.y);
		}

		view_->drawReinforcement(tempPosition, model_->getReinforcementGraphWidht(),
			model_->getReinforcementGraphHeight(), model_->getReinforcementGraphs()[static_cast<ReinforcingType>(choicedReinforcementIndex[i])]);
	}

	view_->drawLevelUpText(model_->getLevelTextPosition(), model_->getLevelText(), model_->getFontHandle(), model_->getTextColor());
}