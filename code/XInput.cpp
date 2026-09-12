#include"DxLib.h"
#include"InputHandler.h"
#include"XInput.h"
#include<cmath>

XInput::XInput(int controllerNumber)
{
	SetJoypadDeadZone(controllerNumber, deadZone_);
	controllerNumber_ = controllerNumber;
}

XInput::~XInput()
{
}

void XInput::init()
{
	elapsedTimeCounter_ = 0;

	for (int i = 0; i < xbox_button_buffer_size; i++)
	{
		buttonState_[i].frame_		= 0;
		buttonState_[i].time_		= 0;
		buttonState_[i].onPressed_	= false;
	}
}

void XInput::update()
{
	//コントローラーが繋がってなかったら-1が帰る。dxlibが悪いです。
	bool isConnected = GetJoypadXInputState(controllerNumber_, &input_) != -1;
	if (!isConnected) return;

	for (int i = 0; i < xbox_button_buffer_size; i++)
	{
		//今フレームのキーの状態
		bool isPressed = true;
		if (i < button_buffer_size_except_trigger) //input.button配列のやつだったら
		{
			//入力が0以外の時にtrueが入る
			isPressed = input_.Buttons[i] != 0;
		}
		else
		{
			//左右トリガーボタン
			if (i == XBOX::BUTTON::LEFT_TRIGGER)
			{
				isPressed = input_.LeftTrigger != 0;
			}
			else
			{
				isPressed = input_.RightTrigger != 0;
			}
		}

		//前フレームから状態が変わったか
		if (buttonState_[i].onPressed_ != isPressed)
		{
			buttonState_[i].onPressed_	= isPressed;
			buttonState_[i].frame_		= elapsedTimeCounter_;

			if (isPressed)
			{
				//押した瞬間の時刻を記録（長押し判定に使用）
				buttonState_[i].time_ = GetNowCount();
			}
		}
	}
}

//todo：引数の値が範囲外のときになんとかする処理を書かないとバグの原因になる
const bool XInput::getButtonPressedMoment(int buttonCode) noexcept
{
	auto curretFrame = elapsedTimeCounter_;
	return buttonState_[buttonCode].onPressed_ && buttonState_[buttonCode].frame_ == curretFrame;
}

//todo：引数の値が範囲外のときになんとかする処理を書かないとバグの原因になる
const bool XInput::getButtonHoldNow(int buttonCode, float seconds) noexcept
{
	auto currentTime = GetNowCount();
	return buttonState_[buttonCode].onPressed_ && (currentTime - buttonState_[buttonCode].time_) >= seconds;
}

//todo：引数の値が範囲外のときになんとかする処理を書かないとバグの原因になる
const bool XInput::getButtonReleasedMoment(int buttonCode) noexcept
{
	auto curretFrame = elapsedTimeCounter_;
	return !buttonState_[buttonCode].onPressed_ && buttonState_[buttonCode].frame_ == curretFrame;
}

//todo：引数の値が範囲外のときになんとかする処理を書かないとバグの原因になる
const int XInput::getStickXY(int stickCode)const noexcept
{
	switch (stickCode)
	{
		case XBOX::STICK::LEFT_X:
		{
			return input_.ThumbLX;
		}
		case XBOX::STICK::LEFT_Y:
		{
			return input_.ThumbLY;
		}
		case XBOX::STICK::RIGHT_X:
		{
			return input_.ThumbRX;
		}
		case XBOX::STICK::RIGHT_Y:
		{
			return input_.ThumbRY;
		}
	}

	return -1;
}

//todo：引数の値が範囲外のときになんとかする処理を書かないとバグの原因になる
/// <summary>
/// 現在のスティックの角度を取得する
/// </summary>
/// <param name="stickCode">どのスティックかを表すコード</param>
/// <returns>「-PI～0～Pi」の値を返す</returns>
const double XInput::getStickAngle(int stickCode)const noexcept
{
	switch (stickCode)
	{
		case XBOX::STICK::LEFT_ANGLE:
		{
			double tiltAngle_ = atan2(static_cast<double>(input_.ThumbLY), static_cast<double>(input_.ThumbLX));
			return tiltAngle_;
		}
		case XBOX::STICK::RIGHT_ANGLE:
		{
			double tiltAngle_ = atan2(static_cast<double>(input_.ThumbRY), static_cast<double>(input_.ThumbRX));
			return tiltAngle_;
		}
	}

	return -1.0;
}

const bool XInput::getIsTiltingStick(bool isRight)const noexcept
{
	bool isTilting = false;
	if (isRight)
	{
		isTilting = input_.ThumbRX != 0 || input_.ThumbRY != 0;
	}
	else
	{
		isTilting = input_.ThumbLX != 0 || input_.ThumbLY != 0;
	}

	return isTilting;
}