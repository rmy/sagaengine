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

#ifndef __OgreView_h__
#define __OgreView_h__

#include <QWidget>
#include <QMouseEvent>
#include <QMessageBox>
#include <QRgb>
#include <QColor>
#include <QString>

#include <Ogre.h>
#if (OGRE_PLATFORM  == OGRE_PLATFORM_WIN32)
#include <windows.h>
#endif

class myLogListener : public Ogre::LogListener, QWidget
{
public:
	myLogListener(QWidget* parent = 0);
	~myLogListener();

	void write(const Ogre::String &name, const Ogre::String &message,
			   Ogre::LogMessageLevel lml=Ogre::LML_CRITICAL, bool maskDebug=true);
	QString getMsg();

private:
	QString logMsg;
};

class OgreView : public QWidget
{
	Q_OBJECT

public:
	OgreView(QWidget* parent = 0);
	~OgreView();

	Ogre::Entity* getMainEnt();

	//Control the Scene
	void addObject(Ogre::String);

	//-->Fog
	void setFog(int);
	void setFogColor(QColor);
	QColor getFogColor();

	//-->Background
	void setBackgroundColor(QColor);
	QColor getBackgroundColor();

	//-->Shadow
	void setShadow(int);

	//-->Light
	void setAmbientLight(QColor);
	QColor getAmbientLight();
	void setDiffuseColor(QColor);
	QColor getDiffuseColor();
	void setSpecularColor(QColor);
	QColor getSpecularColor();
	void createPointLight(Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String);//name, xPos, yPos, zPos, diffuse color, specular color
	void createDirectionalLight(Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String);//name, xDir, yDir, zDir, diffuse color, specular color
	void createSpotlight(Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String);//name, xPos, yPos, zPos, xDir, yDir, zDir, diffuse color, specular color
	void updatePointLight(Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String);//oldName, name, xPos, yPos, zPos, diffuse color, specular color
	void updateDirectionalLight(Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String);//oldName, name, xDir, yDir, zDir, diffuse color, specular color
	void updateSpotlight(Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String, Ogre::String);//oldName, name, xPos, yPos, zPos, xDir, yDir, zDir, diffuse color, specular color
	void deleteLight(Ogre::String);//name

	//-->Display
	void setRenderDetail(int);
	void setLOD(int);
	void setBoundingBoxes(int);
	void setSkeleton(int);

	//-->Material
	void updateMaterial();

	//-->Animation
	void setAnimationState(Ogre::String);
	void setAnimLoop(bool);
	void setAnimEnabled(bool);

	//Log
	Ogre::String getLog();
	///////////////////

	//Stats
	float getFPS();
	float getTriangles();
	Ogre::Vector3 getPosCamera();
	Ogre::Vector3 getDirCamera();
	///////

	//Entity
	Ogre::Entity* mainEnt;
	Ogre::AnimationState* mainEntAnim;
	////////

protected:
    void setupView();

	void update();
	void updateAnim();
	void setupResources();
	void updateStats();

	void createScene();
	void createPhys();
	void createFog();
	void createLight();

	void resizeEvent(QResizeEvent* evt);
	void timerEvent(QTimerEvent* evt);
	void paintEvent(QPaintEvent* evt);

	void dragEnterEvent(QDragEnterEvent* evt);
	void dropEvent(QDropEvent* evt);

	void keyPressEvent(QKeyEvent* evt);
	void keyReleaseEvent(QKeyEvent* evt);
	void mousePressEvent(QMouseEvent* evt);
	void mouseReleaseEvent(QMouseEvent* evt);
	void mouseMoveEvent(QMouseEvent* evt);
	void wheelEvent(QWheelEvent* evt);

	//Setup
	Ogre::RenderWindow* mRenderWindow;
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;
	Ogre::Viewport* mVp;
	Ogre::Root* mRoot;
	///////

	//Deplacement
	Ogre::Vector3 mDirection;
	Ogre::Real mRotate;
	Ogre::Real mMove;
	bool isMoving;
	QPoint mousePressPos;
	QPoint mousePos;
	float angleX, angleY;
	float rotX, rotY;
	////////////

	//Entity
	Ogre::SceneNode* mainNode;
	Ogre::SubEntity* mainSubEnt;
	////////

	//Base Plane
	//Ogre::MovablePlane* mPlane;
	Ogre::Plane* mPlane;
	Ogre::Entity* plane;
	Ogre::SceneNode* planeNode;
	////////////

	//Light
	Ogre::Light* mainLight;
	Ogre::ColourValue lightDiffuseColor;
	Ogre::ColourValue lightSpecularColor;
	Ogre::ColourValue ambientLightColor;
	QColor ambientOldColor;
	QColor diffuseOldColor;
	QColor specularOldColor;
	///////

	//Fog
	Ogre::ColourValue fogColor;
	QColor fogOldColor;//to keep the last selected color
	Ogre::ColourValue backgroundColor;
	QColor backgroundOldColor;
	/////

	//Ogre Stats
	int FPS;
	int triangles;
	Ogre::String oldMsgLog;
	////////////

	QList<Ogre::Entity*> entities;

	myLogListener* mLogListener;

	bool mouseLeftPressed;
	bool mouseRightPressed;
	bool mouseMiddleBtn;
	bool isLoopOn;
	bool isAnimEnabled;
};

#endif // __OgreView_h__
