#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace ant
{

	void Camera::OnUpdate(ant::TimeStep ts)
	{
		OnLogicUpdate(ts);
		if (m_reProject)
			CalculateProjection();

		if (m_reView)
			CalculateView();
		m_projectionView = m_projection * m_view;
	}

	void PerspectiveCamera::SetProjection(float vFov, float nearClip, float farClip)
	{
		m_verticalFOV = vFov;
		m_nearClip = nearClip;
		m_farClip = farClip;
		m_reProject = true;
	}

	void PerspectiveCamera::SetPosition(const glm::vec3 &position)
	{
		m_position = position;
		m_reView = true;
	}

	void PerspectiveCamera::Resize(const glm::vec2 &viewport)
	{
		m_viewportDims = viewport;
		m_reProject = true;
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

	void Camera::SetFarClip(float p_far)
	{
		m_farClip = p_far;
		m_reProject = true;
	}

	void Camera::SetNearClip(float p_near)
	{
		m_nearClip = p_near;
		m_reProject = true;
	}

	void OrtographicCamera::SetOrtographic(float p_size, float p_near, float p_far)
	{
		m_size = p_size;
		m_near = p_near;
		m_far = p_far;
		m_reProject = true;
	}

	void OrtographicCamera::CalculateProjection()
	{
		float left = -m_size * m_aspectRatio * 0.5f;
		float right = m_size * m_aspectRatio * 0.5f;
		float bottom = -m_size * 0.5f;
		float top = m_size * 0.5f;

		m_projection = glm::ortho(left, right, bottom, top, m_near, m_far);
		m_reProject = false;
	}

	void OrtographicCamera::CalculateView()
	{
		m_reView = false;
	}

	void OrtographicCamera::SetAspectRatio(float ratio)
	{
		m_aspectRatio = ratio;
		m_reProject = true;
	}

	void OrtographicCamera::SetOrtographicSize(float size)
	{
		m_size = size;
		m_reProject = true;
	}

}