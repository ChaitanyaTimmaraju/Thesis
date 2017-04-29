#include "window.h"
#include "debughelperpass.h"
#include "firstpass.h"
#include "passinterface.h"
#include "secondpass.h"
#include <QDebug>
#include <QGuiApplication>
#include <QSize>
#include <QString>
#include <QTime>
#include <QOpenGLExtraFunctions>
#define glCheckError() glCheckError_(__FILE__, __LINE__)

static bool DEBUG_ON = false;
static bool FIRSTPASS = true;
static bool SECONDPASS = true;
static bool ANIMATE = true;
static float rotationSpeed = 1.0;

static DebugHelperPass objectDP;
static FirstPass objectFP;
static SecondPass objectSP;

void Window::setFrameBuffer() {
  QOpenGLFramebufferObjectFormat format;
  format.setAttachment(
      QOpenGLFramebufferObject::Attachment::CombinedDepthStencil);
  format.setTextureTarget(GL_TEXTURE_2D);
  format.setInternalTextureFormat(GL_RGBA32F_ARB);
  format.setMipmap(true);
  fbo = new QOpenGLFramebufferObject(width(), height(), format);
  if (fbo->isValid()) {
    qDebug() << "Framebuffer Created";
  }
  fbo->addColorAttachment(width(), height());
  fbo->addColorAttachment(width(), height());
  fbo->addColorAttachment(width(), height());
  fbo->addColorAttachment(width(), height());
  fbo->bind();

  // Clear buffers.
  QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();
  GLenum bufs[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                    GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
  f->glDrawBuffers(4, bufs);
  fbo->release();
  // Need to set textures only once after creating the FBO.
  setTextures();
}

void Window::setTextures() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // get all texture ID's for binding them
  QVector<GLuint> textureID = fbo->textures();
  for (int i = 0; i < 4; ++i) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, textureID[i]);
  }
  glCheckError();
}

void Window::saveTexturesToFiles() {
  for (int i = 0; i < 4; ++i) {
    QImage temp = fbo->toImage(true, i);
    // Note: Please use file extension otherwise it will not create
    temp.save("pics/file" + QString::number(i) + ".png");
  }
}

void Window::initializeGL() {
  // Initialize OpenGL backend
  initializeOpenGLFunctions();
  connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
  // set global information
  printContextInformation();
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glCullFace(GL_BACK);

  // Load all models required
  const QString objs[] = {"cow"};
  // m_transform.scale(0.7, 0.7, 0.7);
  // m_transform.rotate(-90.0f, 1.0, 0.0, 0.0);
  m_transform.translate(0.0, 0.0, -2.0);

  for (auto x : objs)
    models.push_back(new ModelLoader(objectFilePairs[x]));

  // secondPassModels.push_back(new ModelLoader(objectFilePairs["dragon"]));

  // create FrameBuffer
  setFrameBuffer();
  // First pass
  objectFP.initializations(models);
  // Debug pass
  objectDP.initializations(models);
  // Second pass
  objectSP.initializations(models);
  // setting the object to world matrix
  objectFP.m_transform = objectDP.m_transform = objectSP.m_transform =
      m_transform;
}

void Window::paintGL() {
  if (FIRSTPASS) {
    fbo->bind();
    draw(GL_TRIANGLES, objectFP, models, true);
    fbo->release();
  }

  if (SECONDPASS) {
    draw(GL_TRIANGLES, objectSP, models, true);
  }

  if (DEBUG_ON) {
    // Debug Pass For Normals
    draw(GL_POINTS, objectDP, models, false);
  }
}

void Window::update() {
  if (ANIMATE) {
    objectFP.m_transform.rotate(rotationSpeed, 0.0, 1.0, 0.0);
    objectDP.m_transform.rotate(rotationSpeed, 0.0, 1.0, 0.0);
    objectSP.m_transform.rotate(rotationSpeed, 0.0, 1.0, 0.0);
  }
  QOpenGLWindow::update();
}

void Window::resizeGL(int width, int height) {
  m_projection.setToIdentity();
  m_projection.perspective(45.0f, (double)(width) / (double)height, 0.1, 10);
  objectFP.m_projection = objectDP.m_projection = objectSP.m_projection =
      m_projection;
}

