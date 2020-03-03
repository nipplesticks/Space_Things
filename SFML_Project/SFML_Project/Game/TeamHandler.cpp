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
    m_teamColor = color;

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
        sf::Vector2f dir((float)(rand() + 1), (float)(rand() + 1));
        float l = sqrt(dir.x * dir.x + dir.y * dir.y);
        dir = dir * (1.0f / l);
        m_units[i].SetPosition(base->GetPosition() + dir * 2.0f);
        m_units[i].SetDestination(base);
        m_units[i].SetInOrbit(true);
    }
}

void TeamHandler::CreateUnits(size_t count, Planet* atPlanet)
{
    for (size_t i = 0; i < count; i++)
    {
        sf::Vector2f dir((float)(rand() + 1), (float)(rand() + 1));
        float l = sqrt(dir.x * dir.x + dir.y * dir.y);
        dir = dir * (1.0f / l);
        m_units.PushBack(Unit());
        m_units.Back().SetTeam(m_teamName);
        m_units.Back().SetColor(m_teamColor);
        m_units.Back().SetPosition(atPlanet->GetPosition() + dir * 2.0f);
        m_units.Back().SetDestination(atPlanet);
        m_units.Back().SetInOrbit(true);

        m_units.PushBack(Unit());
        m_units.Back().SetTeam(m_teamName);
        m_units.Back().SetColor(m_teamColor);
        m_units.Back().SetPosition(atPlanet->GetPosition() + dir * -2.0f);
        m_units.Back().SetDestination(atPlanet);
        m_units.Back().SetInOrbit(true);
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
    {
        for (size_t i = 0; i < s; i++)
        {
            m_selectedUnits[i]->SetDestination(planet);
            if (planet->GetCurrentLevel() == planet->GetMaxLevel())
                m_selectedUnits[i]->SetInOrbit(true);
        }
    }
    else
        for (size_t i = 0; i < s; i++)
            m_selectedUnits[i]->SetDestination(destination);
}

void TeamHandler::Update(float dt)
{
    m_timer += dt;
    _removeDeadUnits();
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
    {
        if (m_pPlanets[i]->GetTeam() == m_teamName)
        {
            m_pOwnedPlanets.PushBack(m_pPlanets[i]);
        }
    }
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
            CreateUnits(m_pOwnedPlanets[i]->GetCurrentLevel(), m_pOwnedPlanets[i]);
        }
    }
}

void TeamHandler::_removeDeadUnits()
{
    size_t size = m_selectedUnits.Size();
    for (int i = 0; i < (int)size; i++)
    {
        if (m_selectedUnits[i]->IsDead())
        {
            m_selectedUnits.Erase(i);
            i--;
            size--;
        }
    }
    size = m_units.Size();
    for (int i = 0; i < (int)size; i++)
    {
        if (m_units[i].IsDead())
        {
            m_units.Erase(i);
            i--;
            size--;
        }
    }
}
