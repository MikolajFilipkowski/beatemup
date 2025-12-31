#include "animation_manager.h"

#include "managers.h"
#include <cstdio>

AnimationManager::~AnimationManager()
{
}

void AnimationManager::destroy()
{
}

void AnimationManager::createFromSheet(const char* animKey, const char* spriteKey, int frameCount, float frameDuration) {
	Sprite* sprite = mgs->sprite->get(spriteKey);
	if (sprite == nullptr) return;

	AnimationClip* clip = new AnimationClip();
	clip->frameCount = frameCount;
	clip->frameDuration = frameDuration;
	clip->frames = new SDL_Rect[frameCount];
	clip->spriteKey = copy_string(spriteKey);

	int frameW = sprite->width / frameCount;
	int frameH = sprite->height;

	for (int i = 0; i < frameCount; i++) {
		clip->frames[i] = { frameW * i, 0, frameW, frameH };
	}

	animMap.put(animKey, clip);
}

AnimationClip*& AnimationManager::get(const char* key)
{
	return animMap.get(key);
}

void AnimationManager::remove(const char* key) {
	AnimationClip*& clip = animMap.get(key);
	delete[] clip->frames;
	delete clip;
	clip = nullptr;
}