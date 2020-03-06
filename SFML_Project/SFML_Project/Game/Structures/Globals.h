#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <SFML/Graphics.hpp>
#include "QuadTree.h"

class Unit;

namespace Global
{
    static const float g_PI = 3.14159265359f;
    extern sf::Font g_font;
    extern sf::Vector2f g_mousePos;
    extern QuadTree<Unit*> g_unitQuadtree;
    extern sf::Vector2f g_windowSize;

    sf::Color LerpColor(const sf::Color& _a, const sf::Color& _b, float t);
    float RadiansToDegreese(float rad);
    float DegreeseToRadians(float deg);

}

inline sf::Color Global::LerpColor(const sf::Color& _a, const sf::Color& _b, float t)
{
    sf::Color ret;

    double r = (double)_a.r + t * ((double)_b.r - (double)_a.r);
    double g = (double)_a.g + t * ((double)_b.g - (double)_a.g);
    double b = (double)_a.b + t * ((double)_b.b - (double)_a.b);

    ret.r = (unsigned int)(r + 0.5);
    ret.g = (unsigned int)(g + 0.5);
    ret.b = (unsigned int)(b + 0.5);

    return ret;
}
inline float Global::RadiansToDegreese(float rad)
{
    return rad * (180.0f / Global::g_PI);
}
inline float Global::DegreeseToRadians(float deg)
{
    return deg * (Global::g_PI / 180.0f);
}

#endif