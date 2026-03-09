#pragma once
#include "Base.h"
class Player :
    public Base
{
public:
    Player();
    Player(const Vector2D& pos, const Vector2D& vel, unsigned int color,
        const Vector2D& dir, float r, float omega);
    ~Player();
    void Update() override;
    void Draw() override;
public://セッター、ゲッターは全部インラインで書いたわよ
    void SetDirVec(const Vector2D& dir) { dir_ = dir; }
    Vector2D GetDirVec() { return dir_; }
    void SetRadius(float r) { radius_ = r; }
    float GetRadius() { return radius_; }
    void SetOmega(float omega) { omega_ = omega; }
    float GetOmega() { return omega_; }
    void SetAngle(float angle) { angle_ = angle; }
    float GetAngele() { return angle_; }
    bool IsAlive() { return isAlive_; }
    void SetCollisionRadius(float r) { collisionRadius_ = r; }
    float GetCollisionRadius() { return collisionRadius_; }
    void Dead() { isAlive_ = false; }
private:
    //3角形の1点の座標
    Vector2D dir_; //自分の向き
    float radius_; //半径
    float collisionRadius_;
    Vector2D vertex_[3];//三角形の頂点座標
    float omega_; //回転速度(角速度)
    float angle_; //現在の回転角
    bool isAlive_;
};

