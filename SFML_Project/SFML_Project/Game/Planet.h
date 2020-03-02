#ifndef _PLANET_H_
#define _PLANET_H_

#include <SFML/Graphics.hpp>
#include "../Utility/Timer.h"
#include "../Utility/vector.h"
#include <string>
#include "Bar.h"

class Planet
{
public:
    static int MAX_INCREMENT_LEVEL;
    static sf::Color NEUTRAL_COLOR;
    static float START_RADIUS;
    static float INDICATOR_RADIUS;
    static unsigned int FONT_SIZE;

public:
    Planet();
    ~Planet();

    void SetPosition(float x, float y);
    void SetPosition(const sf::Vector2f& position);

    void Move(float x, float y);
    void Move(const sf::Vector2f& offset);

    void SetRadius(float r);

    void SetColor(int r, int g, int b, int a = 255);
    void SetColor(const sf::Color& color);

    void SetTeam(const std::string & team);

    void SetMaxLevel(int maxLevel);
    void IncrementToNextLevel(int inc = 1);
    void SetCurrentLevel(int level);
    void IncrementDestruction(int inc = 1);

    bool Contains(float x, float y);
    bool Contains(const sf::Vector2f& point) const;

    const sf::Vector2f& GetPosition() const;

    float GetRadius() const;

    const sf::Color& GetColor() const;

    const std::string GetTeam() const;

    int GetMaxLevel() const;
    int GetNextLevelIncrement() const;
    int GetCurrentLevel() const;
    int GetDestructionIncrement() const;

    void Update(float dt);

    void Draw(sf::RenderWindow* wnd);
    void DrawInfo(sf::RenderWindow* wnd);
private:
    void _updateFromLevel();
    void _updateText();
    void _updateLevelIndicator();
    void _updateIndicators();

private:
    sf::CircleShape m_shape;
    
    sf::Color m_col;

    Bar m_levelUpIndicator;
    Bar m_destructionIndicator;

    Container::Vector::Vector<sf::CircleShape> m_levelIndicator;
    std::string m_team;
    sf::Text m_text;
    int m_levelUpCounter;
    int m_destructionCounter;
    int m_currentLevel;
    int m_maxLevel;
};


#endif
