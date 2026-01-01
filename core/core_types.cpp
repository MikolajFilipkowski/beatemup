#include "core_types.h"
#include "managers/managers.h"

#include "cstring"

GameObject::GameObject(Managers* mgs, Transform tr) 
	: mgs(mgs),
	transform(tr),
	rb({0,0,0}),
	removalPending(false),
	hasStarted(false)
{
	if (mgs->object != nullptr)
		mgs->object->add((GameObject*)this);
}

GameObject::~GameObject() {}

Transform& GameObject::getTransform()
{
	return transform;
}

void GameObject::setTransform(Transform tr)
{
	transform = tr;
}

Vector3& GameObject::getPosition()
{
	return transform.pos;
}

void GameObject::setPosition(Vector3 pos)
{
	transform.pos = pos;
}

Rigidbody& GameObject::getRb()
{
	return rb;
}

bool GameObject::getRemovalFlag() const
{
	return removalPending;
}

void GameObject::setRemovalFlag(bool flag)
{
	removalPending = true;
}

bool GameObject::getStartedFlag() const
{
	return hasStarted;
}

void GameObject::setStartedFlag(bool flag)
{
	hasStarted = flag;
}

Camera::Camera(Managers* mgs, Vector3 pos) : GameObject(mgs) {
	transform.pos = pos;
}

Camera::~Camera() {}

Rect Camera::getViewport(Dims logDims)
{
	return { transform.pos.x, transform.pos.y, (float)logDims.width, (float)logDims.height};
}

float Camera::getZoom()
{
	float pos_z = (transform.pos.z > 1.0f) ? transform.pos.z : 1.0f;

	return DEFAULT_CAM_Z / pos_z;
}

SpriteObject::SpriteObject(Managers* mgs, Transform tr, const char* key) 
	: GameObject(mgs, tr), spriteKey(copy_string(key))
{

}

SpriteObject::~SpriteObject() {
	delete[] spriteKey;
	spriteKey = nullptr;
}

Sprite* SpriteObject::getSprite() {
	return mgs->sprite->get(spriteKey);
}

void SpriteObject::setSpriteKey(const char* key) {
	delete[] spriteKey;
	spriteKey = copy_string(key);
}

AnimatableObject::AnimatableObject(Managers* mgs, Transform tr)
	: GameObject(mgs, tr), currentAnimKey(nullptr), currentAnimFrame(0), currentAnimTimer(0.0f)
{
	
}

AnimatableObject::~AnimatableObject() {
	delete[] currentAnimKey;
	currentAnimKey = nullptr;
}

char* AnimatableObject::getCurrentAnimKey()
{
	return currentAnimKey;
}

void AnimatableObject::setAnim(const char* key)
{
	if (key == nullptr) return;
	if (currentAnimKey != nullptr && strcmp(key, currentAnimKey) == 0) return;

	delete[] currentAnimKey;
	currentAnimKey = copy_string(key);
	currentAnimFrame = 0;
	currentAnimTimer = 0.0f;
}

void AnimatableObject::updateAnim(float dt)
{
	AnimationClip* clip = mgs->anim->get(currentAnimKey);
	if (clip == nullptr) return;

	currentAnimTimer += dt;
	if (currentAnimTimer >= clip->frameDuration) {
		currentAnimTimer = 0;
		currentAnimFrame++;
		currentAnimFrame %= clip->frameCount;
	}
}
