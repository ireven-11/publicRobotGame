#include"DxLibForIreven.h"
#include"PlayerUIView.h"

PlayerUIView::PlayerUIView()
{
}

PlayerUIView::~PlayerUIView()
{
}

void PlayerUIView::drawScreenAimingCircle(const ScreenAimingCircle& circle)
{
	//”¼“§–¾‚É‚µ‚Ä•`‰æ‚·‚é
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, UCHAR_MAX * 0.5f);
	DrawExtendGraphF(circle.position.x - circle.radius, circle.position.y - circle.radius,
		circle.position.x + circle.radius, circle.position.y + circle.radius, circle.graph, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void PlayerUIView::drawHpGage(const GageData& mainGage, const GageData& backGage, const GageData& layerGage, 
	const VECTOR textPosition, const std::string& text, const int fontHandle, const int textColor,
	const VECTOR logoPosition, const float logoScale, const int logoGraph)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, UCHAR_MAX * 0.15f);
	DrawBox(layerGage.position.x - layerGage.width * 0.5f, layerGage.position.y - layerGage.height * 0.5f,
		layerGage.position.x + layerGage.width * 0.5f, layerGage.position.y + layerGage.height * 0.5f, layerGage.color, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawBox(backGage.position.x - backGage.width * 0.5f, backGage.position.y - backGage.height * 0.5f,
		backGage.position.x + backGage.width * 0.5f, backGage.position.y + backGage.height * 0.5f, backGage.color, true);

	DrawBox(mainGage.position.x - mainGage.width * 0.5f, mainGage.position.y - mainGage.height * 0.5f,
		mainGage.position.x + mainGage.width * 0.5f, mainGage.position.y + mainGage.height * 0.5f, mainGage.color, true);

	DrawRotaGraphF(logoPosition.x, logoPosition.y, logoScale, 0.0, logoGraph, true);

	DrawStringFToHandle(textPosition.x, textPosition.y, text.c_str(), textColor, fontHandle);
}

void PlayerUIView::drawTargetBox(const VECTOR drawingPosition, const float boxSize, const ObjectAxis& axis, const int boxColor)
{
	const VECTOR boxOffset = VGet(boxSize, boxSize, boxSize);

	SetUseLighting(false);
	SetUseZBufferFlag(false);
	DrawCuboid3D(drawingPosition, boxOffset, axis, boxColor);
	SetUseZBufferFlag(true);
	SetUseLighting(true);
}

void PlayerUIView::drawLevelGage(const GageData& expGage, const GageData& backGage)
{
	DrawBox(backGage.position.x - backGage.width * 0.5f, backGage.position.y - backGage.height * 0.5f,
		backGage.position.x + backGage.width * 0.5f, backGage.position.y + backGage.height * 0.5f, backGage.color, true);

	DrawBox(expGage.position.x - expGage.width * 0.5f, expGage.position.y - expGage.height * 0.5f,
		expGage.position.x + expGage.width * 0.5f, expGage.position.y + expGage.height * 0.5f, expGage.color, true);
}