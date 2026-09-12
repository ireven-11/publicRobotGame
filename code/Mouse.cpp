#include"DxLib.h"
#include"InputHandler.h"
#include"Mouse.h"

Mouse::Mouse()
{
	//引数にtrueでマウス表示、falseで非表示
	SetMouseDispFlag(true);
	
	init();
}

Mouse::~Mouse()
{
}

void Mouse::init()
{
	elapsedTimeCounter_ = 0;

	cursorPosition_ = VGet(init_cusor_pos.x, init_cusor_pos.y, 0.0f);
	for (int i = 0; i < mouse_button_buffer_size; i++)
	{
		buttonState_[i].frame_		= 0;
		buttonState_[i].time_		= 0;
		buttonState_[i].onPressed_	= false;
	}
}

void Mouse::update()
{
	//マウスの座標を更新
	int currentX	= 0;
	int currentY	= 0;
	GetMousePoint(&currentX, &currentY);
	cursorPosition_ = VGet(static_cast<float>(currentX), static_cast<float>(currentY), 0.0f);

	//入力を更新
	input();
}

void Mouse::input()
{
	bool isPressedLeftButton	= (GetMouseInput() & MOUSE_INPUT_LEFT)		!= 0;
	bool isPressedRightButton	= (GetMouseInput() & MOUSE_INPUT_RIGHT)		!= 0;
	bool isPressedMiddleButton	= (GetMouseInput() & MOUSE_INPUT_MIDDLE)	!= 0;

	//前フレームから状態が変わったか
	changePrevInputState(MOUSE_BUTTON::LEFT, isPressedLeftButton);
	changePrevInputState(MOUSE_BUTTON::RIGHT, isPressedRightButton);
	changePrevInputState(MOUSE_BUTTON::MIDDLE, isPressedMiddleButton);
}

void Mouse::changePrevInputState(int buttonCode, bool isPressed)
{
	if (buttonState_[buttonCode].onPressed_ != isPressed)
	{
		buttonState_[buttonCode].onPressed_ = isPressed;
		buttonState_[buttonCode].frame_ = elapsedTimeCounter_;

		if (isPressed)
		{
			//押した瞬間の時刻を記録（長押し判定に使用）
			buttonState_[buttonCode].time_ = GetNowCount();
		}
	}
}

const bool Mouse::getButtonPressedMoment(int buttonCode) noexcept
{
	auto curretFrame = elapsedTimeCounter_;
	return buttonState_[buttonCode].onPressed_ && buttonState_[buttonCode].frame_ == curretFrame;
}

const bool Mouse::getButtonHoldNow(int buttonCode, float seconds) noexcept
{
	auto currentTime = GetNowCount();
	return buttonState_[buttonCode].onPressed_ && (currentTime - buttonState_[buttonCode].time_) >= seconds;
}

const bool Mouse::getButtonReleasedMoment(int buttonCode) noexcept
{
	auto curretFrame = elapsedTimeCounter_;
	return !buttonState_[buttonCode].onPressed_ && buttonState_[buttonCode].frame_ == curretFrame;
}

const VECTOR Mouse::getCursorPosButtonPressedMoment(int buttonCode) noexcept
{
	//失敗
	if (!getButtonPressedMoment(buttonCode)) return VGet(-1.0f, -1.0f, -1.0f);

	return  cursorPosition_;
}

const VECTOR Mouse::getCursorPosButtonHoldNow(int buttonCode, float seconds) noexcept
{
	//失敗
	if (!getButtonHoldNow(buttonCode, seconds)) return VGet(-1.0f, -1.0f, -1.0f);

	return  cursorPosition_;
}

const VECTOR Mouse::getCursorPosReleasedHoldMoment(int buttonCode) noexcept
{
	//失敗
	if (!getButtonReleasedMoment(buttonCode)) return VGet(-1.0f, -1.0f, -1.0f);

	return  cursorPosition_;
}