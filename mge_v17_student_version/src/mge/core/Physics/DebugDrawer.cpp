#include "DebugDrawer.h"

#include <iostream>
#include "GL/glew.h"
#include <GL/gl.h>
#include <GL/glu.h>

using std::cout;
using std::endl;

#include <stdio.h> //printf debugging

DebugDrawer::DebugDrawer()
	:m_debugMode(0)
{

}

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	//      if (m_debugMode > 0)
	{
		glDisable(GL_LIGHTING);
		float tmp[6] = { 10,10,10, 20,20,20 };
		glPushMatrix();
		{
			glLineWidth(5.0f);
			glColor4f(color.getX(), color.getY(), color.getZ(), 1.0f);
			glVertexPointer(3,
				GL_FLOAT,
				0,
				&tmp);

			glPointSize(5.0f);
			glDrawArrays(GL_POINTS, 0, 2);
			glDrawArrays(GL_LINES, 0, 2);
		}
		glPopMatrix();
	}
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
