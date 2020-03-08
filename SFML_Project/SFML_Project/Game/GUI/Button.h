#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <SFML/Graphics.hpp>
#include <functional>
#include <DirectXMath.h>
#include "../Structures/Globals.h"
#include "../Structures/Camera.h"

struct ButtonColor
{
    sf::Color Idle = sf::Color(128, 0, 0);
    sf::Color Hover = sf::Color(255, 0, 0);
    sf::Color Press = sf::Color::White;
};

template <class T>
class Button
{
public:
    Button();
    ~Button() {}

    void SetPosition(float x, float y);
    void SetPosition(const sf::Vector2f& position);

    void SetSize(float x, float y);
    void SetSize(const sf::Vector2f& size);
    void SetBackgroundSize(float x, float y);
    void SetBackgroundSize(const sf::Vector2f& size);
    void SetForegroundSize(float x, float y);
    void SetForegroundSize(const sf::Vector2f& size);

    void SetTextSize(int characterSize);
    void SetTextColor(const sf::Color& col);

    void SetOrigin(float x, float y);
    void SetOrigin(const sf::Vector2f& origin);
    void SetForegroundOrigin(float x, float y);
    void SetForegroundOrigin(const sf::Vector2f& origin);
    void SetBackgroundOrigin(float x, float y);
    void SetBackgroundOrigin(const sf::Vector2f& origin);
    void SetTextOrigin(float x, float y);
    void SetTextOrigin(const sf::Vector2f& origin);
    void SetTextString(const std::string& str);

    void SetForegroundColor(const ButtonColor & color);
    void SetBackgroundColor(const ButtonColor & color);
    void SetBackgroundTexture(sf::Texture* texture);
    void SetForegroundTexture(sf::Texture* texture);

    void RegisterFunction(const std::function<void()> & function);

    const sf::Text& GetText() const;
    const sf::Vector2f& GetForegroundSize() const;
    const sf::Vector2f& GetBackgroundSize() const;
    const sf::Vector2f& GetPosition() const;

    void Update(float dt);

    void Draw(sf::RenderWindow* wnd);

private:
    bool _contains(const sf::Vector2f & p);
    void _adaptToCamera();

private:
    std::function<T> m_function;

    sf::RectangleShape m_background;
    ButtonColor m_backgroundColor;

    sf::RectangleShape m_foreground;
    ButtonColor m_foregroundColor;

    sf::Vector2f m_position;
    sf::Vector2f m_foregroundSize;
    sf::Vector2f m_backgroundSize;
    float m_characterSize;

    sf::Vector2f m_foregroundOrigin;
    sf::Vector2f m_backgroundOrigin;
    sf::Vector2f m_textOrigin;

    bool m_pressed;
    bool m_hasFunction;

    sf::Text m_text;
};

