/*************************************************************************\
*  Show Mesh                                                              *
*  Copyright (C) 2006 Quentin Bereau                                      *
*                                                                         *
*  This program is free software; you can redistribute it and/or modify   *
*  it under the terms of the GNU General Public License version 2 as      *
*  published by the Free Software Foundation.                             *
*                                                                         *
*  This program is distributed in the hope that it will be useful,        *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
*  GNU General Public License for more details.                           *
*                                                                         *
*  You should have received a copy of the GNU General Public License      *
*  along with this program; if not, write to the                          *
*  Free Software Foundation, Inc.,                                        *
*  59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.              *
\*************************************************************************/

#include "OgreView.hpp"
//#include "MainWindow.h"
using namespace Ogre;

#if !defined(Q_WS_WIN)
#include <QX11Info>
#endif

//===================CLASS LOGLISTENER======================================
myLogListener::myLogListener(QWidget* parent) : LogListener(), QWidget(parent)
{

}
myLogListener::~myLogListener()
{

}
void myLogListener::write(const Ogre::String& name, const Ogre::String& msg, Ogre::LogMessageLevel lvl, bool maskDebug)
{
	Q_UNUSED(name);
	Q_UNUSED(lvl);
	Q_UNUSED(maskDebug);
	logMsg = msg.c_str();
}
QString myLogListener::getMsg()
{
	return logMsg;
}
//===========================================================================

OgreView::OgreView(QWidget* parent) : QWidget(parent)
{
	mRenderWindow = NULL;
	mSceneMgr = NULL;
	mVp = NULL;
	mainEnt = NULL;
	mainNode = NULL;
	mainSubEnt = NULL;
	mainEntAnim = NULL;

	mouseLeftPressed = false;
	mouseRightPressed = false;
	mouseMiddleBtn = false;
	isMoving = false;
	isLoopOn = false;
	isAnimEnabled = false;

	mDirection = Vector3::ZERO;
	mMove = 5;
	mRotate = 0.0005;
	mousePos = QPoint(0, 0);
	mousePressPos = QPoint(0, 0);
    angleX = 0.0; // Added these because the display started spinning
    angleY = 0.0; // when I upgraded to Qt4.2, they were left undefined.
	rotX = 0.0;
	rotY = 0.0;

	FPS = 0;
	triangles = 0;

	oldMsgLog = "";

	fogColor.setAsRGBA(qRgba(0, 0, 0, 1));
	backgroundColor.setAsRGBA(qRgba(0, 0, 0, 1));
	backgroundOldColor.setRgba(qRgba(0, 0, 0, 1));
	lightDiffuseColor.setAsRGBA(qRgba(0, 0, 0, 1));
	lightSpecularColor.setAsRGBA(qRgba(0, 0, 0, 1));
	ambientLightColor.setAsRGBA(qRgba(0, 0, 0, 1));

	setAcceptDrops(true);
	setupResources();
}

OgreView::~OgreView()
{
	if(mRenderWindow != NULL){
		delete mRenderWindow;
		mRenderWindow = 0;
	}
	if(mSceneMgr != NULL){
		delete mSceneMgr;
		mSceneMgr = 0;
	}
	if(mVp != NULL){
		delete mVp;
		mVp = 0;
	}
	if(mRoot != NULL){
		delete mRoot;
		mRoot = 0;
	}
}

//******************************Setup Ogre***********************************

void OgreView::setupResources()
{
	mRoot = new Root();

	mLogListener = new myLogListener();
	LogManager::getSingleton().addListener(mLogListener);

	Ogre::String dataPath("/home/rune/svn/projects/blue/code/data/");
	ConfigFile cf;
	cf.load(dataPath + "/ogre/resources.cfg");


	ConfigFile::SectionIterator seci = cf.getSectionIterator();
	String secName, typeName, archName;
	while(seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		ConfigFile::SettingsMultiMap* settings = seci.getNext();
		ConfigFile::SettingsMultiMap::iterator i;
		for(i=settings->begin(); i!=settings->end(); ++i){
			typeName = i->first;
			archName = dataPath + i->second;
			ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
		}
	}

	mRoot->restoreConfig();
	mRoot->initialise(false);
}

