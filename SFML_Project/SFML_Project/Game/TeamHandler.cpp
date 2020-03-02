#include "TeamHandler.h"

#include "Globals.h"

using namespace Container::Vector;

TeamHandler::TeamHandler(size_t startUnits)
{
    m_units = Vector<Unit>(5000, startUnits, 1000);
    m_selectedUnits = Vector<Unit*>(5000, 0, 1000);
    m_teamName = "";
    m_timer = 0.f;
}

void TeamHandler::SetNameAndTeamColor(const std::string& name, const sf::Color& color)
{
    m_teamName = name;

    for (size_t i = 0; i < m_units.Size(); i++)
    {
        m_units[i].SetTeam(m_teamName);
        m_units[i].SetColor(color);
    }
}

const std::string TeamHandler::GetTeamName() const
{
    return m_teamName;
}

void TeamHandler::SetPlanetPointers(const Container::Vector::Vector<Planet*>& planets)
{
    m_pPlanets = planets;
    m_pOwnedPlanets = Vector<Planet*>(planets.Size(), 0);
}

void TeamHandler::SetStartPlanet(size_t planetIndex)
{
    Planet* base = m_pPlanets[planetIndex];

    for (size_t i = 0; i < m_units.Size(); i++)
    {
        m_units[i].SetPosition(base->GetPosition());
        m_units[i].SetDestination(base);
    }
}

void TeamHandler::CreateUnits(size_t count, Planet* atPlanet)
{
    for (size_t i = 0; i < count; i+=2)
    {
        sf::Vector2f dir(rand() + 1, rand() + 1);
        float l = sqrt(dir.x * dir.x + dir.y * dir.y);
        dir = dir * (1.0f / l);
        m_units.PushBack(Unit());
        m_units.Back().
            /*
            m_units[i].SetTeam(m_teamName);
        m_units[i].SetColor(color);
        m_units[i].SetPosition(base->GetPosition());
        m_units[i].SetDestination(base);*/
    }
}

void TeamHandler::Select(const sf::Vector2f& start, const sf::Vector2f& end)
{
    m_selectedUnits.Clear();
    Vector<Unit*> units = Global::g_unitQuadtree.GetObjectsFromQuad(start, end);
    size_t s = units.Size();
    for (size_t i = 0; i < s; i++)
    {
        if (units[i]->GetTeam() == m_teamName)
            m_selectedUnits.PushBack(units);
    }
}

void TeamHandler::SetDestination(const sf::Vector2f& destination, Planet* planet)
{
    size_t s = m_selectedUnits.Size();

    if (planet)
        for (size_t i = 0; i < s; i++)
            m_selectedUnits[i]->SetDestination(planet);
    else
        for (size_t i = 0; i < s; i++)
            m_selectedUnits[i]->SetDestination(destination);
}

void TeamHandler::Update(float dt)
{
    m_timer += dt;
    _checkOwnedPlanets();
    _createUnits();
    _updateUnits(dt);
}

void TeamHandler::Draw(sf::RenderWindow* wnd)
{
    size_t unitSize = m_units.Size();
    for (size_t i = 0; i < unitSize; i++)
        m_units[i].DrawAndUpdateQt(wnd);
}

void TeamHandler::_checkOwnedPlanets()
{
    size_t numberOfPlanets = m_pPlanets.Size();
    m_pOwnedPlanets.Clear();
    for (size_t i = 0; i < numberOfPlanets; i++)
        if (m_pPlanets[i]->GetTeam() == m_teamName)
            m_pOwnedPlanets.PushBack(m_pPlanets[i]);
}

void TeamHandler::_updateUnits(float dt)
{
    size_t unitSize = m_units.Size();
    for (size_t i = 0; i < unitSize; i++)
        m_units[i].Update(dt);
}

void TeamHandler::_createUnits()
{
    if (m_timer >= 2.0f)
    {
        m_timer -= 2.0f;

        size_t numberOfPlanets = m_pOwnedPlanets.Size();
        for (size_t i = 0; i < numberOfPlanets; i++)
        {
            CreateUnits(m_pOwnedPlanets[i]->GetCurrentLevel() * 2, m_pOwnedPlanets[i]);
        }
    }
}
