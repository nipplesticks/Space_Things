#ifndef _TEAM_HANDLER_H_
#define _TEAM_HANDLER_H_

#include <string>
#include "../GameObjects/Planet.h"
#include "../GameObjects/Unit.h"
#include <unordered_map>
#include "../../Utility/Timer.h"
#include "../../Utility/vector.h"
#include "../GUI/Button.h"

typedef long long unsigned int U64;

class TeamHandler
{
public:
    TeamHandler(size_t startUnits = 100);
    ~TeamHandler() {}

    void SetNameAndTeamColor(const std::string& name, const sf::Color& color);
    const std::string GetTeamName() const;

    void SetPlanetPointers(const Container::Vector::Vector<Planet*>& planets);
    void SetStartPlanet(size_t planetIndex);
    void CreateUnits(size_t count, Planet* atPlanet);
    void Select(const sf::Vector2f& start, const sf::Vector2f& end);
    void SetDestination(const sf::Vector2f& destination, Planet* planet = nullptr);

    void Update(float dt);

    void Draw(sf::RenderWindow* wnd);
    void DrawInfo(sf::RenderWindow* wnd);

private:
    void _checkOwnedPlanets();
    void _updateInfo();
    void _updateUnits(float dt);
    void _createUnits();
    void _removeDeadUnits();
private:
    float m_timer;
    std::string m_teamName;
    sf::Color m_teamColor;
    std::unordered_map<U64, Unit> m_units;
    Container::Vector::Vector<U64> m_selectedUnits;
    Container::Vector::Vector<Planet*> m_pPlanets;
    Container::Vector::Vector<Planet*> m_pOwnedPlanets;
    

    Button<void()> m_unitCountText;
    Button<void()> m_selectedUnitsCountText;
    bool m_quadTreeCleared;
};


#endif