void OgreView::createScene()
{
	//mPlane = new MovablePlane("BasePlane");
	mPlane = new Plane;
	mPlane->d = 0;
	mPlane->normal = Vector3::UNIT_Y;
	MeshManager::getSingleton().createPlane("BasePlane", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
												 *mPlane, 10000, 10000, 1, 1, true, 1, 100, 100, Vector3::UNIT_Z);
	plane = mSceneMgr->createEntity("Plane", "BasePlane");
	plane->setMaterialName("BasePlane");
	planeNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("PlaneNode");
	planeNode->attachObject(plane);
	planeNode->setPosition(Vector3(0, -50, 0));
}
void OgreView::createFog(){}

void OgreView::createLight()
{
	mSceneMgr->setAmbientLight(ColourValue(1, 1, 1));
	ambientOldColor = QColor(Qt::white);
    mainLight = mSceneMgr->createLight("Main Light");
    mainLight->setPosition(0, 100, -500);
}

void OgreView::setupView()
{
    if(mRenderWindow)
        return;

    NameValuePairList params;

#if !defined(Q_WS_WIN)
    QWidget *q_parent = dynamic_cast <QWidget *> (parent());
    QX11Info xInfo = x11Info();

    params["parentWindowHandle"] = Ogre::StringConverter::toString ((unsigned long)xInfo.display()) +
        ":" + Ogre::StringConverter::toString ((unsigned int)xInfo.screen()) +
        ":" + Ogre::StringConverter::toString ((unsigned long)q_parent->winId());

#else
	params["externalWindowHandle"] = Ogre::StringConverter::toString((size_t)(HWND)winId());
#endif

	mRenderWindow = mRoot->createRenderWindow("View", width(), height(), true, &params);

    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

    mCamera = mSceneMgr->createCamera("PlayerCam 1");

    mCamera->setPosition(Ogre::Vector3(0,100,0));
    mCamera->lookAt(Ogre::Vector3(0,100,100));
    mCamera->setNearClipDistance(1);

    mVp = mRenderWindow->addViewport(mCamera);
	mVp->setBackgroundColour(ColourValue(0.58, 0.65, 0.76, 1));

    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    createScene();
	createFog();
	createLight();

	TextureManager::getSingleton().setDefaultNumMipmaps(5);
	MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
    MaterialManager::getSingleton().setDefaultAnisotropy(2);

    mCamera->setAspectRatio(Real(mVp->getActualWidth()) / Real(mVp->getActualHeight()));

#if !defined(Q_WS_WIN) // Linux
    WId window_id;
    mRenderWindow->getCustomAttribute("GLXWINDOW", &window_id);

    // Take over the ogre created window.
    QWidget::create(window_id);

    mRenderWindow->reposition(x(),y());
#endif

    startTimer(5);
};

void OgreView::paintEvent(QPaintEvent *evt)
{
	Q_UNUSED(evt);
    if(mRenderWindow == NULL)
        setupView();

	update();
}

void OgreView::timerEvent(QTimerEvent* evt)
{
	Q_UNUSED(evt);
	update();
	updateAnim();
}

void OgreView::update()
{
	if(mRenderWindow != NULL){
		mRoot->_fireFrameStarted();
		mRenderWindow->update();

		mCamera->moveRelative(mDirection);
		mCamera->yaw(Radian(angleX));
		mCamera->pitch(Radian(angleY));

		updateStats();
		mRoot->_fireFrameEnded();
	}
}

