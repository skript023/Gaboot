#pragma once

namespace gaboot
{
    template <typename T>
    class Singleton 
    {
    public:
        static T& instance()
        {
            static T instance;
            return instance;
        }
    private:
        Singleton() = default;

        ~Singleton() noexcept = default;

        Singleton(const Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;
    };
}