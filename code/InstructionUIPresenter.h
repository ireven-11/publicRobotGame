#pragma once
#include<memory>
#include<string>

class InstructionUIView;
class InstructionUIModel;
class StageStatus;

class InstructionUIPresenter
{
public:
	InstructionUIPresenter();
	~InstructionUIPresenter();

	void update();
	void draw(const std::shared_ptr<StageStatus> stageStatus);

private:
	std::shared_ptr<InstructionUIView> view_;
	std::shared_ptr<InstructionUIModel> model_;
};