void OgreView::updateAnim()
{
	if(mainEntAnim != NULL)
		mainEntAnim->addTime(mRenderWindow->getBestFPS()/100000);
	//if(mainNode != NULL && mainEntPhys != NULL){
	//	mainNode->setPosition(mainEntPhys->getGlobalPosition());
	//	mainNode->setOrientation(mainEntPhys->getGlobalOrientation());
	//}
}

//***************************************************************************

//**************************** "Get" Properties******************************
Ogre::Entity* OgreView::getMainEnt()
{
	if(mainEnt != NULL)
		return mainEnt;
	else
		return 0;
}
//***************************************************************************

//********************************* Stats **********************************
float OgreView::getFPS()
{
	return FPS;
}

float OgreView::getTriangles()
{
	return triangles;
}

Vector3 OgreView::getPosCamera()
{
	return mCamera->getPosition();
}

Vector3 OgreView::getDirCamera()
{
	return mCamera->getDirection();
}

void OgreView::updateStats()
{
	try{
		const RenderTarget::FrameStats& stats = mRenderWindow->getStatistics();
		FPS = (int)stats.lastFPS;
		triangles = stats.triangleCount;
	}
	catch(...){

	}
}

//**************************************************************************

//*********************************Qt Control*******************************
void OgreView::resizeEvent(QResizeEvent *evt)
{
	Q_UNUSED(evt);
	if (mRenderWindow != NULL){
#if !defined(Q_WS_WIN) // Linux
		mRenderWindow->resize (width(), height());
#endif
		mRenderWindow->windowMovedOrResized();
		mCamera->setAspectRatio(Ogre::Real(mVp->getActualWidth()) / Ogre::Real(mVp->getActualHeight()));
	}
}

void OgreView::keyPressEvent(QKeyEvent* evt)
{
	if(mainEnt != NULL && mainNode != NULL){
		switch(evt->key()){
			case Qt::Key_W:
			case Qt::Key_Up:
				rotX = -0.1;
				mainNode->pitch(Radian(rotX));
				break;
			case Qt::Key_S:
			case Qt::Key_Down:
				rotX = 0.1;
				mainNode->pitch(Radian(rotX));
				break;
			case Qt::Key_A:
			case Qt::Key_Left:
				rotY = -0.1;
				mainNode->yaw(Radian(rotY));
				break;
			case Qt::Key_D:
			case Qt::Key_Right:
				rotY = 0.1;
				mainNode->yaw(Radian(rotY));
				break;
		}
	}
}

void OgreView::keyReleaseEvent(QKeyEvent* evt)
{
	if(mainEnt != NULL && mainNode != NULL){
		switch(evt->key()){
			case Qt::Key_W:
			case Qt::Key_Up:
				rotX = 0.0;
				mainNode->pitch(Radian(rotX));
				break;
			case Qt::Key_S:
			case Qt::Key_Down:
				rotX = 0.0;
				mainNode->pitch(Radian(rotX));
				break;
			case Qt::Key_A:
			case Qt::Key_Left:
				rotY = 0.0;
				mainNode->yaw(Radian(rotY));
				break;
			case Qt::Key_D:
			case Qt::Key_Right:
				rotY = 0.0;
				mainNode->yaw(Radian(rotY));
				break;
		}
	}
}

void OgreView::mousePressEvent(QMouseEvent* evt)
{
	if(evt->button() == Qt::LeftButton)
		mouseLeftPressed = true;
	if(evt->button() == Qt::RightButton){
		mouseRightPressed = true;
		mousePos = evt->pos();
	}
	if(evt->button() == Qt::MidButton)
		mouseMiddleBtn = true;
}

void OgreView::mouseReleaseEvent(QMouseEvent* evt)
{
	Q_UNUSED(evt);
	mouseLeftPressed = false;
	mouseRightPressed = false;
	mouseMiddleBtn = false;
}

