#include "window.h"
#include<QDebug>
#include<QString>
#include<QGuiApplication>
#include<QTime>
#include "firstpass.h"
#include "passinterface.h"


FirstPass objectFP;


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

/*OpenGL events*/

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
    const QString objs[]={"cow"};
    for(auto x : objs)
        models.push_back(new ModelLoader(objectFilePairs[x]));


    //FirstPass Related information
    objectFP.initializations(models);


}

void Window::resizeGL(int width, int height)
{
    m_projection.setToIdentity();
    m_projection.perspective(45,(float)(width)/(float)height,0.1,100);
}


void Window::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    objectFP.setUniforms();
    for(int modelIndex=0;modelIndex<models.size();++modelIndex)
    {
        objectFP.setObjectData(modelIndex);
        models[modelIndex]->draw();
    }
    objectFP.releaseProgramAndObjectData();

}

void Window::delay(float x)
{
    QTime dieTime= QTime::currentTime().addMSecs(x);
    while (QTime::currentTime() < dieTime)
        QGuiApplication::processEvents(QEventLoop::AllEvents, 100);
}

void Window::update()
{

  QOpenGLWindow::update();
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
