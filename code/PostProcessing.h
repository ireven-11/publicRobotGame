#pragma once
#include<string>

enum class PostEffectType
{
	NONE,
	GRAPH,
	GAUSS
};

class PostProcessing
{
public:
	PostProcessing(const std::string& graphPath);
	~PostProcessing();

	void init();
	void update();
	void start(const PostEffectType type, const float addLerpTimeValue = 0.1f);
	void finish();
	void draw();

private:
	int				graphHandle_;
	int				screenHandle_;
	float			alphaLerpTime_;
	float			addLerpTimeValue_;
	PostEffectType	currentType_;

	const float max_alpha_lerp_time = 1.0f;
};