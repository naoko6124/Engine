#pragma once
#pragma once
#include "pch.hpp"
#include "Entity.hpp"

namespace Engine
{
    class Camera : public Entity
    {
    public:
        glm::mat4 GetCameraMatrix()
        {
            glm::vec3 direction;
            direction.x = cos(glm::radians(transform.rotation.y - 90.0f)) * cos(glm::radians(transform.rotation.x));
            direction.y = sin(glm::radians(transform.rotation.x));
            direction.z = sin(glm::radians(transform.rotation.y - 90.0f)) * cos(glm::radians(transform.rotation.x));
            front = glm::normalize(direction);
            right = glm::normalize(glm::cross(front, up));
            glm::mat4 view = glm::lookAt(transform.position, transform.position + front, up);
            glm::mat4 proj = glm::perspective(glm::radians(fov), aspect, near, far);
            glm::mat4 result = proj * view;
            return result;
        }
    public:
        float fov = 45.0f;
        float aspect = 16.0f/9.0f;
        float near = 0.1f;
        float far = 1000.0f;
    private:
        glm::vec3 right = glm::vec3(1, 0, 0);
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 front = glm::vec3(0, 0, 1);
    };
}