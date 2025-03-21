
#include "camera.hpp"



#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

namespace TTe {

glm::mat4 Camera::getViewMatrix() {
    const float yaw = transform.rot->y;
    // transform.rot.
    const float pitch = transform.rot->x;

    glm::vec3 forward{
        std::sin(yaw) * std::cos(pitch),
        std::sin(pitch),
        std::cos(yaw) * std::cos(pitch)
    };

    // Position cible en fonction de la direction
    glm::vec3 target = transform.pos + forward;

    return glm::lookAt(transform.pos.value, target,  up);
}

glm::mat4 Camera::getInvViewMatrix() { return glm::inverse(getViewMatrix()); }

glm::mat4 Camera::getProjectionMatrix() { return glm::perspective(glm::radians(fov), ((float) extent.width / (float) extent.height), near, far); }
}  // namespace TTe