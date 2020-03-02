#ifndef _QUAD_TREE_H_
#define _QUAD_TREE_H_

#include <SFML/Graphics.hpp>
#include "../Utility/vector.h"

template <class T>
class QuadTree
{
public:
    static size_t MAX_NUMBER_OF_ELEMENTS_BEFORE_BRANCHING;
private:
    struct Element
    {
        T Data;
        sf::Vector2f Position;
    };
    class Node
    {
    public:
        Node();
        void Init(const sf::Vector2f& minPos, const sf::Vector2f& maxPos, size_t currDepth, size_t maxDepth);
        void Clear();
        void Release();
        void Erease(T object, sf::Vector2f& objectPos);

        void Place(T object, const sf::Vector2f& at);

        void GetObjectsFromQuad(const sf::Vector2f& startPos, const sf::Vector2f& endPos, Container::Vector::Vector<T>& returnVec);
        void GetObjectsFromCircle(const sf::Vector2f& center, float rad, Container::Vector::Vector<T>& returnVec);
        void GetObjectsFromDepthAndPosition(size_t depth, const sf::Vector2f& position, Container::Vector::Vector<T>& returnVec);

        bool _intersectsWithCircle(const sf::Vector2f& center, float rad);

    public:
        sf::Vector2f Min, Max;
        Container::Vector::Vector<Node> Children;
        bool HasChildren;
        size_t Depth;
        size_t MAX_DEPTH;
        Container::Vector::Vector<Element> Objects;
    };
public:
    QuadTree();
    
    void Init(const sf::Vector2f& startPos, const sf::Vector2f& endPos, size_t maxDepth);
    void Clear();
    void Release();

    void Place(T object, const sf::Vector2f & position);
    void Erease(T object, const sf::Vector2f& objectPos);

    const sf::Vector2f& GetStartPosition() const;
    const sf::Vector2f& GetEndPosition() const;
    sf::Vector2f GetSize() const;

    Container::Vector::Vector<T> GetObjectsFromQuad(const sf::Vector2f& startPos, const sf::Vector2f& endPos);
    Container::Vector::Vector<T> GetObjectsFromCircle(const sf::Vector2f& center, float rad);
    Container::Vector::Vector<T> GetObjectsFromDepthAndPosition(size_t depth, const sf::Vector2f& position);

private:
    Node m_root;
    size_t m_Depth;
};

template<class T>
size_t QuadTree<T>::MAX_NUMBER_OF_ELEMENTS_BEFORE_BRANCHING = 20;

template<class T>
inline QuadTree<T>::Node::Node()
{
    Min = sf::Vector2f(0, 0);
    Max = sf::Vector2f(0, 0);
    HasChildren = false;
    Depth = UINT_MAX;
}

template<class T>
inline void QuadTree<T>::Node::Init(const sf::Vector2f& minPos, const sf::Vector2f& maxPos, size_t currDepth, size_t maxDepth)
{
    Min = minPos;
    Max = maxPos;
    Depth = currDepth;
    MAX_DEPTH = maxDepth;
    Objects = Container::Vector::Vector<Element>(100, 0, 50);
}

template<class T>
inline void QuadTree<T>::Node::Clear()
{
    if (HasChildren)
    {
        for (size_t i = 0; i < 4; i++)
            Children[i].Clear();
    }
    Objects.Clear();
    HasChildren = false;
}

template<class T>
inline void QuadTree<T>::Node::Release()
{
    Min = sf::Vector2f(0, 0);
    Max = sf::Vector2f(0, 0);
    Depth = UINT_MAX;
    Objects.Clear(true);
    if (HasChildren)
    {
        for (size_t i = 0; i < 4; i++)
            Children[i].Release();
        HasChildren = false;
    }
    Children.Clear(true);
}

template<class T>
inline void QuadTree<T>::Node::Erease(T object, sf::Vector2f& objectPos)
{
    sf::FloatRect fr(Min, Max - Min);
    if (fr.contains(objectPos))
    {
        if (HasChildren)
        {
            for (size_t i = 0; i < 4; i++)
                Children[i].Erase(object, objectPos);
        }
        else
        {
            size_t index = UINT_MAX;
            size_t objectsSize = Objects.Size();
            for (size_t i = 0; i < objectsSize; i++)
            {
                if (Objects[i].Data == object)
                {
                    index = i;
                    break;
                }
            }

            if (index != UINT_MAX)
                Objects.Erase(index);

        }
    }
}

template<class T>
inline void QuadTree<T>::Node::Place(T object, const sf::Vector2f& at)
{
    sf::FloatRect fr(Min, Max - Min);

    if (fr.contains(at))
    {
        if (HasChildren)
        {
            for (size_t i = 0; i < 4; i++)
                Children[i].Place(object, at);
        }
        else
        {
            if (Objects.Size() < QuadTree<T>::MAX_NUMBER_OF_ELEMENTS_BEFORE_BRANCHING || Depth == MAX_DEPTH)
            {
                Objects.PushBack({ object, at });
            }
            else
            {
                HasChildren = true;
                Children = Container::Vector::Vector<Node>(4, 4);
                sf::Vector2f _minPos = Min;
                sf::Vector2f size = (Max - Min) * 0.5f;

                for (size_t y = 0; y < 2; y++)
                {
                    _minPos = Min;
                    _minPos.y += size.y * y;
                    for (size_t x = 0; x < 2; x++)
                    {
                        _minPos.x += size.x * x;
                        Children[x + y * 2].Init(_minPos, _minPos + size, Depth + 1, MAX_DEPTH);
                        Children[x + y * 2].Place(object, at);
                    }
                }
                size_t objectsSize = Objects.Size();
                for (size_t i = 0; i < objectsSize; i++)
                {
                    for (size_t j = 0; j < 4; j++)
                        Children[j].Place(Objects[i].Data, Objects[i].Position);
                }
                Objects.Clear();
            }
        }
    }
}

