#include"DxLibForIreven.h"
#include"DInput.h"
#include"XInput.h"
#include"Mouse.h"
#include"KeyInput.h"
#include"InputHandler.h"
#include<cassert>
#include"RightAttackEvent.h"
#include"LeftAttackEvent.h"
#include"RaiseEvent.h"
#include"DescentEvent.h"
#include"CameraEvent.h"
#include"DashEvent.h"
#include"WalkEvent.h"

InputHandler::InputHandler()
	:mouse_(std::make_shared<Mouse>()),
	keyInput_(std::make_shared<KeyInput>()),
	controllerTypePointer_(std::make_shared<int>(controllerType_)),
	rightAttackEvent_(std::make_shared<RightAttackEvent>()),
	leftAttackEvent_(std::make_shared<LeftAttackEvent>()),
	raiseEvent_(std::make_shared<RaiseEvent>()),
	descentEvent_(std::make_shared<DescentEvent>()),
	cameraEvent_(std::make_shared<CameraEvent>()),
	dashEvent_(std::make_shared<DashEvent>()),
	walkEvent_(std::make_shared<WalkEvent>())
{
	warnNonConnectingController();

	xInput_ = std::make_shared<XInput>(DX_INPUT_PAD1);
	dInput_ = std::make_shared<DInput>(DX_INPUT_PAD1);

	init();
}

InputHandler::~InputHandler()
{
	xInput_					= nullptr;
	dInput_					= nullptr;
	mouse_					= nullptr;
	keyInput_				= nullptr;
	controllerTypePointer_	= nullptr;

	rightAttackEvent_		= nullptr;
	leftAttackEvent_		= nullptr;
	raiseEvent_				= nullptr;
	descentEvent_			= nullptr;
	cameraEvent_			= nullptr;
	dashEvent_				= nullptr;
	walkEvent_				= nullptr;
}

void InputHandler::init()
{
	controllerType_ = 0;

	bool isSafe = warnNonConnectingController();
	if (!isSafe) return;

	controllerType_ = GetJoypadType(DX_INPUT_PAD1);

	switch (controllerType_)
	{
		case DX_PADTYPE_SWITCH_PRO_CTRL:
		{
			dInput_->initProcon();

			break;
		}
		case DX_PADTYPE_XBOX_360:
		{
			xInput_->init();

			break;
		}
		case DX_PADTYPE_XBOX_ONE:
		{
			xInput_->init();

			break;
		}
	}
}

void InputHandler::update()
{
	//コントローラーが変わったらもう一回初期化
	if (controllerType_ != GetJoypadType(DX_INPUT_PAD1))
	{
		init();
	}

	bool isSafe = warnNonConnectingController();
	if (!isSafe) return;

	xInput_->update();
	dInput_->update();
	mouse_->update();
	keyInput_->update();
}

bool InputHandler::warnNonConnectingController()
{
	while (gameRoopSetting)
	{
		//コントローラーがつながってたら処理をやめる。（正常終了）
		if (GetJoypadNum() > 0) return true;

		const int screenCenterX = 1000;
		const int screenCenterY = 500;
		DrawString(screenCenterX, screenCenterY, "コントローラーを繋いでください", GetColor(255, 5, 5));

		//裏画面の内容を表画面に反映(ゲームループの最後に呼ぶ)
		ScreenFlip();
	}

	//強制終了(危険：おそらくゲームが強制終了します)
	return false;
}