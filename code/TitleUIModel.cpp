#include"DxLibForIreven.h"
#include"TitleUIModel.h"

TitleUIModel::TitleUIModel()
{
	data_ = LoadDataJson("statusData/uiData/titleUIStatus.json");

	const auto buttonsData		= data_["buttons_status"];
	const auto selectorData		= data_["selector_status"];
	const auto fontData			= data_["font_status"];
	const auto textColorData	= data_["standard_text_color"];
	const auto moviesData		= data_["movies_status"];

	const auto selectorColor	= selectorData["color"];

	const auto textColor	= GetColor(textColorData[0].get<short>(), textColorData[1].get<short>(), textColorData[2].get<short>());
	const auto fontHandle	= CreateFontToHandle(Utf8ToSJIS(fontData["name"].get<std::string>()).c_str(),
		fontData["size"].get<int>(), fontData["thick"].get<int>(), DX_FONTTYPE_ANTIALIASING_4X4);

	buttonDataSize_			= buttonsData.size();
	
	//ボタンデータをセット
	for (int i = 0; i < buttonDataSize_; i++)
	{
		buttonStatuses_.emplace_back();

		const auto currentIndexData = buttonsData[i];
		const auto tempPosition		= currentIndexData["position"];

		buttonStatuses_[i].position		= VGet(tempPosition[0].get<float>(), tempPosition[1].get<float>(), tempPosition[2].get<float>());
		buttonStatuses_[i].text			= Utf8ToSJIS(currentIndexData["text"].get<std::string>());
		buttonStatuses_[i].colorCode	= textColor;
		buttonStatuses_[i].fontHandle	= fontHandle;
	}

	//セレクターデータをセット
	selectorStatus_.widht	= selectorData["widht"].get<float>();
	selectorStatus_.height	= selectorData["height"].get<float>();
	selectorStatus_.color	= GetColor(selectorColor[0].get<short>(), selectorColor[1].get<short>(), selectorColor[2].get<short>());
	selectorStatus_.index	= 0;
}

TitleUIModel::~TitleUIModel()
{
	buttonStatuses_.clear();
}


void TitleUIModel::moveNextSelector()
{
	++selectorStatus_.index;

	if (selectorStatus_.index > buttonDataSize_ - 1)
	{
		selectorStatus_.index = 0;
	}
}

void TitleUIModel::movePreviousSelector()
{
	--selectorStatus_.index;

	if (selectorStatus_.index < 0)
	{
		selectorStatus_.index = buttonDataSize_ - 1;
	}
}