template<class T>
inline void QuadTree<T>::Node::GetObjectsFromQuad(const sf::Vector2f& startPos, const sf::Vector2f& endPos, Container::Vector::Vector<T>& returnVec)
{
    sf::FloatRect fr(Min, Max - Min);
    sf::FloatRect fr2(startPos, endPos - startPos);
    if (fr.intersects(fr2))
    {
        if (HasChildren)
        {
            for (size_t i = 0; i < 4; i++)
            {
                Children[i].GetObjectsFromQuad(startPos, endPos, returnVec);
            }
        }
        else
        {
            size_t size = Objects.Size();
            for (size_t i = 0; i < size; i++)
            {
                if (fr2.contains(Objects[i].Position))
                    returnVec.PushBack(Objects[i].Data);
            }
        }
    }
}

template<class T>
inline void QuadTree<T>::Node::GetObjectsFromCircle(const sf::Vector2f& center, float rad, Container::Vector::Vector<T>& returnVec)
{
    if (_intersectsWithCircle(center, rad))
    {
        if (HasChildren)
        {
            for (size_t i = 0; i < 4; i++)
                Children[i].GetObjectsFromCircle(center, rad, returnVec);
        }
        else
        {
            size_t size = Objects.Size();
            for (size_t i = 0; i < size; i++)
            {
                sf::Vector2f centerToObject = Objects[i].Position - center;
                float lsqr = centerToObject.x * centerToObject.x + centerToObject.y * centerToObject.y;
                if (lsqr < (rad * rad))
                    returnVec.PushBack(Objects[i].Data);
            }
        }
    }
}

template<class T>
inline void QuadTree<T>::Node::GetObjectsFromDepthAndPosition(size_t depth, const sf::Vector2f& position, Container::Vector::Vector<T>& returnVec)
{
    sf::FloatRect fr(Min, Max - Min);

    if (fr.contains(position) || Depth <= Depth)
    {
        if (HasChildren)
        {
            GetObjectsFromDepthAndPosition(depth, position, returnVec);
        }
        else
        {
            size_t size = Objects.Size();
            for (size_t i = 0; i < size; i++)
            {
                returnVec.PushBack(Objects[i].Data);
            }
        }
    }
}

template<class T>
inline bool QuadTree<T>::Node::_intersectsWithCircle(const sf::Vector2f& center, float rad)
{
    sf::Vector2f aabbSize = Max - Min;
    sf::Vector2f circlePos = center;
    sf::Vector2f aabbPos = Min;

    float DeltaX = circlePos.x - std::max(aabbPos.x, std::min(circlePos.x, aabbPos.x + aabbSize.x));
    float DeltaY = circlePos.y - std::max(aabbPos.y, std::min(circlePos.y, aabbPos.y + aabbSize.y));

    return (DeltaX * DeltaX + DeltaY * DeltaY) < (rad * rad);
}

template<class T>
inline QuadTree<T>::QuadTree()
{
}

template<class T>
inline void QuadTree<T>::Init(const sf::Vector2f& startPos, const sf::Vector2f& endPos, size_t depth)
{
    m_Depth = depth;
    m_root.Init(startPos, endPos, 0, depth);
}

template<class T>
inline void QuadTree<T>::Clear()
{
    m_root.Clear();
}

template<class T>
inline void QuadTree<T>::Release()
{
    m_root.Release();
}

template<class T>
inline void QuadTree<T>::Place(T object, const sf::Vector2f & position)
{
    m_root.Place(object, position);
}

template<class T>
inline void QuadTree<T>::Erease(T object, const sf::Vector2f& objectPos)
{
    m_root.Erease(object, objectPos);
}

template<class T>
inline const sf::Vector2f& QuadTree<T>::GetStartPosition() const
{
    return m_root.Min;
}

template<class T>
inline const sf::Vector2f& QuadTree<T>::GetEndPosition() const
{
    return m_root.Max;
}

template<class T>
inline sf::Vector2f QuadTree<T>::GetSize() const
{
    return m_root.Max - m_root.Min;
}

template<class T>
inline Container::Vector::Vector<T> QuadTree<T>::GetObjectsFromQuad(const sf::Vector2f& startPos, const sf::Vector2f& endPos)
{
    Container::Vector::Vector<T> returnVec(50, 0, 25);
    m_root.GetObjectsFromQuad(startPos, endPos, returnVec);
    return returnVec;
}

template<class T>
inline Container::Vector::Vector<T> QuadTree<T>::GetObjectsFromCircle(const sf::Vector2f& center, float rad)
{
    Container::Vector::Vector<T> returnVec(50, 0, 25);
    m_root.GetObjectsFromCircle(center, rad, returnVec);
    return returnVec;
}

template<class T>
inline Container::Vector::Vector<T> QuadTree<T>::GetObjectsFromDepthAndPosition(size_t depth, const sf::Vector2f& position)
{
    Container::Vector::Vector<T> returnVec(50, 0, 25);
    m_root.GetObjectsFromDepthAndPosition(depth, position, returnVec);
    return returnVec;
}

#endif