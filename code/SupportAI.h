#pragma once
#include<functional>
#include<unordered_map>
#include"SupportAIUIPresenter.h"

class SupportAIUIPresenter;
enum class MessageType;

class SupportAI
{
public:
	SupportAI();
	~SupportAI();

	void init();
	void update();
	void draw();

	void setMessageSound(const MessageType message, const std::function<void()>& soundPlayer);
	void startSendingMessage(const MessageType message);

	const bool getIsFinishedLastmessage()const noexcept { return ui_->getIsFinishedLastmessage(); }

private:
	std::shared_ptr<SupportAIUIPresenter> ui_;
	std::unordered_map<MessageType, std::function<void()>> soundPlayers_;
};