#pragma once

class Application {
protected:
	Managers* mgs;
public:
	bool fullscreenDisabled;
	Application() : mgs(nullptr), fullscreenDisabled(false) {}
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;
	virtual ~Application() = default;
	
	/*
	* \brief Metoda po wykonaniu engine.run(&app)
	* 
	* \return true przy poprawnej inicjalizacji, false by wywolac engine.stop()
	*/
	virtual bool onStart(Managers* managers) = 0;
	// \brief Metoda wykonywana w kazdej klatce
	virtual void onUpdate(float dt) = 0;
	// \brief Metoda wykonywana okreslona liczbê razy na sekundê
	virtual void onFixedUpdate(float fixed_dt) = 0;
	// \brief Metoda wykonywana w kazdej klatce przy rysowaniu
	virtual void onDraw() = 0;
	// \brief Metoda wykonywana przed engine.destroy()
	virtual void onDestroy() = 0;
	/*
	* \brief Metoda po nacisnieciu ESC
	* 
	* \return true ¿eby wywo³aæ engine.stop(), false brak akcji
	*/
	virtual bool onEscPressed() { return true; }
};