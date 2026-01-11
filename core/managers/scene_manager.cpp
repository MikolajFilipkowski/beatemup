#include "scene_manager.h"

SceneManager::~SceneManager()
{
	destroy();
}

void SceneManager::destroy()
{
	for (auto& scene : m_Scenes) {
		delete scene.value;
		scene.value = nullptr;
	}
	m_Scenes.clear();
}

void SceneManager::changeScene()
{
	if (m_NextSceneIdx == 0) return;

	Scene* scene = m_Scenes.get(m_NextSceneIdx);
	if (scene != nullptr) {
		if (m_CurrentScene != nullptr)
			m_CurrentScene->destroy();

		m_CurrSceneIdx = m_NextSceneIdx;
		m_NextSceneIdx = 0;

		m_CurrentScene = scene;
		m_CurrentScene->start();
	}
}

void SceneManager::add(int a_Key, Scene* a_Scene)
{
	if (a_Scene == nullptr) return;

	m_Scenes.put(a_Key, a_Scene);
}

void SceneManager::load(int a_Key, bool a_Instant)
{
	m_NextSceneIdx = a_Key;

	if (a_Instant) changeScene();
}

Scene* SceneManager::getCurrentScene()
{
	return m_CurrentScene;
}

int SceneManager::getCurrentSceneIdx() const
{
	return m_CurrSceneIdx;
}

void SceneManager::update(float a_Dt)
{
	if (m_NextSceneIdx != 0)
		changeScene();
	m_CurrentScene->update(a_Dt);
}

void SceneManager::fixedUpdate(float a_FixedDt)
{
	if (m_NextSceneIdx != 0)
		changeScene();
	m_CurrentScene->fixedUpdate(a_FixedDt);
}

void SceneManager::draw()
{
	m_CurrentScene->draw();
}