void OgreView::mouseMoveEvent(QMouseEvent* evt)
{
	if(mouseMiddleBtn || mouseLeftPressed){
		QPoint currentPos = evt->pos();

		if(mousePos.x() < currentPos.x())
			mDirection.x = 1;
		else if(mousePos.x() > currentPos.x())
			mDirection.x = -1;
		else
			mDirection.x = 0;

		if(mousePos.y() < currentPos.y())
			mDirection.y = -1;
		else if(mousePos.y() > currentPos.y())
			mDirection.y = 1;
		else
			mDirection.y = 0;

		update();
		mousePos = currentPos;
		mDirection = Vector3::ZERO;
	}
	if(mouseRightPressed){
		QPoint currentPos = evt->pos();

		if(mousePos.x() < currentPos.x())
			angleX = -0.01;
		else if(mousePos.x() > currentPos.x())
			angleX = 0.01;
		else
			angleX = 0.00;

		if(mousePos.y() < currentPos.y())
			angleY = -0.01;
		else if(mousePos.y() > currentPos.y())
			angleY = 0.01;
		else
			angleY = 0.00;

		update();
		mousePos = currentPos;
		angleX = 0.00;
		angleY = 0.00;
	}
	//if(mouseLeftPressed){
	//	QPoint currentPos = evt->pos();

	//	if(mousePos.x() < currentPos.x())
	//		mDirection.x = -0.01;
	//	else if(mousePos.x() > currentPos.x())
	//		mDirection.x = 0.01;
	//	else
	//		mDirection.x = 0;
	//
	//	update();
	//	mousePos = currentPos;
	//	mDirection = Vector3::ZERO;
	//}
	//if(mouseRightPressed){
	//       QPoint currentPos = evt->pos();

	//	if(mousePos.x() < currentPos.x())
	//		mDirection.x = -0.01;
	//	else if(mousePos.x() > currentPos.x())
	//		mDirection.x = 0.01;
	//	else
	//		mDirection.x = 0;

	//	if(mousePos.y() < currentPos.y())
	//		mDirection.y = -0.01;
	//	else if(mousePos.y() > currentPos.y())
	//		mDirection.y = 0.01;
	//	else
	//		mDirection.y = 0;

	//	mousePos = currentPos;
 //       update();
	//	mDirection = Vector3::ZERO;
	//}
}

void OgreView::wheelEvent(QWheelEvent* evt)
{
	mDirection.z = -evt->delta()/12;
	update();
	mDirection.z = 0;
}
//**************************************************************************

//*************************Interactive Properties***************************
void OgreView::addObject(String name)
{
	//Update the name
	String meshName = name;
	meshName.substr(meshName.size()-5, 5);

	//Remove old object
	if(mainEnt != NULL)
	{
		mSceneMgr->destroyEntity(mainEnt);
		mainEntAnim = 0;
	}
	if(mainNode != NULL)
		mSceneMgr->destroySceneNode(mainNode->getName());

	//Add New Object
	mainEnt = mSceneMgr->createEntity(meshName, name);
	mainNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(meshName+"node");
	mainNode->attachObject(mainEnt);
	mainNode->setPosition(Vector3(0, 0, 0));

	//Update the camera's pos to fit whith the object size
	mCamera->setPosition(mainNode->getPosition().x, mainNode->getPosition().y, mainNode->getPosition().z - 200);
	mCamera->lookAt(mainNode->getPosition());
}

void OgreView::setFog(int fogType)
{
	switch(fogType){
		case 0:
			mSceneMgr->setFog(FOG_NONE);
			break;
		case 1:
			mSceneMgr->setFog(FOG_LINEAR, fogColor, 0.0, 50, 500);
			break;
		case 2:
			mSceneMgr->setFog(FOG_EXP, fogColor, 0.005);
			break;
		case 3:
			mSceneMgr->setFog(FOG_EXP2, fogColor, 0.003);
			break;
	}
}

