#include "DebugDrawer.h"

#include <iostream>
#include "GL/glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "glm.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Camera.hpp"
using std::cout;
using std::endl;

#include <stdio.h> //printf debugging

DebugDrawer::DebugDrawer()
	:m_debugMode(0)
{

}

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	if (_mainCamera == nullptr) {
		return;
	}
	glBegin(GL_LINES);
	glm::vec3 test = glm::vec3(_mainCamera->getMainCamera()->getWorldTransform() * glm::vec4(from.getX(),from.getY(),from.getZ(),0));
	test *= .1;
	glColor3f(color.getX(), color.getY(), color.getZ());
	glVertex3d(test.x,test.y,test.z);
	glColor3f(color.getX(), color.getY(), color.getZ());
	glm::vec3 test2 = glm::vec3(_mainCamera->getMainCamera()->getWorldTransform() * glm::vec4(to.getX(), to.getY(), to.getZ(), 0));
	test2 *= .1;
	glVertex3d(test2.x, test2.y, test2.z);
	glEnd();
	
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
		btVector3 to=pointOnB+normalOnB*distance;
		const btVector3&from = pointOnB;
		glColor4f(color.getX(), color.getY(), color.getZ(), 1.0f);   

		DebugDrawer::drawLine(from, to, color);

		glRasterPos3f(from.x(),  from.y(),  from.z());
		//char buf[12];
		//sprintf(buf," %d",lifeTime);
		//BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),buf);
	}
}
