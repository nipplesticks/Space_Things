#ifndef _VECTOR_H_
#define _VECTOR_H_
#include <memory>

namespace Container
{
    namespace Vector
    {
        template <class T>
        class Vector
        {
        public:
            Vector(size_t capacity = 1, size_t elementCount = 0, size_t increment = 5);
            Vector(const Vector<T>& other);
            ~Vector();

            void PushBack(const T& element);
            void PushBack(const Vector<T>& elements);
            void PushBack(const Vector<T>& elements, size_t start, size_t end);

            void PushFront(const T& element);
            void PushFront(const Vector<T>& elements);
            void PushFront(const Vector<T>& elements, size_t start, size_t end);

            void Insert(const T& element, size_t index);
            void Insert(const Vector<T>& elements, size_t index);
            void Insert(const Vector<T>& elements, size_t index, size_t start, size_t end);

            void EraseElement(const T& element);
            void Erase(size_t index);
            void Erase(size_t start, size_t end);

            void PopBack();
            void PopFront();

            void Reverse();
            void Reverse(size_t start, size_t end);

            void Shuffle(size_t iterations);
            void Shuffle(size_t iterations, size_t start, size_t end);

            void Clear(bool hard = false);
            void Resize(size_t size);
            void ShrinkToFit();


            T& At(size_t index) const;
            T* Data() const;
            T& Front() const;
            T& Back() const;

            size_t Capacity() const;
            size_t Size() const;
            bool Empty() const;

            T& operator[](size_t index) const;
            Vector<T>& operator=(const Vector<T>& other);

        private:
            void _cleanup();
            void _copy(const Vector<T>& other);
            void _resize(size_t newSize);

