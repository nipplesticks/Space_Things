#ifndef _BAR_H_
#define _BAR_H_

#include <SFML/Graphics.hpp>

class Bar
{
public:
    Bar();
    ~Bar(){}

    void SetPosition(float x, float y);
    void SetPosition(const sf::Vector2f& position);

    void SetSize(float x, float y);
    void SetSize(const sf::Vector2f& size);

    void SetOrigin(float x, float y);
    void SetOrigin(const sf::Vector2f& origin);

    void SetForegroundColor(int r, int g, int b, int a = 255);
    void SetForegroundColor(const sf::Color & color);
    void SetBackgroundColor(int r, int g, int b, int a = 255);
    void SetBackgroundColor(const sf::Color& color);

    void SetForegroundMaxColor(int r, int g, int b, int a = 255);
    void SetForegroundMaxColor(const sf::Color& color);
    void SetForegroundMinColor(int r, int g, int b, int a = 255);
    void SetForegroundMinColor(const sf::Color& color);

    void SetOutlineThickness(float t);

    void SetMaxValue(float maxVal);
    void SetCurrentValue(float val);
    void IncrementCurrentValue(float val);

    void SetVertical(bool vertical);

    void UseMinMaxColors(bool status);

    const sf::Vector2f& GetPosition() const;
    const sf::Vector2f& GetSize() const;
    const sf::Vector2f& GetOrigin() const;
    const sf::Color& GetForegroundColor() const;
    const sf::Color& GetBackgroundColor() const;
    const sf::Color& GetForegroundMaxColor() const;
    const sf::Color& GetForegroundMinColor() const;

    float GetOutlineThickness() const;

    float GetMaxValue() const;
    float GetCurrentValue() const;

    bool IsVertical() const;

    void Update(float dt);

    void Draw(sf::RenderWindow* wnd);

private:
    sf::RectangleShape m_background;
    sf::RectangleShape m_foreground;
    
    sf::Color m_minColor;
    sf::Color m_maxColor;

    bool m_useMinMaxColor;

    bool m_isVertical;

    float m_maxValue;
    float m_currentValue;
};


#endif