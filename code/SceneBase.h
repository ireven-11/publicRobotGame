#pragma once
#include<memory>
#include"IScene.h"

enum class SceneState
{
	NORMAL,
	EFFECT
};

//サンドボックスパターン
//テンプレートにして色々なコンテキストに対応する
template<typename ContextT>
class SceneBase : IScene
{
public:
	SceneBase(std::shared_ptr<ContextT>& context)
		: context_(context)
	{

	}
	virtual ~SceneBase()	= default;

	void changeState(const SceneState nextState) { currentState_ = nextState; }

	virtual void init()		= 0;
	virtual void update()	= 0;
	virtual void draw()		= 0;
	virtual void proceed()	= 0;
	virtual void enter()	= 0;
	virtual void exit()		= 0;

protected:
	//コンテキストを継承先で呼び出せるようにしてさらにコンテキストからオブジェクトを呼ぶ
	std::shared_ptr<ContextT> context() { return context_; }

	SceneState currentState_;

private:
	std::shared_ptr<ContextT> context_;
};
