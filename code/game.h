#pragma once
#include"Singleton.h"
#include"SceneBase.h"
#include"SceneContext.h"
#include<memory>

class FPS;
class Title;
class Battle;

class Game : public Singleton<Game>
{
public:
	~Game();

	void update();
	void proceedToBattle();
	void proceedToTitle();

private:
	//Singletonのフレンドに宣言してコンストラクタにアクセス許可
	friend class Singleton<Game>;
	Game();

	std::unique_ptr<FPS> fps_;
	std::shared_ptr<SceneBase<SceneContext>> currentScene_;
	std::shared_ptr<SceneContext> context_;
	std::shared_ptr<Title> titleScene_;
	std::shared_ptr<Battle> battleScene_;
};