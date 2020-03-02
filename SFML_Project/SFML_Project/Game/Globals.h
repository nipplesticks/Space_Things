#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <SFML/Graphics.hpp>

namespace Global
{
    extern sf::Font g_font;
    extern sf::Vector2f g_mousePos;
    sf::Color LerpColor(const sf::Color& _a, const sf::Color& _b, float t);
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

#endif