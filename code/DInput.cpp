#include"DxLibForIreven.h"
#include"InputHandler.h"
#include"DInput.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="controllerNumber">コントローラーが何番目につながってるかの数値</param>
DInput::DInput(int controllerNumber)
{
	input_ = {};
	SetJoypadDeadZone(controllerNumber, deadZone_);

	//TODO:処理に無駄があるのでinputHandlerを使っていい感じに処理をまとめたい
	controllerNumber_	= controllerNumber;
	controllerType_		= GetJoypadType(controllerNumber_);
}

DInput::~DInput()
{
}

void DInput::initProcon()
{
	elapsedTimeCounter_ = 0;

	buttonState_.clear();
	stickState_.clear();

	for (int i = 0; i < PROCON::button_buffer_size; i++)
	{
		buttonState_.emplace_back();
		buttonState_[i].frame_		= 0;
		buttonState_[i].time_		= 0;
		buttonState_[i].onPressed_	= false;
	}

	switch (controllerType_)
	{
		case DX_PADTYPE_SWITCH_PRO_CTRL:
		{
			stickState_.emplace(PROCON::STICK::RIGHT_X, DPadStickState());
			stickState_.emplace(PROCON::STICK::RIGHT_Y, DPadStickState());
			stickState_.emplace(PROCON::STICK::LEFT_X, DPadStickState());
			stickState_.emplace(PROCON::STICK::LEFT_Y, DPadStickState());

			break;
		}
		default:
		{

		}
	}
}

void DInput::update()
{
	//コントローラーが繋がってなかったら-1が帰る。dxlibが悪いです。
	bool isConnected = GetJoypadDirectInputState(controllerNumber_, &input_) != -1;
	if (!isConnected) return;

	//経過時間をカウント
	++elapsedTimeCounter_;

	switch (controllerType_)
	{
		case DX_PADTYPE_SWITCH_PRO_CTRL:
		{
			updateProcon();

			break;
		}
		default:
		{

		}
	}
}

void DInput::updateProcon()
{
	updateProconButton();

	updateProconStick();
}

void DInput::updateProconButton()
{
	for (int i = 0; i < PROCON::button_buffer_size; i++)
	{
		//今フレームのキーの状態
		bool isPressed = true;
		if (i <= PROCON::BUTTON::CAPUTURE) //input.button配列のやつだったら
		{
			//入力が0以外の時にtrueが入る
			isPressed = input_.Buttons[i] != 0;
		}
		else
		{
			//入力が-1以外の時にtrueが入る
			isPressed = input_.POV[0] != -1;
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

void DInput::updateProconStick()
{
	//スティックの値をmapにあうように保存
	std::vector<int> stickValues;
	stickValues.emplace_back(input_.Rx);
	stickValues.emplace_back(input_.Ry);
	stickValues.emplace_back(input_.X);
	stickValues.emplace_back(input_.Y);

	for (int i = 0; i < stickState_.size(); i++)
	{
		//今フレームのキーの状態
		bool isTilted = true;

		//入力が0以外の時にtrueが入る
		isTilted = stickValues[i] != 0;

		//前フレームから状態が変わったか
		if (stickState_[i].onTiled_ != isTilted)
		{
			stickState_[i].onTiled_ = isTilted;
			stickState_[i].frame_	= elapsedTimeCounter_;

			if (isTilted)
			{
				//押した瞬間の時刻を記録（長押し判定に使用）
				stickState_[i].time_ = GetNowCount();
			}
		}
	}
}

//todo：引数の値が範囲外のときになんとかする処理を書かないとバグの原因になる
const bool DInput::getButtonPressedMoment(int buttonCode) noexcept
{
	auto curretFrame = elapsedTimeCounter_;
	return buttonState_[buttonCode].onPressed_ && buttonState_[buttonCode].frame_ == curretFrame;
}

//todo：引数の値が範囲外のときになんとかする処理を書かないとバグの原因になる
const bool DInput::getButtonHoldNow(int buttonCode, float seconds) noexcept
{
	auto currentTime = GetNowCount();
	return buttonState_[buttonCode].onPressed_ && (currentTime - buttonState_[buttonCode].time_) >= seconds;
}

//todo：引数の値が範囲外のときになんとかする処理を書かないとバグの原因になる
const bool DInput::getButtonReleasedMoment(int buttonCode) noexcept
{
	auto curretFrame = elapsedTimeCounter_;
	return !buttonState_[buttonCode].onPressed_ && buttonState_[buttonCode].frame_ == curretFrame;
}

//todo：引数の値が範囲外のときになんとかする処理を書かないとバグの原因になる
const int DInput::getProconStickXY(int stickCode)const noexcept
{
	switch (stickCode)
	{
		case PROCON::STICK::LEFT_X:
		{
			return input_.X;
		}
		case PROCON::STICK::LEFT_Y:
		{
			return input_.Y;
		}
		case PROCON::STICK::RIGHT_X:
		{
			return input_.Rx;
		}
		case PROCON::STICK::RIGHT_Y:
		{
			return input_.Ry;
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
const double DInput::getProconStickAngle(int stickCode)const noexcept
{
	switch (stickCode)
	{
		case PROCON::STICK::LEFT_ANGLE:
		{
			double tiltAngle_ = atan2(static_cast<double>(input_.Y), static_cast<double>(input_.X));
			return tiltAngle_;
		}
		case PROCON::STICK::RIGHT_ANGLE:
		{
			double tiltAngle_ = atan2(static_cast<double>(input_.Ry), static_cast<double>(input_.Rx));
			return tiltAngle_;
		}
	}

	return 0.0;
}

const float	DInput::getProconStickDistance()const noexcept
{
	VECTOR stickPos = VGet(static_cast<float>(getProconStickXY(PROCON::STICK::LEFT_X)), static_cast<float>(getProconStickXY(PROCON::STICK::LEFT_Y)), 0);
	return CalculateDistance<float>(stickPos);
}

const bool DInput::getStickTiledMoment(const int stickCode) noexcept
{
	auto curretFrame = elapsedTimeCounter_;
	const bool test = stickState_[stickCode].onTiled_ && stickState_[stickCode].frame_ == curretFrame;
	return stickState_[stickCode].onTiled_ && stickState_[stickCode].frame_ == curretFrame;
}

const bool DInput::getStickHoldNow(const int stickCode, const float seconds) noexcept
{
	auto currentTime = GetNowCount();
	return stickState_[stickCode].onTiled_ && (currentTime - stickState_[stickCode].time_) >= seconds;
}

const bool DInput::getStickReleasedMoment(const int stickCode) noexcept
{
	auto curretFrame = elapsedTimeCounter_;
	return !stickState_[stickCode].onTiled_ && stickState_[stickCode].frame_ == curretFrame;
}