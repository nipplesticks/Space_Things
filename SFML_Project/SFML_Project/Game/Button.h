#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <SFML/Graphics.hpp>
#include <functional>
#include "Globals.h"

template <class T, class V>
class Button
{
public:
    struct ButtonColor
    {
        sf::Color Idle = sf::Color(128,0,0);
        sf::Color Hover = sf::Color(255,0,0);
        sf::Color Press = sf::Color::White;
    };
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
    std::function<T, V> m_function;

    sf::RectangleShape m_background;
    ButtonColor m_backgroundColor;

    sf::RectangleShape m_foreground;
    ButtonColor m_foregroundColor;

    bool m_pressed;
    bool m_hasFunction;

    sf::Text m_text;
};

template <class T, class V>
inline Button<T,V>::Button()
{
    m_text.setFont(Global::g_font);
    m_text.setFillColor(sf::Color::Black);
    m_pressed = false;
    m_hasFunction = false;
    SetPosition(0, 0);
    SetSize(32, 32);
}
template <class T, class V>
inline void Button<T,V>::SetPosition(float x, float y)
{
    SetPosition({ x, y });
}
template <class T, class V>
inline void Button<T,V>::SetPosition(const sf::Vector2f& position)
{
    m_background.setPosition(position);
    m_foreground.setPosition(position);
    m_text.setPosition(position);
}
template <class T, class V>
inline void Button<T,V>::SetSize(float x, float y)
{
    SetSize({ x, y });
}
template <class T, class V>
inline void Button<T,V>::SetSize(const sf::Vector2f& size)
{
    SetBackgroundSize(size);
    SetForegroundSize(size);
}
template <class T, class V>
inline void Button<T,V>::SetBackgroundSize(float x, float y)
{
    SetBackgroundSize({ x, y });
}
template <class T, class V>
inline void Button<T,V>::SetBackgroundSize(const sf::Vector2f& size)
{
    m_background.setSize(size);
}
template <class T, class V>
inline void Button<T,V>::SetForegroundSize(float x, float y)
{
    SetForegroundSize({ x, y });
}
template <class T, class V>
inline void Button<T,V>::SetForegroundSize(const sf::Vector2f& size)
{
    m_foreground.setSize(size);
}
template <class T, class V>
inline void Button<T,V>::SetTextSize(int characterSize)
{
    m_text.setCharacterSize(characterSize);
}
template <class T, class V>
inline void Button<T,V>::SetTextColor(const sf::Color& col)
{
    m_text.setFillColor(col);
}
template <class T, class V>
inline void Button<T,V>::SetOrigin(float x, float y)
{
    SetOrigin({ x, y });
}
template <class T, class V>
inline void Button<T,V>::SetOrigin(const sf::Vector2f& origin)
{
    SetBackgroundOrigin(origin);
    SetForegroundOrigin(origin);
}
template <class T, class V>
inline void Button<T,V>::SetForegroundOrigin(float x, float y)
{
    SetForegroundOrigin({ x, y });
}
template <class T, class V>
inline void Button<T,V>::SetForegroundOrigin(const sf::Vector2f& origin)
{
    m_foreground.setOrigin(origin);
}
template <class T, class V>
inline void Button<T,V>::SetBackgroundOrigin(float x, float y)
{
    SetBackgroundOrigin({ x, y });
}
template <class T, class V>
inline void Button<T,V>::SetBackgroundOrigin(const sf::Vector2f& origin)
{
    m_background.setOrigin(origin);
}
template <class T, class V>
inline void Button<T,V>::SetTextOrigin(float x, float y)
{
    SetTextOrigin({ x, y });
}
template <class T, class V>
inline void Button<T,V>::SetTextOrigin(const sf::Vector2f& origin)
{
    m_text.setOrigin(origin);
}
template <class T, class V>
inline void Button<T,V>::SetTextString(const std::string& str)
{
    m_text.setString(str);
}
template <class T, class V>
inline void Button<T,V>::SetForegroundColor(const ButtonColor& color)
{
    m_foregroundColor = color;
}
template <class T, class V>
inline void Button<T,V>::SetBackgroundColor(const ButtonColor& color)
{
    m_backgroundColor = color;
}
template <class T, class V>
inline void Button<T,V>::SetBackgroundTexture(sf::Texture* texture)
{
    m_background.setTexture(texture);
}
template <class T, class V>
inline void Button<T,V>::SetForegroundTexture(sf::Texture* texture)
{
    m_foreground.setTexture(texture);
}
template <class T, class V>
inline void Button<T,V>::RegisterFunction(const std::function<void()>& function)
{
    m_function = function;
    m_hasFunction = true;
}
template <class T, class V>
inline const sf::Text& Button<T,V>::GetText() const
{
    return m_text;
}
template <class T, class V>
inline const sf::Vector2f& Button<T,V>::GetForegroundSize() const
{
    return m_foreground.getSize();
}
template <class T, class V>
inline const sf::Vector2f& Button<T,V>::GetBackgroundSize() const
{
    return m_background.getSize();
}
template <class T, class V>
inline const sf::Vector2f& Button<T,V>::GetPosition() const
{
    return m_background.getSize();
}
template <class T, class V>
inline void Button<T,V>::Update(float dt)
{
    bool press = sf::Mouse::isButtonPressed(sf::Mouse::Left);

    if (m_foreground.getGlobalBounds().contains(Global::g_mousePos))
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
}
template <class T, class V>
inline void Button<T,V>::Draw(sf::RenderWindow* wnd)
{
    wnd->draw(m_background);
    wnd->draw(m_foreground);
    wnd->draw(m_text);
}

#endif