template <class T>
inline Button<T>::Button()
{
    m_text.setFont(Global::g_font);
    m_text.setFillColor(sf::Color::Black);
    m_pressed = false;
    m_hasFunction = false;
    m_foregroundOrigin = sf::Vector2f(0, 0);
    m_backgroundOrigin = sf::Vector2f(0, 0);
    m_textOrigin = sf::Vector2f(0, 0);
    SetPosition(0, 0);
    SetSize(32, 32);
    SetTextSize(m_text.getCharacterSize());
}
template <class T>
inline void Button<T>::SetPosition(float x, float y)
{
    SetPosition({ x, y });
}
template <class T>
inline void Button<T>::SetPosition(const sf::Vector2f& position)
{
    m_position = position;
}
template <class T>
inline void Button<T>::SetSize(float x, float y)
{
    SetSize({ x, y });
}
template <class T>
inline void Button<T>::SetSize(const sf::Vector2f& size)
{
    SetBackgroundSize(size);
    SetForegroundSize(size);
}
template <class T>
inline void Button<T>::SetBackgroundSize(float x, float y)
{
    SetBackgroundSize({ x, y });
}
template <class T>
inline void Button<T>::SetBackgroundSize(const sf::Vector2f& size)
{
    m_backgroundSize = size;
}
template <class T>
inline void Button<T>::SetForegroundSize(float x, float y)
{
    SetForegroundSize({ x, y });
}
template <class T>
inline void Button<T>::SetForegroundSize(const sf::Vector2f& size)
{
    m_foregroundSize = size;
}
template <class T>
inline void Button<T>::SetTextSize(int characterSize)
{
    m_characterSize = characterSize;
    m_text.setCharacterSize(characterSize);
}
template <class T>
inline void Button<T>::SetTextColor(const sf::Color& col)
{
    m_text.setFillColor(col);
}
template <class T>
inline void Button<T>::SetOrigin(float x, float y)
{
    SetOrigin({ x, y });
}
template <class T>
inline void Button<T>::SetOrigin(const sf::Vector2f& origin)
{
    SetBackgroundOrigin(origin);
    SetForegroundOrigin(origin);
}
template <class T>
inline void Button<T>::SetForegroundOrigin(float x, float y)
{
    SetForegroundOrigin({ x, y });
}
template <class T>
inline void Button<T>::SetForegroundOrigin(const sf::Vector2f& origin)
{
    m_foregroundOrigin = origin;
}
template <class T>
inline void Button<T>::SetBackgroundOrigin(float x, float y)
{
    SetBackgroundOrigin({ x, y });
}
template <class T>
inline void Button<T>::SetBackgroundOrigin(const sf::Vector2f& origin)
{
    m_backgroundOrigin = origin;
}
template <class T>
inline void Button<T>::SetTextOrigin(float x, float y)
{
    SetTextOrigin({ x, y });
}
template <class T>
inline void Button<T>::SetTextOrigin(const sf::Vector2f& origin)
{
    m_textOrigin = origin;
}
template <class T>
inline void Button<T>::SetTextString(const std::string& str)
{
    m_text.setString(str);
}
template <class T>
inline void Button<T>::SetForegroundColor(const ButtonColor& color)
{
    m_foregroundColor = color;
}
template <class T>
inline void Button<T>::SetBackgroundColor(const ButtonColor& color)
{
    m_backgroundColor = color;
}
template <class T>
inline void Button<T>::SetBackgroundTexture(sf::Texture* texture)
{
    m_background.setTexture(texture);
}
template <class T>
inline void Button<T>::SetForegroundTexture(sf::Texture* texture)
{
    m_foreground.setTexture(texture);
}
template <class T>
inline void Button<T>::RegisterFunction(const std::function<void()>& function)
{
    m_function = function;
    m_hasFunction = true;
}
template <class T>
inline const sf::Text& Button<T>::GetText() const
{
    return m_text;
}
template <class T>
inline const sf::Vector2f& Button<T>::GetForegroundSize() const
{

    return m_foregroundSize;
}
template <class T>
inline const sf::Vector2f& Button<T>::GetBackgroundSize() const
{
    return m_backgroundSize;
}
template <class T>
inline const sf::Vector2f& Button<T>::GetPosition() const
{
    return m_position;
}
template <class T>
inline void Button<T>::Update(float dt)
{
    bool press = sf::Mouse::isButtonPressed(sf::Mouse::Left);

    // TODO, take care of rotation
    if (_contains(Global::g_mousePos))
    {
        m_background.setFillColor(m_backgroundColor.Hover);
        m_foreground.setFillColor(m_foregroundColor.Hover);


        if (press)
        {
            m_background.setFillColor(m_backgroundColor.Press);
            m_foreground.setFillColor(m_foregroundColor.Press);
        }

        if (m_hasFunction && !press && m_pressed)
        {
            m_function();
            m_background.setFillColor(m_backgroundColor.Idle);
            m_foreground.setFillColor(m_foregroundColor.Idle);
        }
    }
    else
    {
        m_background.setFillColor(m_backgroundColor.Idle);
        m_foreground.setFillColor(m_foregroundColor.Idle);
    }
    m_pressed = press;
    _adaptToCamera();
}
template <class T>
inline void Button<T>::Draw(sf::RenderWindow* wnd)
{
    wnd->draw(m_background);
    wnd->draw(m_foreground);
    wnd->draw(m_text);
}

template<class T>
inline bool Button<T>::_contains(const sf::Vector2f& p)
{
    auto lb = m_foreground.getLocalBounds();
    return lb.contains(m_foreground.getTransform().getInverse().transformPoint(p));
}
template<class T>
inline void Button<T>::_adaptToCamera()
{
    Camera* c = Camera::GetActiveCamera();

    sf::Vector2f cPos = c->GetPosition();
    float cZoom = c->GetZoom();
    sf::Vector2f cSize = c->GetSize();
    float cRot = c->GetRotation();

    m_background.setSize(m_backgroundSize * cZoom);
    m_background.setOrigin(sf::Vector2f(
        m_backgroundSize.x * cZoom * m_backgroundOrigin.x,
        m_backgroundSize.y * cZoom * m_backgroundOrigin.y
    ));
    m_foreground.setSize(m_foregroundSize * cZoom);
    m_foreground.setOrigin(sf::Vector2f(
        m_foregroundSize.x * cZoom * m_foregroundOrigin.x,
        m_foregroundSize.y * cZoom * m_foregroundOrigin.y
    ));
    m_text.setScale(sf::Vector2f(cZoom, cZoom));

    auto fr = m_text.getLocalBounds();
    m_text.setOrigin(
        fr.width * m_textOrigin.x,
        fr.height * m_textOrigin.y
    );
    m_background.setRotation(cRot);
    m_foreground.setRotation(cRot);
    m_text.setRotation(cRot);

    using namespace DirectX;

    sf::Vector2f unRotatedPosition = (-cSize * 0.5f + m_position) * cZoom;
    XMFLOAT3 urp = { unRotatedPosition.x, 0.0f, unRotatedPosition.y };
    XMFLOAT3 rp;
    XMStoreFloat3(&rp, XMVector3Transform(XMLoadFloat3(&urp), XMMatrixRotationRollPitchYaw(0.0f, Global::DegreeseToRadians(-cRot), 0.0f)));


    sf::Vector2f rotatedPosition(rp.x, rp.z);
    rotatedPosition = rotatedPosition + cPos;

    m_background.setPosition(rotatedPosition);
    m_foreground.setPosition(rotatedPosition);
    m_text.setPosition(rotatedPosition);

}

#endif