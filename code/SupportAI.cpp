#include"DxLib.h"
#include"SupportAIUIModel.h"
#include"SupportAI.h"

SupportAI::SupportAI()
	:ui_(std::make_shared<SupportAIUIPresenter>())
{
}

SupportAI::~SupportAI()
{
	ui_ = nullptr;
}

void SupportAI::init()
{
	ui_->init();
}

void SupportAI::update()
{
	ui_->update([this](const MessageType message) {startSendingMessage(message); });
}

void SupportAI::draw()
{
	ui_->draw();
}

void SupportAI::setMessageSound(const MessageType message, const std::function<void()>& soundPlayer)
{
	soundPlayers_[message] = soundPlayer;
}

void SupportAI::startSendingMessage(const MessageType message)
{
	ui_->startDrawing(message);
	soundPlayers_[message]();
}