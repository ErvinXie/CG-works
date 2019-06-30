#include "Camera.h"

void Camera::rotate(float xoffset, float yoffset) {
    float rotateAngel = sqrt(xoffset * xoffset + yoffset * yoffset);
    if (rotateAngel == 0) {
        return;
    }
    glm::vec3 cameraRight = glm::cross(Front, Up);
    glm::vec3 cusorDirection = glm::normalize(xoffset * cameraRight + yoffset * Up);
    glm::vec3 rotationAxis = glm::normalize(glm::cross(cusorDirection, Front));
    rotate(rotationAxis, rotateAngel);
}

void Camera::rotate(glm::vec3 rotationAxis, float rotateAngel) {
    glm::quat ro = glm::angleAxis(glm::radians(rotateAngel), rotationAxis);
    glm::quat cameraFrontq = glm::quat(0, Front);
    cameraFrontq = glm::inverse(ro) * cameraFrontq * ro;
    glm::vec3 cameraFrontn = glm::vec3(cameraFrontq.x, cameraFrontq.y, cameraFrontq.z);
    Front = cameraFrontn;
    if (fixedY == 0) {
        glm::quat cameraUpq = glm::quat(0, Up);
        cameraUpq = glm::inverse(ro) * cameraUpq * ro;
        glm::vec3 cameraUpn = glm::vec3(cameraUpq.x, cameraUpq.y, cameraUpq.z);
        Up = cameraUpn;
    }
}

glm::mat4 Camera::View() {
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::setFov(float nfov) {
    if (nfov >= 1.0f && nfov <= max_fov)
        Fov = nfov;
}

void Camera::translate(glm::vec3 tra) {
    Position += tra;
}
