#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <SFML/Graphics.hpp>

class Camera
{
public:
    Camera(const sf::Vector2f& pos = sf::Vector2f(0, 0),
        const sf::Vector2f& size = sf::Vector2f(1280.0f, 720.0f),
        float rotation = 0);
    ~Camera();

    void SetAsActive();
    static Camera* GetActiveCamera();
    void SetPosition(const sf::Vector2f& position);
    void SetPosition(float x, float y);

    void SetSize(const sf::Vector2f& size);
    void SetSize(float x, float y);

    void SetRotation(float r);
    void SetZoom(float factor);

    void Move(const sf::Vector2f& dir, bool relative = false);
    void Move(float x, float y, bool relative = false);
    void IncrementSize(const sf::Vector2f& inc);
    void IncrementSize(float x, float y);
    void Rotate(float r);
    void IncrementZoom(float factor);
    void MultiplyZoom(float factor);

    const sf::Vector2f& GetPosition() const;
    const sf::Vector2f& GetSize() const;
    float GetRotation() const;
    float GetZoom() const;

    sf::Vector2f GetRelativeRight() const;
    sf::Vector2f GetRelativeUp() const;

private:
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    float m_rotation;
    float m_zoom;

    static Camera* ACTIVE_CAMERA;
    static Camera STANDART_CAMERA;
};


#endif