#include"DxLibForIreven.h"
#include"SupportAIUIModel.h"
#include"string.h"
#include<vector>

SupportAIUIModel::SupportAIUIModel()
{
	data_ = LoadDataJson("statusData/uiData/SupportAIUIStatus.json");

	logoHandle_ = LoadGraph(data_["logo_path"].get<std::string>().c_str());
	logoScale_	= data_["logo_scale"].get<float>();

	const auto logoPosition = data_["logo_position"];
	logoPosition_			= VGet(logoPosition[0].get<float>(), logoPosition[1].get<float>(), logoPosition[2].get<float>());

	const auto textPosition = data_["text_position"];
	textPosition_			= VGet(textPosition[0].get<float>(), textPosition[1].get<float>(), textPosition[2].get<float>());
	textBoxWidht_			= data_["text_box_widht"].get<float>();
	textBoxHeight_			= data_["textbox_height"].get<float>();

	const auto boxPosition	= data_["text_box_position"];
	textBoxPosition_		= VGet(boxPosition[0].get<float>(), boxPosition[1].get<float>(), boxPosition[2].get<float>());

	const auto textColor	= data_["text_color"];
	textColor_				= GetColor(textColor[0].get<char>(), textColor[1].get<char>(), textColor[2].get<char>());

	const auto boxColor		= data_["text_box_color"];
	textBoxColor_			= GetColor(boxColor[0].get<char>(), boxColor[1].get<char>(), boxColor[2].get<char>());

	messageSeconds_ = data_["message_seconds"].get<int>();

	const auto font = data_["font"];
	fontHandle_ = CreateFontToHandle(Utf8ToSJIS(font["name"].get<std::string>()).c_str(), font["size"].get<int>(),
		font["thick"].get<int>(), DX_FONTTYPE_ANTIALIASING_4X4);

	oneLineMessageMaxSize_ = data_["one_line_message_max_size"].get<int>();

	const auto supportText = data_["support_text"];

	message_[MessageType::INTRODUCTION] = Utf8ToSJIS(supportText["intoroduction"].get<std::string>());
	
	message_[MessageType::WARN_MARCHING] = Utf8ToSJIS(supportText["warn_marching"].get<std::string>());
	
	message_[MessageType::INSTRUCT_DESTROY_TARGET] = Utf8ToSJIS(supportText["instruct_destroy_target"].get<std::string>());
	
	message_[MessageType::GO_HOME] = Utf8ToSJIS(supportText["go_home"].get<std::string>());
	
	message_[MessageType::DESTROYED_MY_MACHINE] = Utf8ToSJIS(supportText["destroyed_my_machine"].get<std::string>());
	
	message_[MessageType::OTUKARESAMADESITA] = Utf8ToSJIS(supportText["otukaresamadesita"].get<std::string>());
	
	message_[MessageType::SUCSSES_DEFECNCE] = Utf8ToSJIS(supportText["success_defence"].get<std::string>());

	message_[MessageType::TOWER_HP_THREE_QUARTERS] = Utf8ToSJIS(supportText["defencing_target_hp_three_quarters"].get<std::string>());

	message_[MessageType::TOWER_HP_ONE_HALF] = Utf8ToSJIS(supportText["defencing_target_hp_one_half"].get<std::string>());

	message_[MessageType::TOWER_HP_ONE_QUARTERS] = Utf8ToSJIS(supportText["defencing_target_hp_one_quarters"].get<std::string>());

	message_[MessageType::LEVEL_UP] = Utf8ToSJIS(supportText["level_up"].get<std::string>());

	init();
}

SupportAIUIModel::~SupportAIUIModel()
{
	DeleteGraph(logoHandle_);
}

void SupportAIUIModel::init()
{
	for (int i = 0; i < static_cast<int>(MessageType::COUNT); i++)
	{
		const auto messageIndex			= static_cast<MessageType>(i);
		isDrawingMessage_[messageIndex] = false;
		nextMessage_[messageIndex]		= MessageType::NONE;
	}
}

void SupportAIUIModel::setIsDrawingMessage(const MessageType message, const bool isDrawing)
{
	isDrawingMessage_[message] = isDrawing;
}

void SupportAIUIModel::setNextMessage(const MessageType originMessage, const MessageType nextMessage)
{
	nextMessage_[originMessage] = nextMessage;
}