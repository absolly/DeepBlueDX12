#include "DebugDrawer.h"

#include <iostream>
#include "GL/glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "glm.hpp"
#include "glm\common.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Camera.hpp"
#include "glm.hpp"
using std::cout;
using std::endl;

#include <stdio.h> //printf debugging

World* DebugDrawer::_world;
DebugRenderMaterial* DebugDrawer::_debugRenderMaterial;

void DebugDrawer::useShader()
{
	_debugRenderMaterial->render(*_world);
}

DebugDrawer::DebugDrawer()
	:m_debugMode(0)
{
	//_debugRenderMaterial = new DebugRenderMaterial(glm::vec3(1)); TODO
}

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	if (_world == nullptr) {
		return;
	}
	glm::vec3 newColor = glm::vec3(color.getX(), color.getY(), color.getZ());
	if (newColor.x == 1 && newColor.y == 1 && newColor.z == 1)
	{
		newColor = glm::vec3(0, 0.5f, 0);
	}
	_debugRenderMaterial->setDiffuseColor(newColor);


	glm::vec4 lineStart = glm::vec4(from.getX(), from.getY(), from.getZ(), 0);
	glm::vec4 lineEnd = glm::vec4(to.getX(), to.getY(), to.getZ(), 0);

	glm::vec4 glStartPosition = glm::vec4(lineStart.x, lineStart.y, lineStart.z, 1);
	glm::vec4 glEndPosition = glm::vec4(lineEnd.x, lineEnd.y, lineEnd.z, 1);

	//glEndPosition = mvp * glm::vec4(lineEnd.x, lineEnd.y, lineEnd.z, 1.f);

	//Start drawing
	//glColor3f(color.getX(), color.getY(), color.getZ());
	//glDisable(GL_LIGHTING);
	
	glBegin(GL_LINES);
	glColor3f(1, 1, 1);
	glVertex3d(glStartPosition.x, glStartPosition.y, glStartPosition.z);
	glColor3f(1, 1, 1);
	glVertex3d(glEndPosition.x, glEndPosition.y, glEndPosition.z);

	glEnd();
	//glEnable(GL_LIGHTING);
}

void    DebugDrawer::setDebugMode(int debugMode)
{
	m_debugMode = debugMode;
}

void    DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
	//glRasterPos3f(location.x(),  location.y(),  location.z());
	//BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),textString);
}

void    DebugDrawer::reportErrorWarning(const char* warningString)
{
	printf(warningString);
}

void    DebugDrawer::drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	{
		//btVector3 to=pointOnB+normalOnB*distance;
		//const btVector3&from = pointOnB;
		//glColor4f(color.getX(), color.getY(), color.getZ(), 1.0f);   

		//DebugDrawer::drawLine(from, to, color);

		//glRasterPos3f(from.x(),  from.y(),  from.z());
		//char buf[12];
		//sprintf(buf," %d",lifeTime);
		//BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),buf);
	}
}
