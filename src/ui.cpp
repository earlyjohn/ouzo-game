#include "ui.h"

#include <GL/glew.h>

#include "graphics.h"
#include "texture.h"
#include "game.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const vertex_t QUAD_VERTICES[] = {
  { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
  { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
  { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f },
  { 1.0f, 1.0f, 0.0f, 1.0f, 1.0f },
};

///
/// CSceneUI
///

CSceneUI::CSceneUI() {
    m_mainControl.reset(new CUIControlTexture);
}

CSceneUI::~CSceneUI() {}

void CSceneUI::OnInit() {
  m_mvpMatrix.projection = glm::mat4(1.0f);
  m_mvpMatrix.view = glm::mat4(1.0f);
  m_mvpMatrix.model = glm::mat4(1.0f);
}

void CSceneUI::AddControl(std::shared_ptr<CUIControl> control) {
  m_mainControl->AddChild(control);
}

void CSceneUI::OnResize(int width, int height) {
  m_width = width;
  m_height = height;
  m_mvpMatrix.projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
  this->UpdateBackground();
}

void CSceneUI::UpdateBackground() {
  m_mainControl->SetTexture(this->GetBackgroundTexture());
  m_mainControl->width = m_width;
  m_mainControl->height = m_height;
}

void CSceneUI::OnRender() {

  for (S_CUIRenderable renderable : *m_mainControl->GetRenderables()) {
    m_mvpMatrix.model = glm::translate(glm::mat4(1.0f), glm::vec3(m_mainControl->x, m_mainControl->y, 0.0f));
    m_mvpMatrix.model = glm::scale(m_mvpMatrix.model, glm::vec3(m_mainControl->width, m_mainControl->height, 1.0f));
    renderable->OnRender(m_mvpMatrix);
  }


  for (S_CUIControl control : *m_mainControl->GetChildren()) {
    for (S_CUIRenderable renderable : *control->GetRenderables()) {
      m_mvpMatrix.model = glm::translate(glm::mat4(1.0f), glm::vec3(control->x, control->y, 0.0f));
      m_mvpMatrix.model = glm::scale(m_mvpMatrix.model, glm::vec3(control->width, control->height, 1.0f));
      renderable->OnRender(m_mvpMatrix);  // This is pretty naive.
                                          // Replace with instancing (& multidraw indirect).
    }
  }
}

CTexture* CSceneUI::GetBackgroundTexture() {
  return nullptr;
}

void CSceneUI::OnClick(unsigned int button, float x, float y) {
  printf("Clicked at %d, %d with button %d\n", (int)x, (int)y, button);

  for (S_CUIControl c : *m_mainControl->GetChildren()) {
    if (x <= c->x + c->width && x >= c->x && y <= c->y + c->height && y >= c->y) {
      ui_event_params_t clickEvent;
      clickEvent.x = x - c->x;
      clickEvent.y = y - c->y;
      clickEvent.button = button;

      UIEvent type = UIEvent::CLICK;
      c->HandleEvent(type, clickEvent);
    }
  }
}

///
/// CUIControl
///

CUIControl::CUIControl()
  : x(0.0f), y(0.0f),
    width(0.0f), height(0.0f)
{
}

void CUIControl::AddChild(S_CUIControl control) {
  m_children.push_back(control);
}

void CUIControl::AddRenderable(S_CUIRenderable renderable) {
  m_renderables.push_back(renderable);
}

std::vector< std::shared_ptr<CUIControl> >* CUIControl::GetChildren() {
  return &m_children;
}

std::vector<S_CUIRenderable>* CUIControl::GetRenderables() {
  return &m_renderables;
}

///
/// CUISprite
///

unsigned int CUISprite::s_globalSpriteBuffer = 0;
unsigned int CUISprite::s_activeSprites = 0;

CUISprite::CUISprite(CTexture *tex) {
  if (tex != nullptr) {
    this->SetTexture(tex);
  }

  if (s_globalSpriteBuffer == 0) {
    glGenBuffers(1, &s_globalSpriteBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, s_globalSpriteBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD_VERTICES), QUAD_VERTICES, GL_STATIC_DRAW);
  }

  s_activeSprites++;
}

CUISprite::~CUISprite() {
  s_activeSprites--;

  if (s_activeSprites == 0) {
    glDeleteBuffers(1, &s_globalSpriteBuffer);
    s_globalSpriteBuffer = 0;
  }
}

void CUISprite::OnRender(mvp_matrix_t &mvp) {
  GFX->Begin(mvp, GFX->GetDefaultProgram());
  m_texture->Use();

  glBindBuffer(GL_ARRAY_BUFFER, s_globalSpriteBuffer);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  GFX->End();
}

void CUISprite::SetTexture(CTexture *tex) {
  m_texture = tex;
}

///
/// CUIControl
///

void CUIControl::HandleEvent(UIEvent event, ui_event_params_t &params) {
  if (event == UIEvent::CLICK) {
    printf("Got clicked at %f, %f with button %d\n", params.x, params.y, params.button);
  }
}

///
/// CUIControlBackground
///

CUIControlTexture::CUIControlTexture(CTexture *tex)
  : m_sprite(new CUISprite(nullptr))
{
  if (tex != nullptr) {
    m_sprite->SetTexture(tex);
    this->width = tex->GetWidth();
    this->height = tex->GetHeight();
  }

  this->AddRenderable(m_sprite);
}

void CUIControlTexture::SetTexture(CTexture *tex) {
  m_sprite->SetTexture(tex);
}