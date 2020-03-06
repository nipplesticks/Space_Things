#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <SFML/Graphics.hpp>
#include "QuadTree.h"
#include <DirectXMath.h>
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
    sf::Vector2f Rotate(const sf::Vector2f& v, float deg);

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
inline sf::Vector2f Global::Rotate(const sf::Vector2f& v, float deg)
{
    using namespace DirectX;
    XMFLOAT3 urp = { v.x, 0.0f, v.y };
    XMFLOAT3 rp;
    XMStoreFloat3(&rp, XMVector3Transform(XMLoadFloat3(&urp), XMMatrixRotationRollPitchYaw(0.0f, Global::DegreeseToRadians(deg), 0.0f)));
    return sf::Vector2f(rp.x, rp.z);
}

#endif