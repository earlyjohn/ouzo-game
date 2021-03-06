#pragma once

#include <memory>

class CScene {
public:
	virtual void OnRender();
	virtual void OnUpdate();
	virtual void OnInit();
	virtual void OnLeave(CScene *newScene);
	virtual void OnResize(int newWidth, int newHeight);
	virtual void OnClick(unsigned int button, float x, float y);
};

typedef std::shared_ptr<CScene> S_CScene;
