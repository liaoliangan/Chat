//
// Created by 33717 on 2026/1/10.
//

#ifndef SIGNALTEN_H
#define SIGNALTEN_H

#include"global.h"

template <typename T>
class Singalton
{
protected:
    Singalton() = default;
    Singalton(const Singalton&) = delete;
    Singalton& operator=(const Singalton&) = delete;


    static std::shared_ptr<T> __instance;

public:
    ~Singalton() { std::cout << "~Singalton()" << std::endl; }

    static std::shared_ptr<T> getInstance()
    {
        static std::once_flag flag;
        std::call_once(flag, [&]()
        {
            __instance = std::shared_ptr<T>(new T);
        });
        return __instance;
    }

    void PrintAddress()
    {
        std::cout << __instance.get() << std::endl;
    }
};

template <typename T>
std::shared_ptr<T> Singalton<T>::__instance = nullptr;

#endif //SIGNALTEN_H
