#include"DxLibForIreven.h"
#include"Timer.h"
#include"SupportAIUIModel.h"
#include"SupportAIUIView.h"
#include"SupportAIUIPresenter.h"
#include"StageEventHandler.h"
#include"StageClearEvent.h"
#include"StageOverEvent.h"

SupportAIUIPresenter::SupportAIUIPresenter()
	:model_(std::make_shared<SupportAIUIModel>()),
	view_(std::make_shared<SupportAIUIView>()),
	messageTimer_(std::make_shared<Timer>())
{
	isFinishedLastMessage_ = false;
}

SupportAIUIPresenter::~SupportAIUIPresenter()
{
	view_			= nullptr;
	model_			= nullptr;
	messageTimer_	= nullptr;
}

void SupportAIUIPresenter::init()
{
	isFinishedLastMessage_ = false;

	model_->init();

	//特定のメッセージの次に描画したいメッセージを設定
	model_->setNextMessage(MessageType::INTRODUCTION, MessageType::WARN_MARCHING);
	model_->setNextMessage(MessageType::SUCSSES_DEFECNCE, MessageType::GO_HOME);
	model_->setNextMessage(MessageType::GO_HOME, MessageType::OTUKARESAMADESITA);
	model_->setNextMessage(MessageType::INSTRUCT_DESTROY_TARGET, MessageType::GO_HOME);
	model_->setNextMessage(MessageType::DESTROYED_MY_MACHINE, MessageType::GO_HOME);
}

void SupportAIUIPresenter::update(const std::function<void(const MessageType)>& sendMessageFunction)
{
	messageTimer_->update();

	for (int i = 0; i < static_cast<int>(MessageType::COUNT); i++)
	{
		const auto messageIndex = static_cast<MessageType>(i);
		if (model_->getIsDrawingMessage()[messageIndex] && messageTimer_->hasFinishedCountDown())
		{
			model_->setIsDrawingMessage(messageIndex, false);
			messageTimer_->init();

			//「お疲れ様でした」は最後に流すメッセージ
			if (messageIndex == MessageType::OTUKARESAMADESITA)
			{
				isFinishedLastMessage_ = true;
				return;
			}

			//次に描画するメッセージが設定されていれば次のメッセージを描画を始める
			if (model_->getNextMessage()[messageIndex] == MessageType::NONE) return;
			sendMessageFunction(model_->getNextMessage()[messageIndex]);

			return;
		}
	}
}

void SupportAIUIPresenter::draw()
{
	for (int i = 0; i < static_cast<int>(MessageType::COUNT); i++)
	{
		const auto messageIndex = static_cast<MessageType>(i);
		if (model_->getIsDrawingMessage()[messageIndex])
		{
			view_->drawTextBox(model_->getMesseage()[messageIndex], model_->getTextPosition(), model_->getTextBoxPosition(), model_->getTextBoxWidth(), model_->getTextBoxHeight(),
				model_->getTextBoxColor(), model_->getFontHandle(), model_->getTextColor(), model_->getOneLineMessageMaxSize());
			view_->drawLogo(model_->getLogoPosition(), model_->getLogoScale(), model_->getLogoHandle());
		}
	}
}

void SupportAIUIPresenter::startDrawing(const MessageType message)
{
	//メッセージ描画を始める時に他のメッセージの描画は止める
	for (int i = 0; i < static_cast<int>(MessageType::COUNT); i++)
	{
		const auto messageIndex = static_cast<MessageType>(i);
		model_->setIsDrawingMessage(messageIndex, false);
	}

	model_->setIsDrawingMessage(message, true);
	messageTimer_->startCountDown(model_->getMessageSeconds(), 0);
}