#ifndef _TEAM_HANDLER_H_
#define _TEAM_HANDLER_H_

#include "Planet.h"
#include "Unit.h"
#include "../Utility/Timer.h"
#include "../Utility/vector.h"

class TeamHandler
{
public:
    TeamHandler(size_t startUnits = 100);
    ~TeamHandler() {}

    void SetPlanetPointers(const Container::Vector::Vector<Planet*>& planets);
    void SetStartPlanet(size_t planetIndex);
    void CreateUnits(size_t count, Planet* atPlanet);

private:
    Container::Vector::Vector<Unit> m_units;
    Container::Vector::Vector<Planet*> m_pPlanets;
};


#endif