#pragma once

#include <Rendering/Essentials/Vertex.h>


namespace DragonCore::ECS
{
	struct UVs
	{
		float u{ 0.f }, v{ 0.f }, uv_width{ 0.f }, uv_height{ 0.f };
	};

	struct SpriteComponent
	{
		float width{ 0.f }, height{ 0.f };
		UVs uvs{ .u = 0.f,.v = 0.f,.uv_width = 0.f,.uv_height = 0.f };

		DragonRendering::Color color{ .R = 255,.G = 255,.B = 255,.A = 255 };

		int start_x{ 0 }, start_y{ 0 };

		void GenerateUVs(int TextureWidth, int TextureHeight)
		{
			uvs.uv_width = width / TextureWidth;
			uvs.uv_height = height / TextureHeight;

			uvs.u = start_x * uvs.uv_width;
			uvs.v = start_y * uvs.uv_height;
		}
	};
}