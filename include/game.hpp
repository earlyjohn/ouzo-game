#pragma once

#include "scene.hpp"
#include "graphics.hpp"

#define GFX CGame::Inst->GetGraphics()

#include <memory>
#include <SDL2/SDL.h>

struct GLFWwindow;

class CGame {
public:
	CGame();
	~CGame();

	static CGame *Inst;

	void InitGame();

	void StartLoop();
	void Stop();

	void SetTargetFPS(unsigned int fps);
	void SetVerticalSync(bool vsync);

	void SetScene(CScene *scene);
	S_CScene GetScene();

	S_CGraphics GetGraphics();

	unsigned int GetWidth();
	unsigned int GetHeight();

	unsigned int m_width, m_height;

private:
	void Event_OnResize(int width, int height);
	void Event_OnClick(int x, int y, int button, bool press);

	void TranslateEvent(SDL_Event &event);

	SDL_Surface *LoadIcon(const char *file);

	void OnRender();
	void OnUpdate();

	unsigned int m_targetFPS;
	float m_deltaTime;
	bool m_vsync;

	bool m_running;
	SDL_Window *m_window;

	S_CScene m_scene;
	S_CGraphics m_graphics;
};