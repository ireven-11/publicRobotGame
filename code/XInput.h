#pragma once

constexpr char xbox_button_buffer_size = 18;

struct XPadButtonState
{
	int		frame_;
	int		time_;
	bool	onPressed_;
};

namespace XBOX
{
	enum BUTTON
	{
		LEFT_TRIGGER	= 16,
		RIGHT_TRIGGER	= 17
	};

	enum STICK
	{
		LEFT_X,
		LEFT_Y,
		LEFT_ANGLE,
		RIGHT_X,
		RIGHT_Y,
		RIGHT_ANGLE
	};
}

//TODO:コントローラーごとに処理を分割する
class XInput
{
	//アップデートをInputHandler以外で呼べないようにする
	friend class InputHandler;

public:
	XInput(int controllerNumber);
	~XInput();

	const bool		getButtonPressedMoment(const int buttonCode) noexcept;
	const bool		getButtonHoldNow(const int buttonCode, const float seconds) noexcept;
	const bool		getButtonReleasedMoment(const int buttonCode) noexcept;
	const int		getStickXY(const int stickCode)const noexcept;
	const double	getStickAngle(const int stickCode)const noexcept;
	const bool		getIsTiltingStick(const bool isRight)const noexcept;

private:
	void update();
	void init();

	XINPUT_STATE	input_;
	XPadButtonState	buttonState_[xbox_button_buffer_size];
	int				controllerNumber_;
	unsigned int	elapsedTimeCounter_;

	const float deadZone_							= 0.2f;
	const float button_buffer_size_except_trigger	= 16;
};