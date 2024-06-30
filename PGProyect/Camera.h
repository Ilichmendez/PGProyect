#pragma once

// Std. Includes
#include <vector>

// GL Includes
#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <stdio.h>
#include <iostream>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 6.0f;
const GLfloat SENSITIVTY = 0.25f;
const GLfloat ZOOM = 45.0f;

GLfloat MAP_MIN_X = -22.0f; // Left 10
GLfloat MAP_MAX_X = 16.0f;  // Right
GLfloat MAP_MIN_Y = -1.5f;  // Down
GLfloat MAP_MAX_Y = 25.0f;  // Up
GLfloat MAP_MIN_Z = -25.0f; // Front  17.0
GLfloat MAP_MAX_Z = 13.0f;  // behind

const GLfloat strikeX = -10.0;
const GLfloat strikeZ = -16.0;

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM)
    {
        this->position = position;
        this->worldUp = up;
        this->yaw = yaw;
        this->pitch = pitch;
        this->updateCameraVectors();
    }

    // Constructor with scalar values
    Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM)
    {
        this->position = glm::vec3(posX, posY, posZ);
        this->worldUp = glm::vec3(upX, upY, upZ);
        this->yaw = yaw;
        this->pitch = pitch;
        this->updateCameraVectors();
    }

    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(this->position, this->position + this->front, this->up);
    }

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
    {
        GLfloat velocity = this->movementSpeed * deltaTime;

        if (direction == FORWARD)
        {
            this->position += this->front * velocity;
        }

        if (direction == BACKWARD)
        {
            this->position -= this->front * velocity;
        }

        if (direction == LEFT)
        {
            this->position -= this->right * velocity;
        }

        if (direction == RIGHT)
        {
            this->position += this->right * velocity;
        }

        MAP_MIN_X = -22.0;
        // Keep the camera within the map limits
        if (this->position.x < MAP_MIN_X)
            this->position.x = MAP_MIN_X;
        if (this->position.x > MAP_MAX_X)
            this->position.x = MAP_MAX_X;
        if (this->position.y < MAP_MIN_Y)
            this->position.y = MAP_MIN_Y;
        if (this->position.y > MAP_MAX_Y)
            this->position.y = MAP_MAX_Y;
        if (this->position.z < MAP_MIN_Z)
            this->position.z = MAP_MIN_Z;
        if (this->position.z > MAP_MAX_Z)
            this->position.z = MAP_MAX_Z;

        // x = -10; z = -16
        if ((this->position.x < strikeX) && (this->position.z < strikeZ))
            this->position.z = strikeZ;
    }

    void posicion()
    {
        std::string posicion;
        posicion = "X: " + std::to_string(this->position.x) + "\nZ: " + std::to_string(this->position.z) + "\n";
        printf(posicion.c_str());
    }

    void Noche()
    {
        if ((this->position.y >= -1.6f && this->position.y <= 68.0f))
        {
            this->position.y = this->position.y + 141;
            MAP_MAX_Y = 165.7;
            MAP_MIN_Y = 139.5;
        }
    }

    void Dia()
    {
        if ((this->position.y >= 139.1f && this->position.y <= 208.8f))
        {
            this->position.y = this->position.y - 141;
            MAP_MIN_Y = -1.5f;
            MAP_MAX_Y = 25.0f;
        }
    }

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true)
    {
        xOffset *= this->mouseSensitivity;
        yOffset *= this->mouseSensitivity;

        this->yaw += xOffset;
        this->pitch += yOffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (this->pitch > 89.0f)
            {
                this->pitch = 89.0f;
            }

            if (this->pitch < -89.0f)
            {
                this->pitch = -89.0f;
            }
        }

        // Update Front, Right and Up Vectors using the updated Eular angles
        this->updateCameraVectors();
    }

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(GLfloat yOffset)
    {
    }

    GLfloat GetZoom()
    {
        return this->zoom;
    }

    glm::vec3 GetPosition()
    {
        return this->position;
    }

private:
    // Camera Attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    // Eular Angles
    GLfloat yaw;
    GLfloat pitch;

    // Camera options
    GLfloat movementSpeed;
    GLfloat mouseSensitivity;
    GLfloat zoom;

    // Calculates the front vector from the Camera's (updated) Eular Angles
    void updateCameraVectors()
    {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        front.y = sin(glm::radians(this->pitch));
        front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        this->front = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        this->right = glm::normalize(glm::cross(this->front, this->worldUp)); // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        this->up = glm::normalize(glm::cross(this->right, this->front));
    }
};