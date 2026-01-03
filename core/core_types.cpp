#include "math.h"
#include "core_types.h"
#include "managers/managers.h"

#include "cstring"


void GameScene::addLayer(int spriteKey, float scrollFactor, float width)
{
	int copies = (mgs->display->getLogWidth() / (int)width) + 2;
	layers.add({spriteKey, scrollFactor, width, copies});
}

void GameScene::drawBackground()
{
	float camX = mgs->display->getActiveCamera()->getPosition().x;
	Dims logDims = mgs->display->getLogDims();

	for (auto& layer : layers) {
		drawLayer(layer, camX, logDims);
	}
}

void GameScene::drawLayer(BackgroundLayer& layer, float camX, Dims& logDims)
{
	// x - pozycja layera (<=0)
	float x = fmodf( - camX * layer.scrollFactor, layer.width);
	if (x > 0) x -= layer.width;

	// zapetlanie layera
	for (int i = 0; i < layer.copies; i++) {
		Vector2 pos = { x + (i * layer.width), 0 };
		mgs->display->drawSprite(layer.spriteKey, pos, {(float)logDims.width, (float)logDims.height});
	}
}

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

Rect Camera::getViewport()
{
	return { transform.pos.x, transform.pos.y, (float)mgs->display->getLogWidth(), (float)mgs->display->getLogHeight() };
}

float Camera::getZoom()
{
	float pos_z = (transform.pos.z > 1.0f) ? transform.pos.z : 1.0f;

	return DEFAULT_CAM_Z / pos_z;
}

SpriteObject::SpriteObject(Managers* mgs, Transform tr, int key) 
	: GameObject(mgs, tr), spriteKey(key)
{

}

SpriteObject::~SpriteObject() {
	
}

Sprite* SpriteObject::getSprite() {
	if (spriteKey == 0) return nullptr;
	return mgs->sprite->get(spriteKey);
}

void SpriteObject::setSpriteKey(int key) {
	spriteKey = key;
}

void SpriteObject::draw()
{
	mgs->display->drawSprite(spriteKey, transform);
}

AnimatableObject::AnimatableObject(Managers* mgs, Transform tr)
	: GameObject(mgs, tr), currentAnimKey(0), currentAnimFrame(0), currentAnimTimer(0.0f)
{
	
}

AnimatableObject::~AnimatableObject() {
	
}

int AnimatableObject::getCurrentAnimKey()
{
	return currentAnimKey;
}

void AnimatableObject::setAnim(int key)
{
	if (key == 0 || currentAnimKey == key) return;

	currentAnimKey = key;
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


