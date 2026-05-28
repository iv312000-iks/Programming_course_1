#pragma once
#include<SDL3/SDL.h>
#include "structs.h"

class Animation
{
public:
	Animation();
	~Animation();
	void showAnimation(const animationProperties& animation, SDL_FRect& src, int sizeSprite);
	bool update(const animationProperties& animation, SDL_FRect& stc, int sizesprite);
	void reset();
	void start(const animationProperties& animation, SDL_FRect& stc, int sizesprite);
	bool isPlaying() const { return isPlaying_; }
	int getCurrentFrame() const { return currentIndx; }

private:
	int currentIndx;
	Uint64 lastUpdate;
	bool isPlaying_;
	int totalFrames;
};