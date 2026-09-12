#include"DxLibForIreven.h"
#include"TitleUIView.h"

TitleUIView::TitleUIView()
{
}

TitleUIView::~TitleUIView()
{
}

void TitleUIView::draw(const std::vector<ButtonData>& buttonData, const SelectorData selectorData)
{
	drawSelector(buttonData, selectorData);
	drawButtons(buttonData);
}

void TitleUIView::drawButtons(const std::vector<ButtonData>& buttonData)
{
	for (const auto& data : buttonData)
	{
		const auto text				= data.text;
		const auto drawingPosition	= GetCenteredTextPosition(data.position, text, data.fontHandle);

		DrawStringFToHandle(drawingPosition.x, drawingPosition.y, text.c_str(), data.colorCode, data.fontHandle);
	}
}

void TitleUIView::drawSelector(const std::vector<ButtonData>& buttonData, const SelectorData selectorData)
{
	const VECTOR drawingPosition = buttonData[selectorData.index].position;

	DrawBox(drawingPosition.x - selectorData.widht * 0.5f, drawingPosition.y - selectorData.height * 0.5f,
		drawingPosition.x + selectorData.widht * 0.5f, drawingPosition.y + selectorData.height * 0.5f, selectorData.color, true);
}