void Window::draw(GLenum mode,
                  PassInterface& passObject,
                  std::vector<ModelLoader*>& modelsList,
                  bool clearScreen) {
  /* Generic draw command which takes pass interface object and
   * all the models as reference and draws it.
  */
  passObject.setUniforms();
  if (clearScreen)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (int modelIndex = 0; modelIndex < modelsList.size(); ++modelIndex) {
    passObject.setObjectData(modelIndex);
    glDrawElements(mode, modelsList[modelIndex]->indicesCount, GL_UNSIGNED_INT,
                   0);
  }
  passObject.releaseProgramAndObjectData();
}
void Window::clicked(int temp) {
  switch (temp) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
      objectSP.m_shaderHandlerObject.m_program->bind();
      objectSP.m_shaderHandlerObject.m_program->setUniformValue(
          "textureToDisplay", temp);
      objectSP.m_shaderHandlerObject.m_program->release();
      break;
    case 6:
      ANIMATE = !ANIMATE;
      break;
    case 7:
      DEBUG_ON = !DEBUG_ON;
      break;
  }
}
GLenum Window::glCheckError_(QString file, int line) {
  GLenum errorCode;
  while ((errorCode = glGetError()) != GL_NO_ERROR) {
    QString error;
    switch (errorCode) {
      case GL_INVALID_ENUM:
        error = "INVALID_ENUM";
        break;
      case GL_INVALID_VALUE:
        error = "INVALID_VALUE";
        break;
      case GL_INVALID_OPERATION:
        error = "INVALID_OPERATION";
        break;
      case GL_STACK_OVERFLOW:
        error = "STACK_OVERFLOW";
        break;
      case GL_STACK_UNDERFLOW:
        error = "STACK_UNDERFLOW";
        break;
      case GL_OUT_OF_MEMORY:
        error = "OUT_OF_MEMORY";
        break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        error = "INVALID_FRAMEBUFFER_OPERATION";
        break;
    }
    qDebug() << error << " | " << file << " (" << line << ")";
  }
  return errorCode;
}

Window::Window() {
  objectFilePairs["cow"] = ":/models/cowVNT.obj";
  objectFilePairs["dolphin"] = ":/models/aDolphinVNT.obj";
  objectFilePairs["al"] = ":/models/al_TVNT.obj";
  objectFilePairs["bones"] = ":/models/bonesVNT.obj";
  objectFilePairs["bunny"] = ":/models/bunnyVNT.obj";
  objectFilePairs["cube"] = ":/models/cubeAltFTVNT.obj";
  objectFilePairs["dragon"] = ":/models/dragonVNT.obj";
  objectFilePairs["face"] = ":/models/faceVNT.obj";
  objectFilePairs["fandisk"] = ":/models/fandiskVNT.obj";
  objectFilePairs["gourd"] = ":/models/gourdVNT.obj";
  objectFilePairs["man"] = ":/models/mannVNT.obj";
  objectFilePairs["shuttle"] = ":/models/shuttleTVNT.obj";
  objectFilePairs["soccerball"] = ":/models/soccerballTVNT.obj";
  objectFilePairs["stegosaurus"] = ":/models/stegosaurusVNT.obj";
  objectFilePairs["tetrahedron"] = ":/models/tetrahedronVNT.obj";
  objectFilePairs["triceratops"] = ":/models/triceratopsVNT.obj";
  objectFilePairs["victory"] = ":/models/v2VNT.obj";
  objectFilePairs["quad"] = ":/models/quad.obj";
}

Window::~Window() {
  makeCurrent();
  teardownGL();
}

void Window::teardownGL() {
  // Actually destroy our OpenGL information
}

/* private helpers*/
void Window::printContextInformation() {
  QString glType;
  QString glVersion;
  QString glProfile;

  // Get Version Information
  glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
  glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

// Get Profile Information
#define CASE(c)           \
  case QSurfaceFormat::c: \
    glProfile = #c;       \
    break
  switch (format().profile()) {
    CASE(NoProfile);
    CASE(CoreProfile);
    CASE(CompatibilityProfile);
  }
#undef CASE
  // qPrintable() will print our QString w/o quotes around it.
  qDebug() << qPrintable(glType) << qPrintable(glVersion) << "("
           << qPrintable(glProfile) << ")";
}

void Window::keyPressEvent(QKeyEvent* e) {
  char c = e->text().toStdString()[0];
}
void Window::delay(float x) {
  QTime dieTime = QTime::currentTime().addMSecs(x);
  while (QTime::currentTime() < dieTime)
    QGuiApplication::processEvents(QEventLoop::AllEvents, 100);
}
