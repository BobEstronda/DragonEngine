#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace DragonRendering
{
	class Camera2D
	{
	private:
		int m_Width, m_Height;
		float m_Scale;

		glm::vec2 m_Position;
		glm::mat4 m_CameraMatrix, m_OrthoProjection;

		bool m_bNeedsUpdate;
	public:
		Camera2D();
		Camera2D(int Width, int Height);
		void Update();

		inline void SetPosition(glm::vec2 NewPosition) { m_Position = NewPosition; m_bNeedsUpdate = true; }
		inline void SetScale(float scale) { m_Scale = scale; m_bNeedsUpdate = true; }

		inline const glm::vec2 GetPosition() const { return m_Position; }
		inline const float GetScale() const { return m_Scale; }

		inline glm::mat4 GetCameraMatrix() { return m_CameraMatrix; }
	};
}