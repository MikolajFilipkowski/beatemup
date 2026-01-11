#pragma once

class Engine;
class Managers;

class Manager {
	friend class Engine;
protected:
	Managers* m_Mgs;

	Manager(Managers* a_managers) : m_Mgs(a_managers) {}
	virtual ~Manager() {}

	virtual void destroy() = 0;
};