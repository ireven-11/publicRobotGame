#pragma once
#include<memory>
#include"SceneBase.h"
#include"SceneContext.h"

class TitleUIPresenter;
class TitleSoundManager;

class Title : public SceneBase<SceneContext>
{
public:
	Title(std::shared_ptr<SceneContext>& context);
	~Title();

	void init();
	void update();
	void draw();
	void enter();
	void exit();

private:
	void drawMovie();
	void proceed();

	int backGraph_;
	int screenHandle_;
	int movieHandle_;
	int logo_;
	std::unique_ptr<TitleUIPresenter> ui_;
	std::unique_ptr<TitleSoundManager> soundManager_;

	const VECTOR logo_position	= VGet(950.0f, 100.0f, 0.0f);
	const float logo_widht		= 800.0f;
	const float logo_height		= 550.0f;
	const float movie_widht		= 1920.0f;
	const float movie_height	= 1080.0f;
	const float movie_scale		= 0.5f;
	const VECTOR movie_position = VGet(150.0f, 200.0f, 0.0f);
};