void OgreView::setFogColor(QColor color)
{
	fogOldColor = color;
	fogColor.setAsARGB(color.rgba());
	if(mSceneMgr->getFogMode() == FOG_LINEAR)
		mSceneMgr->setFog(FOG_LINEAR, fogColor, mSceneMgr->getFogDensity(), mSceneMgr->getFogStart(), mSceneMgr->getFogEnd());
	else if(mSceneMgr->getFogMode() == FOG_EXP)
		mSceneMgr->setFog(FOG_EXP, fogColor, mSceneMgr->getFogDensity());
	else if(mSceneMgr->getFogMode() == FOG_EXP2)
		mSceneMgr->setFog(FOG_EXP2, fogColor, mSceneMgr->getFogDensity());

}

QColor OgreView::getFogColor()
{
	return fogOldColor;
}

void OgreView::setBackgroundColor(QColor color)
{
	backgroundOldColor = color;
	backgroundColor.setAsARGB(color.rgba());
	mVp->setBackgroundColour(backgroundColor);
}
QColor OgreView::getBackgroundColor()
{
	return backgroundOldColor;
}
void OgreView::setShadow(int fogType)
{
	if(fogType == 0)
		mSceneMgr->setShadowTechnique(SHADOWTYPE_NONE);
	else if(fogType == 1)
		mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);
	else if(fogType == 2)
		mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);
	else if(fogType == 3)
		mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);
}

void OgreView::setAmbientLight(QColor color)
{
	mSceneMgr->setAmbientLight(ColourValue(color.redF(), color.greenF(), color.blueF(), color.alphaF()));
	ambientOldColor = color;
}

QColor OgreView::getAmbientLight()
{
	return ambientOldColor;
}

void OgreView::setDiffuseColor(QColor color)
{
	diffuseOldColor = color;
}

QColor OgreView::getDiffuseColor()
{
	return diffuseOldColor;
}

void OgreView::setSpecularColor(QColor color)
{
	specularOldColor = color;
}

QColor OgreView::getSpecularColor()
{
	return specularOldColor;
}

void OgreView::createPointLight(String name, String xPos, String yPos, String zPos, String diffuse, String specular)
{
	Light* newLight = mSceneMgr->createLight(name);
	newLight->setPosition(StringConverter::parseReal(xPos), StringConverter::parseReal(yPos), StringConverter::parseReal(zPos));

	QColor diffuseColor(diffuse.c_str());
	QColor specularColor(specular.c_str());
	lightDiffuseColor.setAsRGBA(diffuseColor.rgba());
	lightSpecularColor.setAsRGBA(specularColor.rgba());
	diffuseOldColor = diffuseColor;
	specularOldColor = specularColor;
	newLight->setDiffuseColour(diffuseColor.redF(), diffuseColor.greenF(), diffuseColor.blueF());
	newLight->setSpecularColour(specularColor.redF(), specularColor.greenF(), specularColor.blueF());
}

void OgreView::createDirectionalLight(String name, String xDir, String yDir, String zDir, String diffuse, String specular)
{
	Light* newLight = mSceneMgr->createLight(name);
	newLight->setDirection(StringConverter::parseReal(xDir), StringConverter::parseReal(yDir), StringConverter::parseReal(zDir));

	QColor diffuseColor(diffuse.c_str());
	QColor specularColor(specular.c_str());
	lightDiffuseColor.setAsRGBA(diffuseColor.rgba());
	lightSpecularColor.setAsRGBA(specularColor.rgba());
	diffuseOldColor = diffuseColor;
	specularOldColor = specularColor;
	newLight->setDiffuseColour(diffuseColor.redF(), diffuseColor.greenF(), diffuseColor.blueF());
	newLight->setSpecularColour(specularColor.redF(), specularColor.greenF(), specularColor.blueF());
}

