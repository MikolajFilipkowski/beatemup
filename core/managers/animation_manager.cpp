#include "animation_manager.h"

#include "managers.h"
#include <cstdio>

AnimationManager::~AnimationManager()
{
}

void AnimationManager::destroy()
{
}

void AnimationManager::createFromSheet(int a_AnimKey, int a_SpriteKey, int a_FrameCount, float a_FrameDuration) {
	Sprite* sprite = m_Mgs->sprite->get(a_SpriteKey);
	if (sprite == nullptr) return;

	AnimationClip* clip = new AnimationClip();
	clip->frameCount = a_FrameCount;
	clip->frameDuration = a_FrameDuration;
	clip->frames = new SDL_Rect[a_FrameCount];
	clip->spriteKey = a_SpriteKey;

	int frameW = sprite->width / a_FrameCount;
	int frameH = sprite->height;

	for (int i = 0; i < a_FrameCount; i++) {
		clip->frames[i] = { frameW * i, 0, frameW, frameH };
	}

	m_AnimMap.put(a_AnimKey, clip);
}

AnimationClip* AnimationManager::get(int a_Key)
{
	if (!m_AnimMap.containsKey(a_Key))
		return nullptr;
	return m_AnimMap.get(a_Key);
}

void AnimationManager::remove(int a_Key) {
	AnimationClip*& clip = m_AnimMap.get(a_Key);
	delete[] clip->frames;
	delete clip;
	clip = nullptr;
}