        private:
            T* m_data;
            size_t m_capacity = 1;
            size_t m_elementCount = 0;
            size_t m_increment = 5;
        };
        template<class T>
        inline Vector<T>::Vector(size_t capacity, size_t elementCount, size_t increment)
        {
            m_capacity = capacity;
            m_increment = increment;
            m_elementCount = elementCount;
            m_data = new T[m_capacity];
        }
        template<class T>
        inline Vector<T>::Vector(const Vector<T>& other)
        {
            _copy(other);
        }
        template<class T>
        inline Vector<T>::~Vector()
        {
            _cleanup();
        }
        template<class T>
        inline void Vector<T>::PushBack(const T& element)
        {
            if (m_elementCount == m_capacity)
                _resize(m_increment + m_capacity);

            m_data[m_elementCount++] = element;
        }
        template<class T>
        inline void Vector<T>::PushBack(const Vector<T>& elements)
        {
            if (elements.m_elementCount == 0) return;

            if (m_elementCount + elements.m_elementCount >= m_capacity)
                _resize((m_capacity - m_elementCount) + elements.m_elementCount + m_increment + m_capacity);

            for (size_t i = 0; i < elements.m_elementCount; i++)
            {
                m_data[m_elementCount++] = elements.m_data[i];
            }
        }
        template<class T>
        inline void Vector<T>::PushBack(const Vector<T>& elements, size_t start, size_t end)
        {
            if (elements.m_elementCount == 0) return;
            size_t count = end - start;
            if (m_elementCount + count >= m_capacity)
                _resize((m_capacity - m_elementCount) + count + m_increment + m_capacity);

            for (size_t i = start; i < end; i++)
                m_data[m_elementCount++] = elements.m_data[i];
        }
        template<class T>
        inline void Vector<T>::PushFront(const T& element)
        {
            if (m_elementCount == m_capacity)
                _resize(m_increment + m_capacity);

            for (size_t i = m_elementCount; i > 0; i--)
                m_data[i] = m_data[i - 1];

            m_data[0] = element;
            m_elementCount++;
        }
        template<class T>
        inline void Vector<T>::PushFront(const Vector<T>& elements)
        {
            if (elements.m_elementCount == 0) return;
            if (m_elementCount + elements.m_elementCount >= m_capacity)
                _resize((m_capacity - m_elementCount) + elements.m_elementCount + m_increment + m_capacity);

            for (size_t i = m_elementCount; i > 0; i--)
            {
                m_data[i + elements.m_elementCount - 1] = m_data[i - 1];
            }

            for (size_t i = 0; i < elements.m_elementCount; i++)
            {
                m_data[i] = elements.m_data[i];
            }

            m_elementCount += elements.m_elementCount;
        }
        template<class T>
        inline void Vector<T>::PushFront(const Vector<T>& elements, size_t start, size_t end)
        {
            if (elements.m_elementCount == 0) return;
            size_t count = end - start;
            if (m_elementCount + count >= m_capacity)
                _resize((m_capacity - m_elementCount) + count + m_increment + m_capacity);

            for (size_t i = m_elementCount; i > 0; i--)
                m_data[i + count - 1] = m_data[i - 1];

            for (size_t i = 0; i < count; i++)
                m_data[i] = elements.m_data[start + i];

            m_elementCount += count;
        }
        template<class T>
        inline void Vector<T>::Insert(const T& element, size_t index)
        {
            if (m_elementCount == m_capacity)
                _resize(m_increment + m_capacity);

            for (size_t i = m_elementCount; i > index; i--)
                m_data[i] = m_data[i - 1];

            m_data[index] = element;
            m_elementCount++;
        }
        template<class T>
        inline void Vector<T>::Insert(const Vector<T>& elements, size_t index)
        {
            if (elements.m_elementCount == 0) return;
            if (m_elementCount + elements.m_elementCount >= m_capacity)
                _resize((m_capacity - m_elementCount) + elements.m_elementCount + m_increment + m_capacity);

            for (size_t i = m_elementCount; i > index; i--)
                m_data[i + elements.m_elementCount] = m_data[i - 1];

            for (size_t i = 0; i < elements.m_elementCount; i++)
                m_data[i + index] = elements.m_data[i];

            m_elementCount += elements.m_elementCount;
        }
        template<class T>
        inline void Vector<T>::Insert(const Vector<T>& elements, size_t index, size_t start, size_t end)
        {
            if (elements.m_elementCount == 0) return;
            size_t count = end - start;
            if (m_elementCount + count >= m_capacity)
                _resize((m_capacity - m_elementCount) + count + m_increment + m_capacity);

            for (size_t i = m_elementCount; i > index; i--)
                m_data[i + count - 1] = m_data[i - 1];

            for (size_t i = 0; i < count; i++)
                m_data[i + index] = elements.m_data[start + i];

            m_elementCount += count;
        }
        template<class T>
        inline void Vector<T>::EraseElement(const T& element)
        {
            for (size_t i = 0; i < m_elementCount; i++)
            {
                if (element == m_data[i])
                {
                    Erase(i);
                    break;
                }
            }
        }
        template<class T>
        inline void Vector<T>::Erase(size_t index)
        {
            m_elementCount--;
            for (size_t i = index; i < m_elementCount; i++)
                m_data[i] = m_data[i + 1];
        }
        template<class T>
        inline void Vector<T>::Erase(size_t start, size_t end)
        {
            size_t count = end - start;

            for (size_t i = 0; i < count; i++)
                m_data[start + i] = m_data[end + i];

            m_elementCount -= count;
        }
        template<class T>
        inline void Vector<T>::PopBack()
        {
            m_elementCount--;
        }
        template<class T>
        inline void Vector<T>::PopFront()
        {
            m_elementCount--;
            for (size_t i = 0; i < m_elementCount; i++)
                m_data[i] = m_data[i + 1];
        }
        template<class T>
        inline void Vector<T>::Reverse()
        {
            size_t counter = m_elementCount - 1;
            for (size_t i = 0; i < m_elementCount; i++)
            {
                if (counter < i) break;

                T temp = m_data[i];
                m_data[i] = m_data[counter];
                m_data[counter--] = temp;
            }
        }
        template<class T>
        inline void Vector<T>::Reverse(size_t start, size_t end)
        {
            size_t counter = end - 1;
            for (size_t i = start; i < m_elementCount; i++)
            {
                if (counter < i) break;

                T temp = m_data[i];
                m_data[i] = m_data[counter];
                m_data[counter--] = temp;
            }
        }
        template<class T>
        inline void Vector<T>::Shuffle(size_t iterations)
        {
            for (size_t i = 0; i < iterations; i++)
            {
                size_t i1 = rand() % m_elementCount;
                size_t i2 = rand() % m_elementCount;

                T temp = m_data[i1];
                m_data[i1] = m_data[i2];
                m_data[i2] = temp;
            }
        }
        template<class T>
        inline void Vector<T>::Shuffle(size_t iterations, size_t start, size_t end)
        {
            for (size_t i = 0; i < iterations; i++)
            {
                size_t i1 = rand() % (end - start) + start;
                size_t i2 = rand() % (end - start) + start;

                T temp = m_data[i1];
                m_data[i1] = m_data[i2];
                m_data[i2] = temp;
            }
        }
        template<class T>
        inline void Vector<T>::Clear(bool hard)
        {
            m_elementCount = 0;
            if (hard) Resize(1);
        }
        template<class T>
        inline void Vector<T>::Resize(size_t size)
        {
            T* Data = new T[size];

            for (size_t i = 0; i < size; i++)
                Data[i] = m_data[i];
            delete[] m_data;
            m_data = Data;
            m_capacity = size;
            m_elementCount = m_elementCount < size ? m_elementCount : size;
        }
        template<class T>
        inline void Vector<T>::ShrinkToFit()
        {
            T* Data = new T[m_elementCount];
            for (size_t i = 0; i < m_elementCount; i++)
                Data[i] = m_data[i];
            delete[] m_data;
            m_data = Data;
            m_capacity = m_elementCount;
        }
        template<class T>
        inline T& Vector<T>::At(size_t index) const
        {
            return m_data[index];
        }
        template<class T>
        inline T* Vector<T>::Data() const
        {
            return m_data;
        }
        template<class T>
        inline T& Vector<T>::Front() const
        {
            return m_data[0];
        }
        template<class T>
        inline T& Vector<T>::Back() const
        {
            return m_data[m_elementCount - 1];
        }
        template<class T>
        inline size_t Vector<T>::Capacity() const
        {
            return m_capacity;
        }
        template<class T>
        inline size_t Vector<T>::Size() const
        {
            return m_elementCount;
        }
        template<class T>
        inline bool Vector<T>::Empty() const
        {
            return m_elementCount == 0;
        }
        template<class T>
        inline T& Vector<T>::operator[](size_t index) const
        {
            return m_data[index];
        }
        template<class T>
        inline Vector<T>& Vector<T>::operator=(const Vector<T>& other)
        {
            if (this != &other)
            {
                _cleanup();
                _copy(other);
            }

            return *this;
        }
        template<class T>
        inline void Vector<T>::_cleanup()
        {
            delete[] m_data;
            m_data = nullptr;
            m_elementCount = 0;
            m_capacity = 0;
            m_increment = 0;
        }
        template<class T>
        inline void Vector<T>::_copy(const Vector<T>& other)
        {
            m_elementCount = other.m_elementCount;
            m_capacity = other.m_capacity;
            m_increment = other.m_increment;
            m_data = new T[m_capacity];

            for (size_t i = 0; i < m_elementCount; i++)
                m_data[i] = other.m_data[i];
        }
        template<class T>
        inline void Vector<T>::_resize(size_t newSize)
        {
            T* Data = new T[newSize];

            size_t count = (newSize < m_elementCount) ? newSize : m_elementCount;

            for (size_t i = 0; i < count; i++)
            {
                Data[i] = m_data[i];
            }

            delete[] m_data;
            m_data = Data;
            m_capacity = newSize;
        }
    }
}

#endif
