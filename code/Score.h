#pragma once

class Score
{
public:
	Score();
	~Score();

	void init();
	void draw()const;

	const int getCurrentScore()const noexcept { return currentScore_; }

private:
	void addObjectScore();
	void decreaseObjectScore();

	int currentScore_;

	const int object_score = 100;
};