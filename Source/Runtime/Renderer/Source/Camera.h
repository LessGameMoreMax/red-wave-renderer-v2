#ifndef CAMERA_H
#define CAMERA_H

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <cmath>
#include <iostream>

namespace sablin{

inline static const float kYaw = -90.0f;
inline static const float kPitch = 0.0f;
inline static const float kSpeed = 1.5f;
inline static const float kSensitivity = 0.1f;
inline static const float kZoom =  45.0f;


class Camera
{
private:
    void UpdateCameraDirection()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
        front.y = sin(glm::radians(pitch_));
        front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));

        front_ = glm::normalize(front);

        right_ = glm::normalize(glm::cross(front_, world_up_)); 
        up_ = glm::normalize(glm::cross(right_, front_));
    }
public:
    enum class MoveDirection{
        kForward,
        kBackward,
        kLeft,
        kRight
    };
public:
    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 right_;
    glm::vec3 world_up_;

    float yaw_;
    float pitch_;

    float move_speed_;
    float mouse_sensitivity_;
    float zoom_;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
           float yaw = kYaw, 
           float pitch = kPitch) : 
        position_(position),
        front_(0.0f, 0.0f, -1.0f), 
        world_up_(up),
        yaw_(yaw),
        pitch_(pitch),
        move_speed_(kSpeed), 
        mouse_sensitivity_(kSensitivity),
        zoom_(kZoom)
    {
        UpdateCameraDirection();
    }

    Camera(float pos_x, float pos_y, float pos_z, float up_x, 
           float up_y, float up_z, float yaw, float pitch) :
        position_(pos_x, pos_y, pos_z),
        front_(0.0f, 0.0f, -1.0f),
        world_up_(up_x, up_y, up_z),
        yaw_(yaw),
        pitch_(pitch),
        move_speed_(kSpeed), 
        mouse_sensitivity_(kSensitivity),
        zoom_(kZoom)
    {
        UpdateCameraDirection();
    }

    glm::mat4 GetViewMatrix() {
        return glm::lookAt(position_, position_ + front_, world_up_);
    }

    void ProcessKeyboard(MoveDirection direction, float delta_time)
    {
        float velocity = move_speed_ * delta_time;
        switch(direction){
            case MoveDirection::kForward:
                position_ += front_ * velocity;
                break;
            case MoveDirection::kBackward:
                position_ -= front_ * velocity;
                break;
            case MoveDirection::kLeft:
                position_ -= right_ * velocity;
                break;
            case MoveDirection::kRight:
                position_ += right_ * velocity;
                break;
            default:
                break;
        }
    }

    void ProcessMouseMovement(float x_offset, float y_offset, GLboolean constrain_pitch = true)
    {
        x_offset *= mouse_sensitivity_;
        y_offset *= mouse_sensitivity_;

        yaw_   += x_offset;
        pitch_ += y_offset;

        if (constrain_pitch){
            if (pitch_ > 89.0f) pitch_ = 89.0f;
            if (pitch_ < -89.0f) pitch_ = -89.0f;
        }

        UpdateCameraDirection();
    }

    void ProcessMouseScroll(float y_offset)
    {
        zoom_ -= y_offset;
        if (zoom_ < 1.0f) zoom_ = 1.0f;
        if (zoom_ > 45.0f) zoom_ = 45.0f;
    }
};

}

#endif
