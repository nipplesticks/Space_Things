#include "Console.h"
#include "Camera.h"
#include "Globals.h"

Console::Console()
{
    m_size = sf::Vector2f(Global::g_windowSize.x * 0.15, Global::g_windowSize.y);
    m_position = sf::Vector2f(0, 0);
    sf::Color c = sf::Color::White;
    c.a = 128;
    SetBackgroundColor(c);
    SetCharacterSize(11);
    SetTextColor(sf::Color::Black);
    SetBorderColor(sf::Color::Blue);
}

Console::~Console()
{
}

void Console::SetPosition(float x, float y)
{
    SetPosition({ x, y });
}

void Console::SetPosition(const sf::Vector2f& position)
{
    m_position = position;
}

void Console::Move(float x, float y)
{
    Move({ x, y });
}

void Console::Move(const sf::Vector2f& offset)
{
    SetPosition(GetPosition() + offset);
}

void Console::SetSize(float x, float y)
{
    SetSize({ x, y });
}

void Console::SetSize(const sf::Vector2f& size)
{
    m_size = size;
}

void Console::SetTextColor(const sf::Color& color)
{
    m_textCol = color;
}

void Console::SetBackgroundColor(const sf::Color& color)
{
    m_background.setFillColor(color);
}

void Console::SetBorderColor(const sf::Color& color)
{
    m_background.setOutlineColor(color);
}

void Console::SetCharacterSize(unsigned int size)
{
    m_textSize = size;
}

void Console::Clear()
{
    m_texts.Clear();
}

void Console::PushString(const std::string& str)
{
    m_texts.PushFront(sf::Text());
    m_texts.Front().setString(str);
    m_texts.Front().setFillColor(m_textCol);
    m_texts.Front().setFont(Global::g_font);
    m_texts.Front().setCharacterSize(m_textSize);

    float s = 0.0f;
    for (size_t i = 0; i < m_texts.Size(); i++)
    {
        s += m_texts[i].getGlobalBounds().height;
    }

    if (s >= m_size.y)
    {
        m_texts.PopBack();
    }
}

const sf::Vector2f& Console::GetPosition() const
{
    return m_position;
}

void Console::Update(float dt)
{
    Camera* c = Camera::GetActiveCamera();

    sf::Vector2f cPos = c->GetPosition();
    float cZoom = c->GetZoom();
    sf::Vector2f cSize = c->GetSize();
    float cRot = c->GetRotation();

    m_background.setSize(m_size * cZoom);

    m_background.setRotation(cRot);

    using namespace DirectX;

    sf::Vector2f unRotatedPosition = (-cSize * 0.5f + m_position) * cZoom;
    XMFLOAT3 urp = { unRotatedPosition.x, 0.0f, unRotatedPosition.y };
    XMFLOAT3 rp;
    XMStoreFloat3(&rp, XMVector3Transform(XMLoadFloat3(&urp), XMMatrixRotationRollPitchYaw(0.0f, Global::DegreeseToRadians(-cRot), 0.0f)));


    sf::Vector2f rotatedPosition(rp.x, rp.z);
    rotatedPosition = rotatedPosition + cPos;

    m_background.setPosition(rotatedPosition);
    for (size_t i = 0; i < m_texts.Size(); i++)
    {
        m_texts[i].setScale(sf::Vector2f(cZoom, cZoom));
        m_texts[i].setRotation(cRot);
        m_texts[i].setPosition(rotatedPosition);
        m_texts[i].setOrigin(sf::Vector2f(0.0f, -((int)i) * ((int)m_textSize + 1)));
    }
}

void Console::Draw(sf::RenderWindow* wnd)
{
    wnd->draw(m_background);
    for (size_t i = 0; i < m_texts.Size(); i++)
        wnd->draw(m_texts[i]);
}
