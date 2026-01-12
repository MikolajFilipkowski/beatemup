#include <math.h>
#include "core_types.h"
#include "managers/managers.h"

#include <cstring>
#include <iostream>

ActionData::ActionData(int a_Prio, int a_Win, int a_Cond, bool a_Inter, int a_Owner, Array<int>* a_Seq, Array<ActionFrame>* a_Frames)
	: priority(a_Prio), inputWindow(a_Win), conditions(a_Cond),
	interruptible(a_Inter), owner(a_Owner), m_Sequence(a_Seq), m_Frames(a_Frames)
{
	for (int i = 0; i < m_Frames->count(); i++) {
		m_TotalDuration += m_Frames->get(i).duration;
	}
}

ActionData::~ActionData() {
	delete m_Sequence;
	delete m_Frames;
}

void ActionData::setSequence(Array<int>* a_Seq)
{
	m_Sequence = a_Seq;
}

void ActionData::setFrames(Array<ActionFrame>* a_Frames)
{
	m_Frames = a_Frames;
	m_TotalDuration = 0.0f;
	for (int i = 0; i < a_Frames->count(); i++) {
		m_TotalDuration += a_Frames->get(i).duration;
	}
}

Array<int>* ActionData::getSequence() const
{
	return m_Sequence;
}

Array<ActionFrame>* ActionData::getFrames() const
{
	return m_Frames;
}

float ActionData::getTotalDuration()
{
	return m_TotalDuration;
}


void GameScene::addLayer(int a_SpriteKey, float a_ScrollFactor, float a_Width, float a_Height)
{
	int copies = (m_Mgs->display->getLogWidth() / (int)a_Width + 2);
	m_Layers.add({ a_SpriteKey, a_ScrollFactor, {a_Width, a_Height}, copies });
}

void GameScene::drawBackground()
{
	float camX = m_Mgs->display->getActiveCamera()->getPosition().x;
	Dims logDims = m_Mgs->display->getLogDims();

	for (auto& layer : m_Layers) {
		drawLayer(layer, camX, logDims);
	}
}

void GameScene::drawLayer(BackgroundLayer& a_Layer, float a_CamX, Dims& a_LogDims)
{
	// x - pozycja layera (<=0)
	float x = fmodf(-a_CamX * a_Layer.scrollFactor, a_Layer.dims.width);
	if (x > 0) x -= a_Layer.dims.width;

	// zapetlanie layera
	for (int i = 0; i < a_Layer.copies; i++) {
		Vector2 pos = { x + (i * a_Layer.dims.width), 0 };
		m_Mgs->display->drawSprite(a_Layer.spriteKey, pos, a_Layer.dims);
	}
}

GameObject::GameObject(Managers* a_Managers, Transform a_Transform) 
	: m_Mgs(a_Managers),
	m_Transform(a_Transform),
	m_Rb({ {0,0,0}, a_Transform.pos, a_Transform.pos })
{
	if (a_Managers->object != nullptr)
		a_Managers->object->add((GameObject*)this);
}

GameObject::~GameObject() {}

Uint8 GameObject::getType() const
{
	return 0U;
}

Transform& GameObject::getTransform()
{
	return m_Transform;
}

void GameObject::setTransform(Transform a_Transform)
{
	m_Transform = a_Transform;
}

Vector3& GameObject::getPosition()
{
	return m_Transform.pos;
}

void GameObject::setPosition(Vector3 a_Pos)
{
	m_Transform.pos = a_Pos;
}

Rigidbody& GameObject::getRb()
{
	return m_Rb;
}

bool GameObject::getRemovalFlag() const
{
	return m_RemovalPending;
}

void GameObject::setRemovalFlag(bool a_Flag)
{
	m_RemovalPending = true;
}

bool GameObject::getStartedFlag() const
{
	return m_HasStarted;
}

void GameObject::setStartedFlag(bool a_Flag)
{
	m_HasStarted = a_Flag;
}

Vector3 GameObject::getIPos()
{
	float ifactor = m_Mgs->time->getIFactor();

	// pozycja zinterpolowana (pomiedzy klatkami fizyki)
	float dx = m_Rb.prevPos.x * (1.0f - ifactor) + m_Rb.currPos.x * ifactor;
	float dy = m_Rb.prevPos.y * (1.0f - ifactor) + m_Rb.currPos.y * ifactor;
	float dz = m_Rb.prevPos.z * (1.0f - ifactor) + m_Rb.currPos.z * ifactor;

	return { dx, dy, dz };
}

void GameObject::drawShadow(int a_ShKey, float a_ObjWidth, FDims a_ShDims)
{
	float shX = m_Transform.pos.x;
	float shY = m_Transform.pos.z - a_ShDims.height/2;

	// przy skoku
	float shScale = 1.0f / (1.0f - m_Transform.pos.y / 2500.0f);

	// do szerokosci obiektu * stala
	shScale *= (a_ObjWidth / a_ShDims.width) * .8f;

	a_ShDims.width *= shScale;
	a_ShDims.height *= shScale;

	Transform tr = Transform::zero();
	tr.pos = { shX, shY };
	tr.scale = { shScale, shScale };

	m_Mgs->display->drawSprite(a_ShKey, tr);
}

Camera::Camera(Managers* a_Managers, Vector3 a_Pos) : GameObject(a_Managers) {
	m_Transform.pos = a_Pos;
	m_Rb.currPos = a_Pos;
	m_Rb.prevPos = a_Pos;
}

Camera::~Camera() {}

Rect Camera::getViewport()
{
	return { m_Transform.pos.x, m_Transform.pos.y, (float)m_Mgs->display->getLogWidth(), (float)m_Mgs->display->getLogHeight() };
}

float Camera::getZoom()
{
	float pos_z = (m_Transform.pos.z > 1.0f) ? m_Transform.pos.z : 1.0f;

	return DEFAULT_CAM_Z / pos_z;
}

SpriteObject::SpriteObject(Managers* a_Managers, Transform a_Transform, int a_Key) 
	: GameObject(a_Managers, a_Transform), m_SpriteKey(a_Key)
{

}

SpriteObject::~SpriteObject() {
	
}

Sprite* SpriteObject::getSprite() {
	if (m_SpriteKey == 0) return nullptr;
	return m_Mgs->sprite->get(m_SpriteKey);
}

void SpriteObject::setSpriteKey(int a_Key) {
	m_SpriteKey = a_Key;
}

void SpriteObject::draw()
{
	m_Mgs->display->drawSprite(m_SpriteKey, m_Transform);
}