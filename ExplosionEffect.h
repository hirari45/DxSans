#pragma once
#include "Base.h"
#include <vector>

class ExplosionEffect :
    public Base
{
public:
    ExplosionEffect(const Vector2D& pos, int particleCount = 20);
    void Update() override;
    void Draw() override;
    bool IsFinished() const { return isFinished_; }

    struct Particle
    {
        Vector2D Offset;
        Vector2D vel;
        float life;
        float radius;
        float alpha;//•s“§–¾“x
    };
private:
    bool isFinished_;
    std::vector<Particle> particles_;
};

