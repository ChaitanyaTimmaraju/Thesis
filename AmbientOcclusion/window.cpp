#include<QDebug>
#include<QString>
#include<QGuiApplication>
#include<QTime>
#include <QSize>
#include "window.h"
#include "firstpass.h"
#include "passinterface.h"
#include "debughelperpass.h"
#include "secondpass.h"

#define DEBUG_ON 0
#define FIRSTPASS 1
#define SECONDPASS 1

static DebugHelperPass objectDP;
static FirstPass objectFP;
static SecondPass objectSP;

Window::Window()
{
    objectFilePairs["cow"]=":/models/cowVNT.obj";
    objectFilePairs["dolphin"]=":/models/aDolphinVNT.obj";
    objectFilePairs["al"]=":/models/al_TVNT.obj";
    objectFilePairs["bones"]=":/models/bonesVNT.obj";
    objectFilePairs["bunny"]=":/models/bunnyVNT.obj";
    objectFilePairs["cube"]=":/models/cubeAltFTVNT.obj";
    objectFilePairs["dragon"]=":/models/dragonVNT.obj";
    objectFilePairs["face"]=":/models/faceVNT.obj";
    objectFilePairs["fandisk"]=":/models/fandiskVNT.obj";
    objectFilePairs["gourd"]=":/models/gourdVNT.obj";
    objectFilePairs["man"]=":/models/mannVNT.obj";
    objectFilePairs["shuttle"]=":/models/shuttleTVNT.obj";
    objectFilePairs["soccerball"]=":/models/soccerballTVNT.obj";
    objectFilePairs["stegosaurus"]=":/models/stegosaurusVNT.obj";
    objectFilePairs["tetrahedron"]=":/models/tetrahedronVNT.obj";
    objectFilePairs["triceratops"]=":/models/triceratopsVNT.obj";
    objectFilePairs["victory"]=":/models/v2VNT.obj";
    objectFilePairs["quad"]=":/models/quad.obj";

}

Window::~Window()
{
    makeCurrent();
    teardownGL();
}

void Window::keyPressEvent(QKeyEvent *e)
{
    char c  = e->text().toStdString()[0];
}

bool Window::Init(unsigned int WindowWidth, unsigned int WindowHeight)
{
    // Create the FBO
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

    // Create the gbuffer textures
    glGenTextures(GBUFFER_NUM_TEXTURES, m_textures);

    for (unsigned int i = 0 ; i < GBUFFER_NUM_TEXTURES ; i++) {
       glBindTexture(GL_TEXTURE_2D, m_textures[i]);

       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WindowWidth, WindowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
       glGenerateMipmap(GL_TEXTURE_2D);
       glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
       glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
       glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);
    }

    GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3 };
    glDrawBuffers(GBUFFER_NUM_TEXTURES,DrawBuffers);

    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        printf("FB error, status: 0x%x\n", Status);
        return false;
    }

    // restore default FBO
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    return true;
}

void Window::BindForWriting()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
}

void Window::BindForReading()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
}

void Window::SetReadBuffer(Window::GBUFFER_TEXTURE_TYPE TextureType)
{
    glReadBuffer(GL_COLOR_ATTACHMENT0 + TextureType);
}

void Window::ReadFromFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    BindForReading();
    int WINDOW_WIDTH = QWindow::width();
    int WINDOW_HEIGHT = QWindow::height();
    GLsizei HalfWidth = (GLsizei)(WINDOW_WIDTH / 2.0f);
    GLsizei HalfHeight = (GLsizei)(WINDOW_HEIGHT / 2.0f);

    SetReadBuffer(GBUFFER_TEXTURE_TYPE_POSITION0);
    glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
                       0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);


    SetReadBuffer(GBUFFER_TEXTURE_TYPE_POSITION1);
    glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
                       0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    SetReadBuffer(GBUFFER_TEXTURE_TYPE_POSITION2);
    glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
                       0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    SetReadBuffer(GBUFFER_NORMAL_TEXTURE);
    glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
                       0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    for (unsigned int i = 0 ; i < GBUFFER_NUM_TEXTURES; i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, m_textures[GBUFFER_TEXTURE_TYPE_POSITION0 + i]);
        }

}

