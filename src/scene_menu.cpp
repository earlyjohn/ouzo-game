#include "scene_menu.h"

#include "graphics.h"
#include "game.h"
#include "shader.h"
#include "font.h"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

CSceneMenu::CSceneMenu() {}
CSceneMenu::~CSceneMenu() {}

void CSceneMenu::OnInit() {
  m_buttonTexture.LoadFromFile("assets/ui/button_test.png");

  S_CUIControlTexture tex(new CUIControlTexture(&m_buttonTexture));
  tex->x = 80;
  tex->y = 40;
  this->AddControl(tex);

  CSceneUI::OnInit();

  m_texture.LoadFromFile("assets/backgrounds/test.jpeg");

  S_CShader shaders[] = {
    S_CShader(new CShader(GL_VERTEX_SHADER, "assets/shaders/uitext.vsh")),
    S_CShader(new CShader(GL_FRAGMENT_SHADER, "assets/shaders/uitext.fsh")),
  };

  m_fontProgram.reset(new CProgram(2, shaders));

  glEnable(GL_TEXTURE_3D);

  CFont font("assets/fonts/Ubuntu-C.ttf");

  unsigned long chars[] = {
    0x41, 0x42, 0x43
  };

  font.CreateGlyphTexture(&m_fontTexture, 16, 3, chars);
}

void CSceneMenu::OnRender() {
  CSceneUI::OnRender();

  m_mvpMatrix.model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
  m_mvpMatrix.model = glm::scale(m_mvpMatrix.model, glm::vec3(16.0f, 16.0f, 1.0f));
  GFX->Begin(m_mvpMatrix, m_fontProgram);

  glUniform1f(m_fontProgram->GetUniformLocation("u_Char"), 0.0f);
  glUniform1f(m_fontProgram->GetUniformLocation("u_CharCount"), 3.0f);

  glBindBuffer(GL_ARRAY_BUFFER, CUISprite::s_globalSpriteBuffer);
  glBindTexture(GL_TEXTURE_3D, m_fontTexture.GetOpenGLHandle());
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  GFX->End();
}

void CSceneMenu::OnUpdate() {

}

void CSceneMenu::OnLeave(S_CScene *scene) {

}

CTexture* CSceneMenu::GetBackgroundTexture() {
  return &m_texture;
}
