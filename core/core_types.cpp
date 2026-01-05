#include "math.h"
#include "core_types.h"
#include "managers/managers.h"

#include "cstring"


void GameScene::addLayer(int spriteKey, float scrollFactor, float width, float height)
{
	int copies = (mgs->display->getLogWidth() / (int)width + 2);
	layers.add({ spriteKey, scrollFactor, {width, height}, copies });
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
	float x = fmodf(-camX * layer.scrollFactor, layer.dims.width);
	if (x > 0) x -= layer.dims.width;

	// zapetlanie layera
	for (int i = 0; i < layer.copies; i++) {
		Vector2 pos = { x + (i * layer.dims.width), 0 };
		mgs->display->drawSprite(layer.spriteKey, pos, layer.dims);
	}
}

GameObject::GameObject(Managers* mgs, Transform tr) 
	: mgs(mgs),
	transform(tr),
	rb({ {0,0,0}, tr.pos, tr.pos }),
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

Vector3 GameObject::getIPos()
{
	float ifactor = mgs->time->getIFactor();

	// pozycja zinterpolowana (pomiedzy klatkami fizyki)
	float dx = rb.prevPos.x * (1.0f - ifactor) + rb.currPos.x * ifactor;
	float dy = rb.prevPos.y * (1.0f - ifactor) + rb.currPos.y * ifactor;
	float dz = rb.prevPos.z * (1.0f - ifactor) + rb.currPos.z * ifactor;

	return { dx, dy, dz };
}

void GameObject::drawShadow(int sh_key, float obj_width, FDims sh_dims)
{
	float shX = transform.pos.x;
	float shY = transform.pos.z - sh_dims.height/2;

	// przy skoku
	float shScale = 1.0f / (1.0f - transform.pos.y / 2500.0f);

	// do szerokosci obiektu * stala
	shScale *= (obj_width / sh_dims.width) * .8f;

	sh_dims.width *= shScale;
	sh_dims.height *= shScale;

	Transform tr = Transform::zero();
	tr.pos = { shX, shY };
	tr.scale = { shScale, shScale };

	mgs->display->drawSprite(sh_key, tr);
}

Camera::Camera(Managers* mgs, Vector3 pos) : GameObject(mgs) {
	transform.pos = pos;
	rb.currPos = pos;
	rb.prevPos = pos;
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


