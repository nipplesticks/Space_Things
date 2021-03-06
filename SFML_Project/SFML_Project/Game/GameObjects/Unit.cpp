#include "Unit.h"
#include "Planet.h"
#include <DirectXMath.h>
#include "../Structures/Globals.h"
#include "../../Utility/vector.h"

using namespace Container::Vector;

float Unit::DEFAULT_UNIT_RADIUS = 3.0f;
float Unit::DEFAULT_UNIT_SPEED = 64.0f;
float Unit::DEFAULT_DETECT_RANGE = 64.0f;
int Unit::DEFUALT_MIN_ORBIT_DISTANCE = 16;
int Unit::DEFUALT_MAX_ORBIT_DISTANCE = 64;
int Unit::DEFAULT_UNIT_STRENGTH = 1;
U64 Unit::STATIC_ID = 0;

Unit::Unit()
{
    m_inOrbit = false;
    m_team = "";
    m_shape.setRadius(DEFAULT_UNIT_RADIUS);
    m_shape.setOrigin(sf::Vector2f(DEFAULT_UNIT_RADIUS, DEFAULT_UNIT_RADIUS));
    m_target = nullptr;
    m_destination = m_shape.getPosition();
    m_strength = DEFAULT_UNIT_STRENGTH;
    m_speed = DEFAULT_UNIT_SPEED;
    m_isFollowingTarget = false;
    m_id = STATIC_ID++;
}

void Unit::SetPosition(float x, float y)
{
    SetPosition({ x, y });
}

void Unit::SetPosition(const sf::Vector2f& position)
{
    m_shape.setPosition(position);
}

void Unit::Move(float x, float y)
{
    Move({ x, y });
}

void Unit::Move(const sf::Vector2f& offset)
{
    SetPosition(GetPosition() + offset);
}

void Unit::SetRadius(float r)
{
    m_shape.setRadius(r);
    m_shape.setOrigin(sf::Vector2f(r, r));
}

void Unit::SetColor(int r, int g, int b, int a)
{
    SetColor(sf::Color(r, g, b, a));
}

void Unit::SetColor(const sf::Color& color)
{
    m_shape.setFillColor(color);
}

void Unit::SetTeam(const std::string& team)
{
    m_team = team;
}

void Unit::SetStrength(int strength)
{
    m_strength = strength;
}

void Unit::SetDestination(float x, float y)
{
    SetDestination({ x, y });
}

void Unit::SetDestination(const sf::Vector2f& destination)
{
    m_destination = destination;
    m_isFollowingTarget = false;
    m_inOrbit = false;
    m_target = nullptr;
    m_isIdle = false;
}

void Unit::SetDestination(Planet* planet)
{
    m_isIdle = false;
    m_inOrbit = false;
    m_target = planet;
    m_destination = planet->GetPosition();
    m_isFollowingTarget = false;
    m_orbitDistance = rand() % 
        (DEFUALT_MAX_ORBIT_DISTANCE - DEFUALT_MIN_ORBIT_DISTANCE + 1)
        + DEFUALT_MIN_ORBIT_DISTANCE;
}

void Unit::SetInOrbit(bool inOrbit)
{
    m_inOrbit = inOrbit && m_target;
}

void Unit::SetSpeed(float speed)
{
    m_speed = speed;
}

const sf::Vector2f& Unit::GetPosition() const
{
    return m_shape.getPosition();
}

float Unit::GetRadius() const
{
    return m_shape.getRadius();
}

const sf::Color& Unit::GetColor() const
{
    return m_shape.getFillColor();
}

const std::string Unit::GetTeam() const
{
    return m_team;
}

int Unit::GetStrength() const
{
    return m_strength;
}

const sf::Vector2f& Unit::GetDestination() const
{
    return m_destination;
}

bool Unit::GetInOrbit() const
{
    return m_inOrbit && m_target;
}

U64 Unit::GetId() const
{
    return m_id;
}

float Unit::GetSpeed() const
{
    return m_speed;
}

bool Unit::IsDead() const
{
    return m_strength <= 0;
}

void Unit::PlaceInQT()
{
    Global::g_unitQuadtree.Place(this, GetPosition());
}

