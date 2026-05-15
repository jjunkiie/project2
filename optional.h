#pragma once

#include <new>
#include <type_traits>
#include <utility>

template <typename T>
class Optional {
public:
    // ======================== Constructors ========================

    Optional() : m_has_value(false) {}

    Optional(const T& value) : m_has_value(true) {
        new (m_storage) T(value);
    }

    Optional(T&& value) : m_has_value(true) {
        new (m_storage) T(std::move(value));
    }

    // =================== Copy and Move ===========================

    Optional(const Optional& other) : m_has_value(other.m_has_value) {
        if (m_has_value) {
            new (m_storage) T(other.value());
        }
    }

    Optional(Optional&& other) : m_has_value(other.m_has_value) {
        if (m_has_value) {
            new (m_storage) T(std::move(other.value()));
        }
    }

    Optional& operator=(const Optional& other) {
        if (this != &other) {
            if (m_has_value && other.m_has_value) {
                value() = other.value();
            } else if (!m_has_value && other.m_has_value) {
                new (m_storage) T(other.value());
                m_has_value = true;
            } else if (m_has_value && !other.m_has_value) {
                reset();
            }
        }
        return *this;
    }

    Optional& operator=(Optional&& other) {
        if (this != &other) {
            if (m_has_value && other.m_has_value) {
                value() = std::move(other.value());
            } else if (!m_has_value && other.m_has_value) {
                new (m_storage) T(std::move(other.value()));
                m_has_value = true;
            } else if (m_has_value && !other.m_has_value) {
                reset();
            }
        }
        return *this;
    }

    Optional& operator=(const T& value) {
        if (m_has_value) {
            this->value() = value;
        } else {
            new (m_storage) T(value);
            m_has_value = true;
        }
        return *this;
    }

    Optional& operator=(T&& value) {
        if (m_has_value) {
            this->value() = std::move(value);
        } else {
            new (m_storage) T(std::move(value));
            m_has_value = true;
        }
        return *this;
    }

    // =================== Destructor ==============================

    ~Optional() {
        reset();
    }

    // =================== Observers ===============================

    bool has_value() const {
        return m_has_value;
    }

    explicit operator bool() const {
        return m_has_value;
    }

    T& value() {
        return *reinterpret_cast<T*>(m_storage);
    }

    const T& value() const {
        return *reinterpret_cast<const T*>(m_storage);
    }

    T value_or(const T& default_value) const {
        return m_has_value ? value() : default_value;
    }

    T& operator*() {
        return value();
    }

    const T& operator*() const {
        return value();
    }

    T* operator->() {
        return reinterpret_cast<T*>(m_storage);
    }

    const T* operator->() const {
        return reinterpret_cast<const T*>(m_storage);
    }

    // =================== Modifiers ===============================

    template <typename... Args>
    T& emplace(Args&&... args) {
        reset();
        new (m_storage) T(std::forward<Args>(args)...);
        m_has_value = true;
        return value();
    }

    void reset() {
        if (m_has_value) {
            value().~T();
            m_has_value = false;
        }
    }

    void swap(Optional& other) {
        if (m_has_value && other.m_has_value) {
            std::swap(value(), other.value());
        } else if (m_has_value && !other.m_has_value) {
            new (other.m_storage) T(std::move(value()));
            other.m_has_value = true;
            reset();
        } else if (!m_has_value && other.m_has_value) {
            new (m_storage) T(std::move(other.value()));
            m_has_value = true;
            other.reset();
        }
    }

    // =================== Comparison ==============================

    bool operator==(const Optional& rhs) const {
        if (!m_has_value && !rhs.m_has_value) return true;
        if (m_has_value != rhs.m_has_value) return false;
        return value() == rhs.value();
    }

    bool operator!=(const Optional& rhs) const {
        return !(*this == rhs);
    }

private:
    alignas(T) unsigned char m_storage[sizeof(T)];
    bool m_has_value = false;
};
