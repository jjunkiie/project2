#pragma once

#include <cstddef>

template <typename T>
class Array {
public:
    // ======================== Constructors ========================

    Array() : m_data(nullptr), m_size(0) {}

    explicit Array(size_t size) : m_data(size > 0 ? new T[size]() : nullptr), m_size(size) {}

    Array(size_t size, const T& value) : m_data(size > 0 ? new T[size] : nullptr), m_size(size) {
        for (size_t i = 0; i < m_size; ++i) {
            m_data[i] = value;
        }
    }

    // =================== Copy and Move ===========================

    Array(const Array& other) : m_data(other.m_size > 0 ? new T[other.m_size] : nullptr), m_size(other.m_size) {
        for (size_t i = 0; i < m_size; ++i) {
            m_data[i] = other.m_data[i];
        }
    }

    Array(Array&& other) noexcept : m_data(other.m_data), m_size(other.m_size) {
        other.m_data = nullptr;
        other.m_size = 0;
    }

    Array& operator=(const Array& other) {
        if (this != &other) {
            T* new_data = other.m_size > 0 ? new T[other.m_size] : nullptr;
            for (size_t i = 0; i < other.m_size; ++i) {
                new_data[i] = other.m_data[i];
            }
            delete[] m_data;
            m_data = new_data;
            m_size = other.m_size;
        }
        return *this;
    }

    Array& operator=(Array&& other) noexcept {
        if (this != &other) {
            delete[] m_data;
            m_data = other.m_data;
            m_size = other.m_size;
            other.m_data = nullptr;
            other.m_size = 0;
        }
        return *this;
    }

    // =================== Destructor ==============================

    ~Array() {
        delete[] m_data;
    }

    // =================== Element access ==========================

    T& operator[](size_t index) {
        return m_data[index];
    }

    const T& operator[](size_t index) const {
        return m_data[index];
    }

    T& front() {
        return m_data[0];
    }

    const T& front() const {
        return m_data[0];
    }

    T& back() {
        return m_data[m_size - 1];
    }

    const T& back() const {
        return m_data[m_size - 1];
    }

    T* data() {
        return m_data;
    }

    const T* data() const {
        return m_data;
    }

    // =================== Capacity ================================

    size_t size() const {
        return m_size;
    }

    bool empty() const {
        return m_size == 0;
    }

    // =================== Operations ==============================

    void fill(const T& value) {
        for (size_t i = 0; i < m_size; ++i) {
            m_data[i] = value;
        }
    }

    void swap(Array& other) noexcept {
        T* tmp_data = m_data;
        size_t tmp_size = m_size;
        
        m_data = other.m_data;
        m_size = other.m_size;
        
        other.m_data = tmp_data;
        other.m_size = tmp_size;
    }

    // =================== Comparison ==============================

    bool operator==(const Array& rhs) const {
        if (m_size != rhs.m_size) return false;
        for (size_t i = 0; i < m_size; ++i) {
            if (m_data[i] != rhs.m_data[i]) return false;
        }
        return true;
    }

    bool operator!=(const Array& rhs) const {
        return !(*this == rhs);
    }

private:
    T* m_data = nullptr;
    size_t m_size = 0;
};
