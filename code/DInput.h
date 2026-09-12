#pragma once
#include<vector>
#include<map>

struct DPadButtonState
{
	int		frame_;
	int		time_;
	bool	onPressed_;
};

struct DPadStickState
{
	int		frame_;
	int		time_;
	bool	onTiled_;
};

namespace PROCON
{
	constexpr char button_buffer_size	= 18;		//ボタンの総数
	constexpr char stick_buffer_size	= 2 * 2;	//スティックの総数×縦横（XY）

	//暗黙の型変換をしたいのでenumClassにはしない
	enum BUTTON
	{
	B,
	A,
	Y,
	X,
	L,
	R,
	ZL,
	ZR,
	MINUS,
	PLUS,
	LEFTSTICK_PRESS,
	RIGHTSTICK_PRESS,
	HOME,
	CAPUTURE,

	//下はDinputのinput.POVのやつ
	UP_CROSS,
	RIGHT_CROSS,
	DOWN_CROSS,
	LEFT_CROSS,
	};

	//暗黙の型変換をしたいのでenumClassにはしない
	enum STICK
	{
		RIGHT_X,
		RIGHT_Y,
		LEFT_X,
		LEFT_Y,
		RIGHT_ANGLE,
		LEFT_ANGLE,
	};
}

//TODO:コントローラーの種類ごとに処理を分離する
class DInput
{
	//アップデートをInputHandler以外で呼べないようにする
	friend class InputHandler;

public:
	DInput(int controllerNumber);
	~DInput();

	const bool		getButtonPressedMoment(const int buttonCode) noexcept;
	const bool		getButtonHoldNow(const int buttonCode, const float seconds) noexcept;
	const bool		getButtonReleasedMoment(const int buttonCode) noexcept;
	const int		getProconStickXY(const int stickCode)const noexcept;
	const double	getProconStickAngle(const int stickCode)const noexcept;
	const float		getProconStickDistance()const noexcept;
	const bool		getStickTiledMoment(const int stickCode) noexcept;
	const bool		getStickHoldNow(const int stickCode, const float seconds) noexcept;
	const bool		getStickReleasedMoment(const int stickCode) noexcept;

private:
	void update();
	void updateProconButton();
	void updateProconStick();
	void updateProcon();
	void initProcon();
	
	DINPUT_JOYSTATE					input_;
	std::vector<DPadButtonState>	buttonState_;
	std::map<int, DPadStickState>	stickState_;
	int								controllerType_;
	int								controllerNumber_;
	unsigned int					elapsedTimeCounter_;

	const float deadZone_ = 0.2f;
};