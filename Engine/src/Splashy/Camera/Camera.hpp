#pragma once
#include <glm/glm.hpp>
#include "Core/Core.hpp"

namespace ant
{
    struct CameraBoundaries
    {
        float left, right;
        float top, bottom;

        float GetWidth(){return right - left;}
        float GetHeight(){return top - bottom;}
    };
    
    class OrthographicCamera
    {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);
        OrthographicCamera() {}
        ~OrthographicCamera() {}

        void CalculateViewProjectionMatrix();
        const glm::mat4 &GetViewProjectionMatrix() { return m_viewProjectionMatrix; }

        void SetPosition(const glm::vec3 &position);
        glm::vec3 &SetPosition() { return m_translationVector; }
        const glm::vec3 GetPosition() const { return m_translationVector; }
        
        void SetRotation(float rotation);
        float &SetRotation() { return m_rotation; }
        float GetRotation() const { return m_rotation; }

        void SetProjection(float left, float right, float bottom, float top);

        float GetZoom() const { return m_zoom; }
        void SetZoom(float zoom){ m_zoom = zoom; }

        float GetAscpectRatio() const { return m_aspectRatio; }
        void SetAscpectRatio(float ratio){ m_aspectRatio = ratio; CalculateProjection(); }

        const CameraBoundaries& GetBoundaries() const {return m_boundaries;}

    private:
        friend class OrthographicCameraController;
        void CalculateProjection();
        
    private:
        glm::mat4 m_projectionMatrix;     // fov and aspect ratio
        glm::mat4 m_viewMatrix;           // where it is and where its looking, rotation
        glm::mat4 m_viewProjectionMatrix; // projectionMat * vievMat
        glm::vec3 m_translationVector;    // position
        float m_rotation = 0.f;
        float m_zoom = 1.f;
        float m_aspectRatio;
        CameraBoundaries m_boundaries;
    };

    class Event;
    class WindowRezisedEvent;

    class OrthographicCameraController
    {
    protected:
        OrthographicCameraController() {}

    public:
        ~OrthographicCameraController() {}
        virtual void OnUpdate() = 0;
        virtual void OnEvent(Event *event) = 0;
        inline OrthographicCamera &GetCamera() { return *m_camera; }
        inline Ref<OrthographicCamera> GetCameraRef() { return m_camera; }
        void SetCamera(Ref<OrthographicCamera> camera){m_camera = camera;}

        void OnWindowResized(WindowRezisedEvent * e);
        void OnResize(float x, float y);

    protected:
        Ref<OrthographicCamera> m_camera;
        
    };

} // namespace ant
