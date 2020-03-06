#include "Bar.h"
#include "../Structures/Globals.h"
Bar::Bar()
{
    m_maxValue = 100.0f;
    m_currentValue = 100.0f;
    m_useMinMaxColor = false;
    m_minColor = sf::Color::Red;
    m_maxColor = sf::Color::Green;
    m_foreground.setSize(sf::Vector2f(100.0f, 25.0f));
    m_background.setSize(sf::Vector2f(100.0f, 25.0f));
    m_background.setFillColor(sf::Color::Transparent);
    m_isVertical = false;
}

void Bar::SetPosition(float x, float y)
{
    SetPosition({ x, y });
}

void Bar::SetPosition(const sf::Vector2f& position)
{
    m_foreground.setPosition(position);
    m_background.setPosition(position);
}

void Bar::SetSize(float x, float y)
{
    SetSize({ x, y });
}

void Bar::SetSize(const sf::Vector2f& size)
{
    m_foreground.setSize(size);
    m_background.setSize(size);
}

void Bar::SetOrigin(float x, float y)
{
    SetOrigin({ x, y });
}

void Bar::SetOrigin(const sf::Vector2f& origin)
{
    m_foreground.setOrigin(origin);
    m_background.setOrigin(origin);
}

void Bar::SetForegroundColor(int r, int g, int b, int a)
{
    SetForegroundColor(sf::Color(r, g, b, a));
}

void Bar::SetForegroundColor(const sf::Color& color)
{
    m_foreground.setFillColor(color);
}

void Bar::SetBackgroundColor(int r, int g, int b, int a)
{
    SetBackgroundColor(sf::Color(r, g, b, a));
}

void Bar::SetBackgroundColor(const sf::Color& color)
{
    m_background.setFillColor(color);
}

void Bar::SetForegroundMaxColor(int r, int g, int b, int a)
{
    SetForegroundMaxColor(sf::Color(r, g, b, a));
}

void Bar::SetForegroundMaxColor(const sf::Color& color)
{
    m_maxColor = color;
}

void Bar::SetForegroundMinColor(int r, int g, int b, int a)
{
    SetForegroundMinColor(sf::Color(r, g, b, a));
}

void Bar::SetForegroundMinColor(const sf::Color& color)
{
    m_minColor = color;
}

void Bar::SetOutlineThickness(float t)
{
    m_background.setOutlineThickness(t);
}

void Bar::SetMaxValue(float maxVal)
{
    m_maxValue = maxVal;
}

void Bar::SetCurrentValue(float val)
{
    m_currentValue = val;
}

void Bar::IncrementCurrentValue(float val)
{
    m_currentValue += val;
}

void Bar::SetVertical(bool vertical)
{
    m_isVertical = true;
}

void Bar::UseMinMaxColors(bool status)
{
    m_useMinMaxColor = true;
}

const sf::Vector2f& Bar::GetPosition() const
{
    return m_background.getPosition();
}

const sf::Vector2f& Bar::GetSize() const
{
    return m_background.getSize();
}

const sf::Vector2f& Bar::GetOrigin() const
{
    return m_background.getOrigin();
}

const sf::Color& Bar::GetForegroundColor() const
{
    return m_foreground.getFillColor();
}

const sf::Color& Bar::GetBackgroundColor() const
{
    return m_background.getFillColor();
}

const sf::Color& Bar::GetForegroundMaxColor() const
{
    return m_maxColor;
}

const sf::Color& Bar::GetForegroundMinColor() const
{
    return m_minColor;
}

float Bar::GetOutlineThickness() const
{
    return m_background.getOutlineThickness();
}

float Bar::GetMaxValue() const
{
    return m_maxValue;
}

float Bar::GetCurrentValue() const
{
    return m_currentValue;
}

bool Bar::IsVertical() const
{
    return m_isVertical;
}

void Bar::Update(float dt)
{
    float prc = m_currentValue / m_maxValue;

    sf::Vector2f backgroundSize = GetSize();
    sf::Vector2f foregroundSize = backgroundSize;
    if (m_isVertical)
        foregroundSize.y *= prc;
    else
        foregroundSize.x *= prc;

    m_foreground.setSize(foregroundSize);

    if (m_useMinMaxColor)
    {
        m_foreground.setFillColor(Global::LerpColor(m_minColor, m_maxColor, prc));
    }
}

void Bar::Draw(sf::RenderWindow* wnd)
{
    wnd->draw(m_background);
    wnd->draw(m_foreground);
}
