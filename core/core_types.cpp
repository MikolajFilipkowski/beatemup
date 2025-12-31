#include "core_types.h"
#include "managers/managers.h"

GameObject::GameObject(Managers* mgs, Transform tr, const char* key) 
	: mgs(mgs),
	transform(tr),
	spriteKey(copy_string(key)), 
	removalPending(false) {}

GameObject::~GameObject()
{
	delete[] spriteKey;
	spriteKey = nullptr;
}

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

Sprite* GameObject::getSprite() const
{
	return mgs->sprite->get(spriteKey);
}

void GameObject::setSprite(const char* key)
{
	delete[] spriteKey;
	spriteKey = copy_string(key);
}

bool GameObject::getRemovalFlag() const
{
	return removalPending;
}

void GameObject::setRemovalFlag(bool flag)
{
	removalPending = true;
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