#pragma once

class Engine;
class Managers;

class Manager {
	friend class Engine;
protected:
	Managers* mgs;

	Manager(Managers* managers) : mgs(managers) {}
	virtual ~Manager() {}

	virtual void destroy() = 0;
};