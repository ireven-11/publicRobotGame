#pragma once

constexpr int	key_buffer_size	= 256;	//キーの総数

struct KeyState
{
	int		frame_;
	int		time_;
	bool	onPressed_;
}; 

class KeyInput
{
	//アップデートをInputHandler以外で呼べないようにする
	friend class InputHandler;

public:
	KeyInput();
	~KeyInput();

	const bool getKeyPressedMoment(const int keyCode) noexcept;
	const bool getKeyHoldNow(const int keyCode, const int seconds) noexcept;
	const bool getKeyReleasedMoment(const int keyCode) noexcept;
	
private:
	void update();
	void init();

	KeyState		keyState_[key_buffer_size];	//キーの状態
	unsigned int	elapsedTimeCounter_;
};