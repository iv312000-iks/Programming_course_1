#include "Animation.h"

Animation::Animation() : currentIndx(0), lastUpdate(0), isPlaying_(false), totalFrames(0)
{
}

Animation::~Animation()
{
}

void Animation::showAnimation(const animationProperties& animation, SDL_FRect& src, int sizeSprite) {
	Uint64 now = SDL_GetTicks();
	Uint64 delay = now - lastUpdate;

	if (delay >= (Uint64)animation.animationDelay) {
		lastUpdate = now;
		currentIndx = (currentIndx + 1) % animation.frames;

		src.x = currentIndx * sizeSprite;
		src.y = animation.y;
	}
}

bool Animation::update(const animationProperties& animation, SDL_FRect& src, int sizeSprite) {
	if (!isPlaying_) {
		return true;
	}

	Uint64 now = SDL_GetTicks();
	Uint64 delay = now - lastUpdate;

	if (delay >= (Uint64)animation.animationDelay) {
		lastUpdate = now;
		currentIndx++;

		if (currentIndx >= animation.frames) {
			isPlaying_ = false;
			currentIndx = 0;
			return true;
		}

		src.x = currentIndx * sizeSprite;
		src.y = animation.y;
	}
	return false;
}


void Animation::start(const animationProperties& animation, SDL_FRect& src, int sizeSprite) {
	currentIndx = 0;
	lastUpdate = SDL_GetTicks();
	isPlaying_ = true;
	totalFrames = animation.frames;

	src.x = 0;
	src.y = animation.y;
}


void Animation::reset() {
	currentIndx = 0;
	lastUpdate = 0;
	isPlaying_ = false;
}