#pragma once
#include<memory>
#include"Singleton.h"

class XInput;
class DInput;
class Mouse;
class KeyInput;
class RightAttackEvent;
class LeftAttackEvent;
class RaiseEvent;
class DescentEvent;
class CameraEvent;
class DashEvent;
class WalkEvent;

class InputHandler : public Singleton<InputHandler>
{
public:
	~InputHandler();

	void update();
	void init();

	const std::shared_ptr<XInput> getXInput()const noexcept { return xInput_; }
	const std::shared_ptr<DInput> getDInput()const noexcept { return dInput_; }
	const std::shared_ptr<Mouse> getMouse()const noexcept { return mouse_; }
	const std::shared_ptr<KeyInput> getKeyInput()const noexcept { return keyInput_; }

	const std::shared_ptr<RightAttackEvent> getRightAttackEvent()const noexcept { return rightAttackEvent_; }
	const std::shared_ptr<LeftAttackEvent> getLeftAttackEvent()const noexcept { return leftAttackEvent_; }
	const std::shared_ptr<RaiseEvent> getRaiseEvent()const noexcept { return raiseEvent_; }
	const std::shared_ptr<DescentEvent> getDescentEvent()const noexcept { return descentEvent_; }
	const std::shared_ptr<CameraEvent> getCameraEvent()const noexcept { return cameraEvent_; }
	const std::shared_ptr<DashEvent> getDashEvent()const noexcept { return dashEvent_; }
	const std::shared_ptr<WalkEvent> getWalkEvent()const noexcept { return walkEvent_; }

private:
	//Singletonのフレンドに宣言してコンストラクタにアクセス許可
	friend class Singleton<InputHandler>;
	InputHandler();

	bool warnNonConnectingController();

	std::shared_ptr<XInput>		xInput_;
	std::shared_ptr<DInput>		dInput_;
	std::shared_ptr<Mouse>		mouse_;
	std::shared_ptr<KeyInput>	keyInput_;
	int controllerType_;
	std::shared_ptr<int> controllerTypePointer_;

	std::shared_ptr<RightAttackEvent> rightAttackEvent_;
	std::shared_ptr<LeftAttackEvent> leftAttackEvent_;
	std::shared_ptr<RaiseEvent> raiseEvent_;
	std::shared_ptr<DescentEvent> descentEvent_;
	std::shared_ptr<CameraEvent> cameraEvent_;
	std::shared_ptr<DashEvent> dashEvent_;
	std::shared_ptr<WalkEvent> walkEvent_;
};