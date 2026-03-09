#pragma once
#include <cmath>
//カプセル化しない方が楽なので、Vector2Dはカプセル化しない
class Vector2D
{
public:
	float x, y;//メンバ変数
	Vector2D(float _x = 0, float _y = 0);
	Vector2D Add(const Vector2D& _v); // this = this + _v;
	Vector2D ScalarMul(float k); // this = k * this;
	float Distance(const Vector2D& _v); //thisと_vの距離
};

struct Matrix2D
{
    // [ m00 m01 tx ]
    // [ m10 m11 ty ]
    // [  0   0   1 ]
    float m00, m01, tx;
    float m10, m11, ty;

    static Matrix2D Identity()
    {
        return { 1,0,0, 0,1,0 };
    }

    static Matrix2D Rotation(float rad)
    {
        float c = cosf(rad);
        float s = sinf(rad);
        return { c, -s, 0,  s, c, 0 };
    }

    static Matrix2D Translation(const Vector2D& t)
    {
        return { 1,0,t.x, 0,1,t.y };
    }

    inline Matrix2D Multiply(const Matrix2D& a, const Matrix2D& b)
    {
        return Matrix2D{
            a.m00 * b.m00 + a.m01 * b.m10,
            a.m00 * b.m01 + a.m01 * b.m11,
            a.m00 * b.tx + a.m01 * b.ty + a.tx,
            a.m10 * b.m00 + a.m11 * b.m10,
            a.m10 * b.m01 + a.m11 * b.m11,
            a.m10 * b.tx + a.m11 * b.ty + a.ty
        };
    }
};

inline Vector2D TransformPoint(const Vector2D& v, const Matrix2D& m)
{
    return Vector2D(
        m.m00 * v.x + m.m01 * v.y + m.tx,
        m.m10 * v.x + m.m11 * v.y + m.ty
    );
}