void OgreView::createSpotlight(String name, String xPos, String yPos, String zPos, String xDir, String yDir, String zDir, String diffuse, String specular)
{
	Light* newLight = mSceneMgr->createLight(name);
	newLight->setPosition(StringConverter::parseReal(xPos), StringConverter::parseReal(yPos), StringConverter::parseReal(zPos));
	newLight->setDirection(StringConverter::parseReal(xDir), StringConverter::parseReal(yDir), StringConverter::parseReal(zDir));

	QColor diffuseColor(diffuse.c_str());
	QColor specularColor(specular.c_str());
	lightDiffuseColor.setAsRGBA(diffuseColor.rgba());
	lightSpecularColor.setAsRGBA(specularColor.rgba());
	diffuseOldColor = diffuseColor;
	specularOldColor = specularColor;
	newLight->setDiffuseColour(diffuseColor.redF(), diffuseColor.greenF(), diffuseColor.blueF());
	newLight->setSpecularColour(specularColor.redF(), specularColor.greenF(), specularColor.blueF());
}

void OgreView::updatePointLight(String oldName, String name, String xPos, String yPos, String zPos, String diffuse, String specular)
{
	Light* newLight = NULL;
	if(oldName != "none"){//name has changed
		mSceneMgr->destroyLight(oldName);
		newLight = mSceneMgr->createLight(name);
	}
	else
		newLight = mSceneMgr->getLight(name);

	newLight->setPosition(StringConverter::parseReal(xPos), StringConverter::parseReal(yPos), StringConverter::parseReal(zPos));

	QColor diffuseColor(diffuse.c_str());
	QColor specularColor(specular.c_str());
	lightDiffuseColor.setAsRGBA(diffuseColor.rgba());
	lightSpecularColor.setAsRGBA(specularColor.rgba());
	diffuseOldColor = diffuseColor;
	specularOldColor = specularColor;
	newLight->setDiffuseColour(diffuseColor.redF(), diffuseColor.greenF(), diffuseColor.blueF());
	newLight->setSpecularColour(specularColor.redF(), specularColor.greenF(), specularColor.blueF());
}

void OgreView::updateDirectionalLight(String oldName, String name, String xDir, String yDir, String zDir, String diffuse, String specular)
{
	Light* newLight = NULL;
	if(oldName != "none"){//name has changed
		mSceneMgr->destroyLight(oldName);
		newLight = mSceneMgr->createLight(name);
	}
	else
		newLight = mSceneMgr->getLight(name);

	newLight->setDirection(StringConverter::parseReal(xDir), StringConverter::parseReal(yDir), StringConverter::parseReal(zDir));

	QColor diffuseColor(diffuse.c_str());
	QColor specularColor(specular.c_str());
	lightDiffuseColor.setAsRGBA(diffuseColor.rgba());
	lightSpecularColor.setAsRGBA(specularColor.rgba());
	diffuseOldColor = diffuseColor;
	specularOldColor = specularColor;
	newLight->setDiffuseColour(diffuseColor.redF(), diffuseColor.greenF(), diffuseColor.blueF());
	newLight->setSpecularColour(specularColor.redF(), specularColor.greenF(), specularColor.blueF());
}

void OgreView::updateSpotlight(String oldName, String name, String xPos, String yPos, String zPos, String xDir, String yDir, String zDir, String diffuse, String specular)
{
	Light* newLight = NULL;
	if(oldName != "none"){//name has changed
		mSceneMgr->destroyLight(oldName);
		newLight = mSceneMgr->createLight(name);
	}
	else
		newLight = mSceneMgr->getLight(name);

	newLight->setPosition(StringConverter::parseReal(xPos), StringConverter::parseReal(yPos), StringConverter::parseReal(zPos));
	newLight->setDirection(StringConverter::parseReal(xDir), StringConverter::parseReal(yDir), StringConverter::parseReal(zDir));

	QColor diffuseColor(diffuse.c_str());
	QColor specularColor(specular.c_str());
	lightDiffuseColor.setAsRGBA(diffuseColor.rgba());
	lightSpecularColor.setAsRGBA(specularColor.rgba());
	diffuseOldColor = diffuseColor;
	specularOldColor = specularColor;
	newLight->setDiffuseColour(diffuseColor.redF(), diffuseColor.greenF(), diffuseColor.blueF());
	newLight->setSpecularColour(specularColor.redF(), specularColor.greenF(), specularColor.blueF());
}

