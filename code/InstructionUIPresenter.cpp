#include"DxLibForIreven.h"
#include"InstructionUIModel.h"
#include"InstructionUIView.h"
#include"InstructionUIPresenter.h"
#include"StageStatus.h"

InstructionUIPresenter::InstructionUIPresenter()
	:model_(std::make_shared<InstructionUIModel>()),
	view_(std::make_shared<InstructionUIView>())
{
}

InstructionUIPresenter::~InstructionUIPresenter()
{
	view_	= nullptr;
	model_	= nullptr;
}

void InstructionUIPresenter::update()
{

}

void InstructionUIPresenter::draw(const std::shared_ptr<StageStatus> stageStatus)
{
	const auto drawingPosition = GetCenteredTextPosition(model_->getMissionTextPosition(), stageStatus->getMissionText(), model_->getMissionTextFont());
	view_->drawMissionText(stageStatus->getMissionText(), drawingPosition, model_->getMissionTextFont(), model_->getMissionTextColor());
}