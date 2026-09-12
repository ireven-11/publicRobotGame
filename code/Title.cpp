#include"DxLibForIreven.h"
#include"TitleUIPresenter.h"
#include"TitleSoundManager.h"
#include"Title.h"
#include"InputHandler.h"
#include"DInput.h"
#include"game.h"

Title::Title(std::shared_ptr<SceneContext>& context) 
	: SceneBase(context),
	ui_(std::make_unique<TitleUIPresenter>()),
	soundManager_(std::make_unique<TitleSoundManager>())
{
	SetFontSize(75);
	backGraph_	= LoadGraph("graph/titleBackGraph.png");
	logo_		= LoadGraph("graph/titleLogo.png");

	screenHandle_	= MakeScreen(init_screen_width, init_screen_height);
	movieHandle_	= LoadGraph("movie/demo1.mp4");

	init();
}

Title::~Title()
{
	DeleteGraph(backGraph_);
	DeleteGraph(screenHandle_);
	DeleteGraph(movieHandle_);
	DeleteGraph(logo_);

	ui_				= nullptr;
	soundManager_	= nullptr;
}

void Title::init()
{
	soundManager_->init();
}

void Title::update()
{
	ui_->update();

	if (!ui_->getIsMovingSelector()) return;
	soundManager_->start2DSound("selectButton", false, true);
}

void Title::draw()
{
	drawMovie();
	DrawExtendGraph(0, 0, init_screen_width, init_screen_height, backGraph_, true);
	DrawExtendGraphF(logo_position.x, logo_position.y, 
		logo_position.x + logo_widht, logo_position.y + logo_height, logo_, true);

	ui_->draw();
}

void Title::proceed()
{
	if (InputHandler::instance().getDInput()->getButtonReleasedMoment(PROCON::BUTTON::A) && ui_->getSelectedButtonIndex() == 0)
	{
		soundManager_->start2DSound("decide");
		PlayMovie("movie/transition.mp4", 1.0, DX_MOVIEPLAYTYPE_NORMAL);

		Game::instance().proceedToBattle();
	}
}

void Title::enter()
{
	init();
}

void Title::exit()
{
	soundManager_->stop2DSound("bgm");
}

void Title::drawMovie()
{
	//スクリーンハンドルに動画を描画する
	SetDrawScreen(screenHandle_);
	PlayMovieToGraph(movieHandle_, DX_PLAYTYPE_LOOP);
	DrawExtendGraph(0, 0, init_screen_width, init_screen_height, movieHandle_, TRUE);

	//元のスクリーンハンドルに戻す
	SetDrawScreen(DX_SCREEN_BACK);
	DrawExtendGraph(movie_position.x, movie_position.y,
		movie_position.x + movie_widht * movie_scale, movie_position.y + movie_height * movie_scale, screenHandle_, TRUE);
}