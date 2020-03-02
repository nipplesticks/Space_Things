#ifndef _UNIT_H_
#define _UNIT_H_
#include <SFML/Graphics.hpp>
#include <string>
class Planet;

class Unit
{
public:
    static float DEFAULT_UNIT_RADIUS;
    static float DEFAULT_UNIT_SPEED;
    static int DEFUALT_MIN_ORBIT_DISTANCE;
    static int DEFUALT_MAX_ORBIT_DISTANCE;
    static int DEFAULT_UNIT_STRENGTH;
public:
    Unit();
    ~Unit(){}

    void SetPosition(float x, float y);
    void SetPosition(const sf::Vector2f& position);

    void Move(float x, float y);
    void Move(const sf::Vector2f& offset);

    void SetRadius(float r);

    void SetColor(int r, int g, int b, int a = 255);
    void SetColor(const sf::Color& color);

    void SetTeam(const std::string& team);

    void SetStrength(int strength);

    void SetDestination(float x, float y);
    void SetDestination(const sf::Vector2f& destination);
    void SetDestination(Planet* planet);

    void SetInOrbit(bool inOrbit);

    void SetSpeed(float speed);

    const sf::Vector2f& GetPosition() const;

    float GetRadius() const;

    const sf::Color& GetColor() const;

    const std::string GetTeam() const;

    int GetStrength() const;

    const sf::Vector2f& GetDestination() const;

    bool GetInOrbit() const;

    float GetSpeed() const;

    void Update(float dt);

    void Draw(sf::RenderWindow* wnd);

private:
    sf::Vector2f _calcOrbitPosition() const;

private:
    std::string m_team;
    sf::CircleShape m_shape;
    sf::Vector2f m_destination;
    Planet* m_target;
    bool m_inOrbit;
    int m_strength;
    int m_orbitDistance;
    bool m_isFollowingTarget;
    float m_speed;
};



#endif // !_UNIT_H_