void Unit::Update(float dt)
{
    Unit* enemy = nullptr;
    float search = Unit::DEFAULT_DETECT_RANGE;
    if (m_isFollowingTarget)
        search *= 2.f;

    Vector<Unit*> units = Global::g_unitQuadtree.GetObjectsFromCircle(GetPosition(), search);
    float emyDist = FLT_MAX;
    sf::Vector2f emyDir;
    for (size_t i = 0; i < units.Size(); i++)
    {
        if (!units[i]->IsDead() && units[i]->GetTeam() != m_team)
        {
            emyDir = units[i]->GetPosition() - GetPosition();
            float lsqr = emyDir.x * emyDir.x + emyDir.y * emyDir.y;
            if (lsqr < emyDist)
            {
                emyDist = lsqr;
                enemy = units[i];
            }
        }
    }


    if (enemy)
    {
        float l = sqrt(emyDist);

        if (l < enemy->GetRadius() + GetRadius())
        {
            enemy->SetStrength(enemy->GetStrength() - m_strength);
        }
        else
        {
            sf::Vector2f dir = emyDir * (1.0f / l);
            Move(dir * m_speed * dt);
        }
    }
    else if (!m_isIdle)
    {
        sf::Vector2f direction = m_destination - GetPosition();
        float l = sqrt(direction.x * direction.x + direction.y * direction.y);

        if (m_target)
        {
            float lBefore = l;
            sf::Vector2f directionBefore = direction;
            m_destination = m_target->GetPosition();
            direction = m_destination - GetPosition();
            l = sqrt(direction.x * direction.x + direction.y * direction.y);

            if (m_inOrbit && (l < m_orbitDistance + m_target->GetRadius() || m_isFollowingTarget))
            {
                sf::Vector2f _dir = directionBefore * (1.0f / lBefore);
                SetPosition(m_destination - _dir * ((float)m_orbitDistance + m_target->GetRadius()));
                m_isFollowingTarget = true;
                m_destination = _calcOrbitPosition();
                direction = m_destination - GetPosition();
                l = sqrt(direction.x * direction.x + direction.y * direction.y);
            }
        }

        if (l > GetRadius())
        {
            direction = direction * (1.0f / l);

            Move(direction * m_speed * dt);

            if (m_isFollowingTarget)
                m_destination = m_target->GetPosition();
        }
        else if (!m_inOrbit && m_target)
        {
            if (m_target->GetCurrentLevel() != m_target->GetMaxLevel() || m_target->GetTeam() != m_team)
            {
                if (m_target->GetTeam() == m_team)
                    m_target->IncrementToNextLevel(m_strength);
                else if (m_target->GetTeam() == "Neutral")
                {
                    m_target->SetTeam(m_team);
                    m_target->SetColor(m_shape.getFillColor());
                    m_target->IncrementToNextLevel(m_strength);
                }
                else
                {
                    m_target->IncrementDestruction(m_strength);
                }
                m_strength = 0;
            }
            else
                m_inOrbit = true;
        }
        else if (m_target == nullptr)
            m_isIdle = true;
    }
    else
    {
        Vector<Unit*> nearbyUnits = Global::g_unitQuadtree.GetObjectsFromCircle(GetPosition(), GetRadius());
        if (nearbyUnits.Size() > 1)
        {
            sf::Vector2f idleDir(0, 0);
            for (size_t i = 0; i < nearbyUnits.Size(); i++)
            {
                if (nearbyUnits[i] == this)
                    continue;
                if (nearbyUnits[i]->GetTeam() == m_team)
                {
                    idleDir = idleDir + nearbyUnits[i]->GetPosition();
                }
            }
            idleDir = idleDir * (1.0f / (float)(nearbyUnits.Size() - 1));

            idleDir = GetPosition() - idleDir;
            float l = sqrt(idleDir.x * idleDir.x + idleDir.y * idleDir.y);

            idleDir = idleDir * (1.0f / l);

            Move(idleDir * (float)nearbyUnits.Size() * dt);
        }
    }
}

void Unit::Draw(sf::RenderWindow* wnd)
{
    wnd->draw(m_shape);
}

void Unit::DrawAndUpdateQt(sf::RenderWindow* wnd)
{
    Draw(wnd);
    if (!IsDead())
        PlaceInQT();
}

sf::Vector2f Unit::_calcOrbitPosition() const
{
    sf::Vector2f newDestination;

    sf::Vector2f posNow = GetPosition();
    sf::Vector2f center = m_target->GetPosition();
    sf::Vector2f centerToPos = posNow - center;

    float l = sqrt(centerToPos.x * centerToPos.x + centerToPos.y * centerToPos.y);
    sf::Vector2f dir = centerToPos * (1.0f / l);

    using namespace DirectX;

    XMFLOAT3 _dir = { dir.x, 0.0f, dir.y };
    XMFLOAT3 _up =  { 0.0f, 1.0f, 0.0f };
    XMFLOAT3 _newDir;
    XMStoreFloat3(&_newDir, XMVector3Normalize(XMVector3Cross(
        XMLoadFloat3(&_dir), XMLoadFloat3(&_up))));
    
    sf::Vector2f newDir(_newDir.x, _newDir.z);

    dir = dir * ((float)m_orbitDistance + m_target->GetRadius());
    newDir = newDir * GetRadius() * 2.0f;

    newDestination = center + dir + newDir;

    return newDestination;
}
