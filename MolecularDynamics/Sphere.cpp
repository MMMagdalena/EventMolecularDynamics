#include "stdafx.h"
#include "Sphere.h"

#include "Constants.h"

#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace OpenGL {

	Sphere::Sphere(float r, unsigned int lats, unsigned int longs, bool useTexture)
		: m_lats(lats), m_longs(longs)
	{
		std::vector<GLfloat> buffer;
		
		const int STRIDE_SIZE = (useTexture ? 8 : 6); // 3 for vertex, 3 for normal, 2 for texture coordinates
		const int STEP_SIZE = 2 * STRIDE_SIZE; // 2 vertices at once

		unsigned int length = (lats + 1) * (longs + 1) * STEP_SIZE;
		buffer.resize(length); 

		for (unsigned int i = 0; i < lats; ++i) {
			double lat0 = M_PI * (-0.5 + ((double)i) / (double)lats);
			double z0 = r*sin(lat0);
			double r0 = r*cos(lat0);

			double lat1 = M_PI * (-0.5 + ((double)(i + 1)) / (double)lats);
			double z1 = r*sin(lat1);
			double r1 = r*cos(lat1);

			for (unsigned int j = 0; j <= longs; ++j) {
				double longitude = 2. * M_PI * ((double)j) / (double)longs;

				double c = cos(longitude);
				double s = sin(longitude);

				double x = r0 * c;
				double y = r0 * s;

				double XTex = ((double)j) / ((double)longs);
				double YTex0 = ((double)i) / ((double)lats);
				double YTex1 = (((double)i) + 1.) / ((double)lats);

				double lng = sqrt(x*x + y*y + z0*z0);

				double x1 = r1 * c;
				double y1 = r1 * s;
				double lng1 = sqrt(x1*x1 + y1*y1 + z1*z1);

				// vertex
				buffer[i*(longs + 1) * STEP_SIZE + j * STEP_SIZE] = (float)x1;
				buffer[i*(longs + 1) * STEP_SIZE + j * STEP_SIZE + 1] = (float)y1;
				buffer[i*(longs + 1) * STEP_SIZE + j * STEP_SIZE + 2] = (float)z1;

				// normal
				buffer[i*(longs + 1) * STEP_SIZE + j * STEP_SIZE + 3] = (float)(x1 / lng1);
				buffer[i*(longs + 1) * STEP_SIZE + j * STEP_SIZE + 4] = (float)(y1 / lng1);
				buffer[i*(longs + 1) * STEP_SIZE + j * STEP_SIZE + 5] = (float)(z1 / lng1);

				// texture coordinate
				if (useTexture)
				{
					buffer[i*(longs + 1) * STEP_SIZE + j * STEP_SIZE + 6] = (float)XTex;
					buffer[i*(longs + 1) * STEP_SIZE + j * STEP_SIZE + 7] = (float)YTex1;
				}


				// vertex 
				buffer[i*(longs + 1) * STEP_SIZE + j * STEP_SIZE + 6 + (useTexture ? 2 : 0)] = (float)x;
				buffer[i*(longs + 1) * STEP_SIZE + j * STEP_SIZE + 7 + (useTexture ? 2 : 0)] = (float)y;
				buffer[i*(longs + 1) * STEP_SIZE + j * STEP_SIZE + 8 + (useTexture ? 2 : 0)] = (float)z0;

				// normal
				buffer[i*(longs + 1) * STEP_SIZE + j * STEP_SIZE + 9 + (useTexture ? 2 : 0)] = (float)(x / lng);
				buffer[i*(longs + 1) * STEP_SIZE + j * STEP_SIZE + 10 + (useTexture ? 2 : 0)] = (float)(y / lng);
				buffer[i*(longs + 1) * STEP_SIZE + j * STEP_SIZE + 11 + (useTexture ? 2 : 0)] = (float)(z0 / lng);

				// texture coordinate
				if (useTexture)
				{
					buffer[i*(longs + 1) * STEP_SIZE + j * STEP_SIZE + 14] = (float)XTex;
					buffer[i*(longs + 1) * STEP_SIZE + j * STEP_SIZE + 15] = (float)YTex0;
				}
			}
		}

		Renderable::Bind();
		setData(buffer.data(), length * sizeof(GLfloat));

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE_SIZE * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, STRIDE_SIZE * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		
		if (useTexture)
		{
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, STRIDE_SIZE * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		}
	}


	Sphere::~Sphere()
	{
	}

	void Sphere::Draw()
	{
		VertexBufferRenderable::Bind();

		for (unsigned int i = 0; i < m_lats; ++i)			
			glDrawArrays(GL_TRIANGLE_STRIP, (GLint)(i * (m_longs + 1) * 2 ) , (GLsizei)(2 * (m_longs + 1)));

		//VertexBufferRenderable::UnBind();
	}

	void Sphere::DrawInstanced(unsigned int count)
	{
		VertexBufferRenderable::Bind();

		for (unsigned int i = 0; i < m_lats; ++i)
		    glDrawArraysInstanced(GL_TRIANGLE_STRIP, (GLint)(i * (m_longs + 1) * 2), (GLsizei)(2 * (m_longs + 1)), count);
	}

}