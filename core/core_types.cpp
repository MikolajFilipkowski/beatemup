#include "core_types.h"
#include "managers/managers.h"

GameObject::GameObject(Managers* mgs, Vector3 pos, const char* key) 
	: mgs(mgs),
	position(pos),
	spriteKey(copy_string(key)), 
	removalPending(false) {}

GameObject::~GameObject()
{
	delete[] spriteKey;
	spriteKey = nullptr;
}

Vector3& GameObject::getPosition()
{
	return position;
}

void GameObject::setPosition(Vector3 pos)
{
	position = pos;
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

Camera* Camera::activeCamera = nullptr;

Camera::Camera(Managers* mgs, Vector3 pos) : GameObject(mgs, pos) {}

Camera::~Camera()
{
	if (activeCamera == this) {
		activeCamera = nullptr;
	}
}

Rect Camera::getViewport(Dims logDims)
{
	return { position.x, position.y, (float)logDims.width, (float)logDims.height };
}

float Camera::getZoom()
{
	float pos_z = (position.z > 1.0f) ? position.z : 1.0f;

	return DEFAULT_CAM_Z / pos_z;
}

Camera* Camera::getActiveCamera()
{
	return activeCamera;
}

void Camera::setActiveCamera(Camera* cam)
{
	activeCamera = cam;
}


