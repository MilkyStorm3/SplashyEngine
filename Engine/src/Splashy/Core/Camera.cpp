#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace ant {
	void PerspectiveCamera::SetProjection(float vFov, float nearClip, float farClip)
	{
		m_verticalFOV = vFov;
		m_nearClip = nearClip;
		m_farClip = farClip;
		m_reProject = true;
	}

	void PerspectiveCamera::SetPosition(const glm::vec3& position)
	{
		m_position = position;
		m_reView = true;
	}

	void PerspectiveCamera::Resize(const glm::vec2& viewport)
	{
		m_viewportDims = viewport;
		m_reProject = true;
	}

	void PerspectiveCamera::PerUpdate(ant::TimeStep ts)
	{
		if (m_reProject)
			CalculateProjection();

		if (m_reView)
			CalculateView();
	}
	void PerspectiveCamera::CalculateProjection()
	{
		m_projection = glm::perspectiveFov(
			glm::radians(m_verticalFOV),
			(float)m_viewportDims.x,
			(float)m_viewportDims.y,
			m_nearClip,
			m_farClip);

		m_reProject = false;
	}

	void PerspectiveCamera::CalculateView()
	{
		auto focusPoint = m_position + m_forwardDirection;
		m_view = glm::lookAt(m_position, focusPoint, m_upDirection);

		m_reView = false;
	}

}