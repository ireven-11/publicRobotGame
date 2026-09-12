#include"DxLib.h"
#include"InputHandler.h"
#include"KeyInput.h"

KeyInput::KeyInput()
{
	init();
}

KeyInput::~KeyInput()
{
}

void KeyInput::init()
{
	elapsedTimeCounter_ = 0;

	for (int i = 0; i < key_buffer_size; i++)
	{
		keyState_[i].frame_		= 0;
		keyState_[i].time_		= 0;
		keyState_[i].onPressed_	= false;
	}
}

void KeyInput::update()
{
	++elapsedTimeCounter_;

	//すべてのキーの現在の状態を取得
	char allKeys[key_buffer_size];
	GetHitKeyStateAll(allKeys);

	for (int i = 0; i < key_buffer_size; i++)
	{
		//今フレームのキーの状態
		bool isPressed = allKeys[i] != 0;

		//前フレームから状態が変わったか
		if (keyState_[i].onPressed_ != isPressed)
		{
			keyState_[i].onPressed_ = isPressed;
			keyState_[i].frame_		= elapsedTimeCounter_;

			if (isPressed)
			{
				//押した瞬間の時刻を記録（長押し判定に使用）
				keyState_[i].time_ = GetNowCount();
			}
		}
	}
}

/// <summary>
/// 押した瞬間かどうかを判定
/// </summary>
/// <param name="keyCode">キーコード(dxlibのキーコードを参照。リファレンスに書いてる)</param>
/// <returns></returns>
const bool KeyInput::getKeyPressedMoment(int keyCode) noexcept
{
	auto curretFrame = elapsedTimeCounter_;
	return keyState_[keyCode].onPressed_ && keyState_[keyCode].frame_ == curretFrame;
}

/// <summary>
/// 長押ししてるかの判定
/// </summary>
/// <param name="keyCode">キーコード(dxlibのキーコードを参照。リファレンスに書いてる)</param>
/// <param name="seconds">長押し判定になるまでの時間(秒)。1000で1秒</param>
/// <returns></returns>
const bool KeyInput::getKeyHoldNow(int keyCode, int seconds) noexcept
{
	auto currentTime = GetNowCount();
	return keyState_[keyCode].onPressed_ && (currentTime - keyState_[keyCode].time_) >= seconds;
}

/// <summary>
/// 離した瞬間かどうかを判定
/// </summary>
/// <param name="keyCode">キーコード(dxlibのキーコードを参照。リファレンスに書いてる)</param>
/// <returns></returns>
const bool KeyInput::getKeyReleasedMoment(int keyCode) noexcept
{
	auto curretFrame = elapsedTimeCounter_;
	return !keyState_[keyCode].onPressed_ && keyState_[keyCode].frame_ == curretFrame;
}