void Window::initializeGL()
{
    //Initialize OpenGL backend
    initializeOpenGLFunctions();
    connect(this,SIGNAL(frameSwapped()),this,SLOT(update()));
    //set global information
    printContextInformation();
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);


    //Load all models required
    const QString objs[]={"dragon"};
    m_transform.scale(0.7,0.7,0.7);
    m_transform.rotate(-90.0f,1.0,0.0,0.0);
    m_transform.translate(0.0,0.0,-2.0);

    for(auto x : objs)
        models.push_back(new ModelLoader(objectFilePairs[x]));

    secondPassModels.push_back(new ModelLoader(objectFilePairs["dragon"]));

    if(SECONDPASS)
        Init(QWindow::width(),QWindow::height());

    //First pass
    objectFP.initializations(models);
    //Debug pass
    objectDP.initializations(models);
    //Second pass
    objectSP.initializations(secondPassModels);

    //setting the object to world matrix
    objectFP.m_transform = objectDP.m_transform = objectSP.m_transform = m_transform;
}

void Window::resizeGL(int width, int height)
{
    m_projection.setToIdentity();
    m_projection.perspective(45.0f,(double)(width)/(double)height,0.1,10);
    objectFP.m_projection = objectDP.m_projection = objectSP.m_projection= m_projection;

}

void Window::paintGL()
{
    //First Pass
    if(FIRSTPASS)
     {
        objectFP.setUniforms();

        if(SECONDPASS)
            BindForWriting();

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        for(int modelIndex=0;modelIndex<models.size();++modelIndex)
        {
            objectFP.setObjectData(modelIndex);
            glDrawElements(GL_TRIANGLES,models[modelIndex]->indices.size(),GL_UNSIGNED_INT,0);
        }
        objectFP.releaseProgramAndObjectData();
     }

     if(DEBUG_ON)
     {
        //Debug Pass For Normals
        objectDP.setUniforms();
        for(int modelIndex=0;modelIndex<models.size();++modelIndex)
        {
            objectDP.setObjectData(modelIndex);
            glDrawElements(GL_POINTS,models[modelIndex]->indices.size(),GL_UNSIGNED_INT,0);
        }
        objectDP.releaseProgramAndObjectData();
     }

     if(SECONDPASS)
     {
         //Second Pass
         objectSP.setUniforms();
         ReadFromFrameBuffer();
         glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
         for(int modelIndex=0;modelIndex<secondPassModels.size();++modelIndex)
         {
            objectSP.setObjectData(modelIndex);
            glDrawElements(GL_TRIANGLES,secondPassModels[modelIndex]->indices.size(),GL_UNSIGNED_INT,0);
         }
        objectSP.releaseProgramAndObjectData();
     }
}

void Window::delay(float x)
{
    QTime dieTime= QTime::currentTime().addMSecs(x);
    while (QTime::currentTime() < dieTime)
        QGuiApplication::processEvents(QEventLoop::AllEvents, 100);
}

void Window::update()
{
 //objectFP.m_transform.rotate(0.5,0.0,1.0,0.0);
 //objectDP.m_transform.rotate(0.5,0.0,1.0,0.0);
 // objectSP.m_transform.rotate(0.5,0.0,1.0,0.0);

 // QOpenGLWindow::update();
}

void Window::teardownGL()
{
   // Actually destroy our OpenGL information

}

/* private helpers*/
void Window::printContextInformation()
{
    QString glType;
    QString glVersion;
    QString glProfile;

    //Get Version Information
    glType=(context()->isOpenGLES())?"OpenGL ES":"OpenGL";
    glVersion= reinterpret_cast<const char*>(glGetString(GL_VERSION));

    //Get Profile Information
#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
  switch (format().profile())
  {
    CASE(NoProfile);
    CASE(CoreProfile);
    CASE(CompatibilityProfile);
  }
#undef CASE
  // qPrintable() will print our QString w/o quotes around it.
 qDebug()<<qPrintable(glType)<<qPrintable(glVersion)<<"("<<qPrintable(glProfile)<<")";
}
