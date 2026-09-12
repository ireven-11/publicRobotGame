#pragma once
#include<memory>
#include<string>

class SupportAIUIView;
class SupportAIUIModel;
class Timer;
enum class MessageType;

class SupportAIUIPresenter
{
public:
	SupportAIUIPresenter();
	~SupportAIUIPresenter();

	void init();
	void update(const std::function<void(MessageType)>& sendMessageFunction);
	void draw();
	void startDrawing(const MessageType message);
	
	const bool getIsFinishedLastmessage()const noexcept { return isFinishedLastMessage_; }
	
private:
	std::shared_ptr<SupportAIUIView> view_;
	std::shared_ptr<SupportAIUIModel> model_;
	std::shared_ptr<Timer> messageTimer_;
	bool isFinishedLastMessage_;
};