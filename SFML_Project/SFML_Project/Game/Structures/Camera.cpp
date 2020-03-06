#include "Camera.h"
#include "Globals.h"

Camera Camera::STANDART_CAMERA;
Camera* Camera::ACTIVE_CAMERA = &Camera::STANDART_CAMERA;

Camera::Camera(const sf::Vector2f& pos, const sf::Vector2f& size, float rotation)
{
    m_position = pos;
    m_size = size;
    m_rotation = rotation;
    m_zoom = 1.0f;
}

Camera::~Camera()
{
    if (ACTIVE_CAMERA == this)
        ACTIVE_CAMERA = &STANDART_CAMERA;
}

void Camera::SetAsActive()
{
    ACTIVE_CAMERA = this;
}

Camera* Camera::GetActiveCamera()
{
    return ACTIVE_CAMERA;
}

void Camera::SetPosition(const sf::Vector2f& position)
{
    m_position = position;
}

void Camera::SetPosition(float x, float y)
{
    SetPosition({ x, y });
}

void Camera::SetSize(const sf::Vector2f& size)
{
    m_size = size;
}

void Camera::SetSize(float x, float y)
{
    SetSize({ x, y });
}

void Camera::SetRotation(float r)
{
    m_rotation = r;
}

void Camera::SetZoom(float factor)
{
    m_zoom = factor;
    m_zoom = std::fmax(m_zoom, 0.1f);
}

void Camera::Move(const sf::Vector2f& dir, bool relative)
{
    if (!relative)
        SetPosition(m_position + dir);
    else
    {
        float rot = Global::DegreeseToRadians(m_rotation);
        float x = dir.x * std::cos(rot) - dir.y * std::sin(rot);
        float y = dir.x * std::sin(rot) + dir.y * std::cos(rot);

        SetPosition(m_position + sf::Vector2f(x, y));
    }
}

void Camera::Move(float x, float y, bool relative)
{
    Move({ x, y }, relative);
}

void Camera::IncrementSize(const sf::Vector2f& inc)
{
    SetSize(m_size + inc);
}

void Camera::IncrementSize(float x, float y)
{
    IncrementSize({ x, y });
}

void Camera::Rotate(float r)
{
    m_rotation += r;
}

void Camera::IncrementZoom(float factor)
{
    m_zoom += factor;
    m_zoom = std::fmax(m_zoom, 0.1f);
}

void Camera::MultiplyZoom(float factor)
{
    m_zoom *= factor;
    m_zoom = std::fmax(m_zoom, 0.1f);
}

const sf::Vector2f& Camera::GetPosition() const
{
    return m_position;
}

const sf::Vector2f& Camera::GetSize() const
{
    return m_size;
}

float Camera::GetRotation() const
{
    return m_rotation;
}

float Camera::GetZoom() const
{
    return m_zoom;
}
