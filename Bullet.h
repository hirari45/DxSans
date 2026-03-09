#pragma once
#include "Base.h"
#include "math2D.h"

class Bullet :
    public Base
{
public :
    Bullet(const Vector2D& pos, const Vector2D& vel, unsigned int color,
        float radius, float life);
    void Update() override;
    void Draw() override;
	void Dead() { life_ = 0.0f; }
    bool IsDead() const { return life_ <= 0.0f; }
    float GetRadius() const { return radius_; }
private:
    float radius_; //”¼Œa
    float life_; //’e‚ÌŽõ–½(•bj
};

