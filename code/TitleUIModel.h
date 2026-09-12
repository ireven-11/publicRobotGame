#pragma once
#include"nlohmannJson.h"
#include"string.h"
#include"TitleUIData.h"
#include<vector>

class TitleUIModel
{
public:
	TitleUIModel();
	~TitleUIModel();

	void moveNextSelector();
	void movePreviousSelector();

	const int						getButtonsDataSize()const noexcept { return buttonDataSize_; }
	const std::vector<ButtonData>	getButtonDatas()const noexcept { return buttonStatuses_; }
	const SelectorData				getSelectorData()const noexcept { return selectorStatus_; }

private:
	nlohmann::json		data_;
	char				buttonDataSize_;

	std::vector<ButtonData> buttonStatuses_;
	SelectorData			selectorStatus_;
};