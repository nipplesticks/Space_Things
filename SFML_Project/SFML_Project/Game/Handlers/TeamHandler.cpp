#include "TeamHandler.h"
#include "../Structures/Globals.h"

using namespace Container::Vector;

TeamHandler::TeamHandler(size_t startUnits)
{
    m_selectedUnits = Vector<U64>(5000, 0, 1000);
    for (size_t i = 0; i < startUnits; i++)
    {
        Unit u;
        m_units.insert(std::make_pair(u.GetId(), u));
    }
    m_teamName = "";
    m_timer = 0.f;
    m_selectedUnitsCountText.setFont(Global::g_font);
    m_selectedUnitsCountText.setCharacterSize(16);
    m_selectedUnitsCountText.setFillColor(sf::Color::White);
    m_unitCountText = m_selectedUnitsCountText;
    m_quadTreeCleared = false;
}

void TeamHandler::SetNameAndTeamColor(const std::string& name, const sf::Color& color)
{
    m_teamName = name;
    m_teamColor = color;

    for (auto& u : m_units)
    {
        u.second.SetTeam(m_teamName);
        u.second.SetColor(color);
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

    for (auto& u : m_units)
    {
        sf::Vector2f dir((float)(rand() + 1), (float)(rand() + 1));
        float l = sqrt(dir.x * dir.x + dir.y * dir.y);
        dir = dir * (1.0f / l);
        u.second.SetPosition(base->GetPosition() + dir * 2.0f);
        u.second.SetDestination(base);
        u.second.SetInOrbit(true);
    }
}

void TeamHandler::CreateUnits(size_t count, Planet* atPlanet)
{
    for (size_t i = 0; i < count; i++)
    {
        sf::Vector2f dir((float)(rand() + 1), (float)(rand() + 1));
        float l = sqrt(dir.x * dir.x + dir.y * dir.y);
        dir = dir * (1.0f / l);

        Unit u;
        u.SetTeam(m_teamName);
        u.SetColor(m_teamColor);
        u.SetPosition(atPlanet->GetPosition() + dir * 2.0f);
        u.SetDestination(atPlanet);
        u.SetInOrbit(true);

        Unit u2;
        u2.SetTeam(m_teamName);
        u2.SetColor(m_teamColor);
        u2.SetPosition(atPlanet->GetPosition() + dir * -2.0f);
        u2.SetDestination(atPlanet);
        u2.SetInOrbit(true);

        m_units.insert(std::make_pair(u.GetId(), u));
        m_units.insert(std::make_pair(u2.GetId(), u2));
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
            m_selectedUnits.PushBack(units[i]->GetId());
    }
}

void TeamHandler::SetDestination(const sf::Vector2f& destination, Planet* planet)
{
    size_t s = m_selectedUnits.Size();

    if (planet)
    {
        for (size_t i = 0; i < s; i++)
        {
            m_units[m_selectedUnits[i]].SetDestination(planet);
            if (planet->GetCurrentLevel() == planet->GetMaxLevel())
                m_units[m_selectedUnits[i]].SetInOrbit(true);
        }
    }
    else
        for (size_t i = 0; i < s; i++)
            m_units[m_selectedUnits[i]].SetDestination(destination);
}

void TeamHandler::Update(float dt)
{
    m_timer += dt;
    _removeDeadUnits();
    _updateInfo();
    _checkOwnedPlanets();
    _createUnits();
    _updateUnits(dt);
    m_quadTreeCleared = true;
}

void TeamHandler::Draw(sf::RenderWindow* wnd)
{
    if (m_quadTreeCleared)
        for (auto& u : m_units)
            u.second.DrawAndUpdateQt(wnd);
    else
        for (auto& u : m_units)
            u.second.Draw(wnd);
    m_quadTreeCleared = false;
}

void TeamHandler::DrawInfo(sf::RenderWindow* wnd)
{
    wnd->draw(m_unitCountText);
    wnd->draw(m_selectedUnitsCountText);
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

void TeamHandler::_updateInfo()
{
    m_unitCountText.setString("Total Units: " + std::to_string(m_units.size()));
    m_selectedUnitsCountText.setString("Selection: " + std::to_string(m_selectedUnits.Size()));
    auto fr = m_unitCountText.getGlobalBounds();
    m_unitCountText.setOrigin(sf::Vector2f(fr.width + 2, 0.0f));
    m_unitCountText.setPosition(Global::g_windowSize.x, 0.0f);
    fr = m_selectedUnitsCountText.getGlobalBounds();
    m_selectedUnitsCountText.setOrigin(sf::Vector2f(fr.width + 2, 0.0f));
    m_selectedUnitsCountText.setPosition(Global::g_windowSize.x, m_unitCountText.getCharacterSize() + 2);
}

void TeamHandler::_updateUnits(float dt)
{
    for (auto& u : m_units)
    {
        u.second.Update(dt);
    }
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
    if (!m_selectedUnits.Empty())
    {
        size_t size = m_selectedUnits.Size();
        for (int i = (int)size - 1; i >= 0; i--)
        {
            if (m_units[m_selectedUnits[i]].IsDead())
                m_selectedUnits.Erase(i);
        }
    }

    Vector<U64> deadIds(m_units.size());

    for (auto& u : m_units)
    {
        if (u.second.IsDead())
        {
            deadIds.PushBack(u.first);
        }
    }

    size_t size = deadIds.Size();

    for (size_t i = 0; i < size; i++)
        m_units.erase(deadIds[i]);
}
