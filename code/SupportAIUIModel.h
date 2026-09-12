#pragma once
#include"nlohmannJson.h"
#include<string>
#include<unordered_map>

enum class MessageType
{
	INTRODUCTION,
	WARN_MARCHING,
	INSTRUCT_DESTROY_TARGET,
	GO_HOME,
	DESTROYED_MY_MACHINE,
	OTUKARESAMADESITA,
	SUCSSES_DEFECNCE,
	TOWER_HP_THREE_QUARTERS,
	TOWER_HP_ONE_HALF,
	TOWER_HP_ONE_QUARTERS,
	LEVEL_UP,
	COUNT,	//forで初期化するとき用のカウントなので必ずNONEの前においてください
	NONE
};

class SupportAIUIModel
{
public:
	SupportAIUIModel();
	~SupportAIUIModel();

	void init();
	void setIsDrawingMessage(const MessageType message, const bool isDrawing);
	void setNextMessage(const MessageType originMessage, const MessageType nextMessage);

	const VECTOR getLogoPosition()const noexcept { return logoPosition_; }
	const int getLogoHandle()const noexcept { return logoHandle_; }
	const float getLogoScale()const noexcept { return logoScale_; }
	const VECTOR getTextPosition()const noexcept { return textPosition_; }
	const VECTOR getTextBoxPosition()const noexcept{return textBoxPosition_;}
	const float getTextBoxWidth()const noexcept { return textBoxWidht_; }
	const float getTextBoxHeight()const noexcept { return textBoxHeight_; }
	const int getFontHandle()const noexcept { return fontHandle_; }
	const int getMessageSeconds()const noexcept { return messageSeconds_; }
	const int getTextColor()const noexcept { return textColor_; }
	const int getTextBoxColor()const noexcept { return textBoxColor_; }
	const int getOneLineMessageMaxSize()const noexcept { return oneLineMessageMaxSize_; }
	std::unordered_map<MessageType, std::string> getMesseage()const noexcept { return message_; }
	std::unordered_map<MessageType, bool> getIsDrawingMessage()const noexcept { return isDrawingMessage_; }
	std::unordered_map<MessageType, MessageType> getNextMessage()const noexcept { return nextMessage_; }

private:
	nlohmann::json data_;
	VECTOR logoPosition_;
	int logoHandle_;
	float logoScale_;
	VECTOR textPosition_;
	VECTOR textBoxPosition_;
	float textBoxWidht_;
	float textBoxHeight_;
	int fontHandle_;
	std::unordered_map<MessageType, std::string> message_;
	std::unordered_map<MessageType, bool> isDrawingMessage_;
	std::unordered_map<MessageType, MessageType> nextMessage_;
	int messageSeconds_;
	int textColor_;
	int textBoxColor_;
	int oneLineMessageMaxSize_;
};