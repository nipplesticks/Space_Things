#include <SFML/Graphics.hpp>
#include "../../Utility/vector.h"

class Console
{
public:
    Console();
    ~Console();

    void SetPosition(float x, float y);
    void SetPosition(const sf::Vector2f& position);

    void Move(float x, float y);
    void Move(const sf::Vector2f& offset);

    void SetSize(float x, float y);
    void SetSize(const sf::Vector2f& size);

    void SetTextColor(const sf::Color& color);
    void SetBackgroundColor(const sf::Color& color);
    void SetBorderColor(const sf::Color& color);

    void SetCharacterSize(unsigned int size);

    void Clear();
    void PushString(const std::string& str);

    const sf::Vector2f& GetPosition() const;
    
    void Update(float dt);

    void Draw(sf::RenderWindow* wnd);
private:
    void _adaptToCamera();

private:
    sf::RectangleShape m_background;
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    sf::Color m_textCol;
    Container::Vector::Vector<sf::Text> m_texts;
    unsigned int m_textSize;
};
