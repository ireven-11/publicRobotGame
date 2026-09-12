#include"DxLib.h"
#include"Score.h"
#include"DestroyObjectEvent.h"
#include"ColliderEventHander.h"

Score::Score()
{
	init();

	ColliderEventHander::instance().getDestroyObjectEvent()->addStartingEvent([this] {addObjectScore(); });
}

Score::~Score()
{
}

void Score::init()
{
	currentScore_ = 0.0f;
}

void Score::addObjectScore()
{
	currentScore_ += object_score;
}

void Score::decreaseObjectScore()
{
	currentScore_ -= object_score;
}

void Score::draw()const
{
	DrawFormatString(1500, 0, GetColor(255, 25, 25), "SCORE:%d", currentScore_);	//test
}