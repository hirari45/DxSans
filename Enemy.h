#pragma once
#include "Base.h"
#include <vector>


class Enemy :
    public Base
{
public:
    enum Size
    {
        LARGE,
        MEDIUM,
        SMALL
    };
    Enemy(int segment);
    Enemy(Size size, int segment);
    Enemy(const Vector2D& pos, const Vector2D& vel, Size size, int segment);

    void Update() override;
    void Draw() override;
    bool IsAlive() { return isAlive_; }
    float GetCollisionRadius() const { return radius_; }
    void Dead() { isAlive_ = false; }
    Size GetSize() { return size_; }
    Size checkSize()const;

    bool ShouldSpawn();

private:
    void MakeShape();//頂点座標初期化
    float RandomRadius(Size size);//サイズに応じたランダムな半径取得


private:
    int segment_;//円の分割数
    float radius_;//半径
    float omega_;//回転速度（角速度）
    float angle_;//現在の回転角

    float spawnTimer_;
    float spawnInterval_;
    bool shouldSpawn_;

    Size size_;//大中小のサイズ情報

    bool isAlive_;
    std::vector<Vector2D> vertex_;//円の頂点座標
};

