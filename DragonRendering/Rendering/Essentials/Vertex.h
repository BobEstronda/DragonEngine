#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace DragonRendering
{
	struct Color
	{
		GLubyte R, G, B, A;
	};

	struct Vertex
	{
		glm::vec2 position{ 0.f }, uvs{ 0.f };
		Color color{ .R = 255, .G = 0, .B = 255, .A = 255 };

		void set_color(GLubyte R, GLubyte G, GLubyte B, GLubyte A)
		{
			color.R = R;
			color.G = G;
			color.B = B;
			color.A = A;
		}

		/*
		* 0xff00ac
		*       R         G         B         A
		* 0b1111 1111 0000 0000 1111 1111 1010 1100
		*/

		void set_color(GLuint NewColor)
		{
			color.R = (NewColor >> 24) & 0xFF;
			color.G = (NewColor >> 16) & 0xFF;
			color.B = (NewColor >> 8) & 0xFF;
			color.A = (NewColor >> 0) & 0xFF;
		}
	};
}