void OgreView::deleteLight(String name)
{
	mSceneMgr->destroyLight(name);
}

void OgreView::setRenderDetail(int value)
{
	switch(value){
		case 0:
			mCamera->setPolygonMode(PM_SOLID);
			break;
		case 1:
			mCamera->setPolygonMode(PM_WIREFRAME);
			break;
		case 2:
			mCamera->setPolygonMode(PM_POINTS);
			break;
	}
}

void OgreView::setLOD(int value)
{
	mainEnt->setMeshLodBias(value);
}

void OgreView::setBoundingBoxes(int value)
{
	if(value)
		mainNode->showBoundingBox(true);
	else
		mainNode->showBoundingBox(false);
}

void OgreView::setSkeleton(int value)
{
	if(value)
		mainEnt->setDisplaySkeleton(true);
	else
		mainEnt->setDisplaySkeleton(false);
}

void OgreView::updateMaterial()
{
	for(unsigned int i=0; i<mainEnt->getNumSubEntities(); ++i){
		mainSubEnt = mainEnt->getSubEntity(i);
		mainSubEnt->getMaterial()->reload();
	}
}

void OgreView::setAnimationState(Ogre::String name)
{
	if(mainEntAnim != NULL){
		mainEntAnim->setLoop(false);
		mainEntAnim->setEnabled(false);
	}
	mainEntAnim = mainEnt->getAnimationState(name);
	mainEntAnim->setLoop(false);
	if(isLoopOn)
		mainEntAnim->setLoop(true);
	if(isAnimEnabled)
		mainEntAnim->setEnabled(true);
}

void OgreView::setAnimLoop(bool enable)
{
	mainEntAnim->setLoop(enable);
	isLoopOn = enable;
}

void OgreView::setAnimEnabled(bool enable)
{
	mainEntAnim->setEnabled(enable);
	isAnimEnabled = enable;
}

String OgreView::getLog()
{
	String msgLog(mLogListener->getMsg().toStdString() + '\n');
	if(msgLog != oldMsgLog){//the message has been updated
		return msgLog;
		///////////////////////////////////////////////////////////
		// Why is this here? It never gets executed...
		///////////////////////////////////////////////////////////
		//oldMsgLog = msgLog;
		//QMessageBox::warning(this, msgLog.c_str(), oldMsgLog.c_str());
	}
	else
		return "";
}
//**************************************************************************

void OgreView::dragEnterEvent(QDragEnterEvent* evt)
{
#if defined(Q_WS_WIN)
    if(evt->mimeData()->hasUrls())
    {
        QRegExp fileMatcher("^/([A-Z]:/.*\\.mesh)$");
        if( fileMatcher.exactMatch(evt->mimeData()->urls().first().path()) )
            evt->acceptProposedAction();
    }
#else
	QRegExp fileMatcher("^file://(/.*\\.mesh)$");
	if( fileMatcher.exactMatch(evt->mimeData()->text()) )
		evt->acceptProposedAction();
#endif
};

void OgreView::dropEvent(QDropEvent* evt)
{
#if defined(Q_WS_WIN)
    QRegExp fileMatcher("^/([A-Z]:/.*\\.mesh)$");
    if( fileMatcher.exactMatch(evt->mimeData()->urls().first().path()) )
    {
        QString meshName = fileMatcher.cap(1);
        meshName.replace("/", "\\");
#else
    QRegExp fileMatcher("^file://(/.*\\.mesh)$");
    if( fileMatcher.exactMatch(evt->mimeData()->text()) )
    {
        QString meshName = fileMatcher.cap(1);
#endif
		//((MainWindow*)parentWidget())->loadObj(meshName);
        //evt->acceptProposedAction();
    }
};
