#pragma once
#include<memory>
#include"Player.h"
#include"CameraBrain.h"
#include"InputHandler.h"
#include"PlayerStatus.h"

//playerなどの色々なシーンで使用するかもしれないオブジェクトを持つクラス
//オブジェクトを呼び出すことはできるが変更はできない
class SceneContext final
{
public:
	SceneContext()
	{
		player_			= std::make_shared<Player>();
		cameraBrain_	= std::make_shared<CameraBrain>(std::dynamic_pointer_cast<PlayerStatus>(player_->getStatus()));
	}
	~SceneContext()
	{
		player_			= nullptr;
		cameraBrain_	= nullptr;
	}
	std::shared_ptr<Player> getPlayer()const noexcept { return player_; }
	std::shared_ptr<CameraBrain> getCameraBrain()const noexcept { return cameraBrain_; }

private:
	std::shared_ptr<Player> player_;
	std::shared_ptr<CameraBrain> cameraBrain_;
};