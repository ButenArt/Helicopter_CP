#pragma once
#include <iostream>
#include <cmath>
/*
* Этот класс реалзует функциональность электронного энкодера для МАБа
* на вход получает значение с МАБ крутилки, которая может быть нормирована любым числом
* наример 10, что означает - один оборот 10 градусов,оборотов может быть много
* эти обороты надо суммировать и вычитать в зависимости от наравления вращения
* предологаем, что вращенеи достаточно медленное, что бы значения в разных итерациях
* не превышали четверть оборота, это позволит отловить наравленеи при перескоке через 0
* смещение против часовой стрелки считается как pos - base
*/
class Encoder 
{
    float val_ = 0.f;
    float prev_ = 0.f;
    float base_ = 0.f;
    float sens_ = 0.f;
public:
    Encoder(float base, float sens = 0.0001f) : base_(base), sens_(sens){};
    float get_prev() { return prev_; }
    float pred_set = 0.f;
    void reset() { val_ = 0; };
    float pos(float pos)
    {
        auto delta = pos - prev_;
        if (std::fabs(delta) > base_ * sens_)  // если положенеи ручки не изменилось + фильтр на дрожание
        {
            if (std::fabs(delta) < base_ / 2) 
            {
                val_ += delta;   // если положение изменилось не намного
            }
            else   // положение перескочило через 0
            {
//                val_ += (delta < 0.f) ? delta + base_ : delta - base_;
                // https://www.cplusplus.com/reference/cmath/copysign/
                val_ += delta - std::copysign(base_,delta);
            }
            prev_ = pos;
        }
        return val_;
    }
    operator float() { return val_; };
};

