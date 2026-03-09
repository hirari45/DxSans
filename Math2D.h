#pragma once
#include <cmath>
#include "globals.h"

///////////////////////////////////////////////
// 2D ベクトル型
///////////////////////////////////////////////
struct Vector2D
{
	float x;
	float y;
	Vector2D()
		:x(0),y(0){ }

	Vector2D(float _x, float _y)
		:x(_x), y(_y)
	{
	}
};

///////////////////////////////////////////////
// 2D アフィン変換行列
// [ m00 m01 tx]
// [ m10 m11 ty]
// [ 0   0   1 ]
///////////////////////////////////////////////

struct Mat2
{
	float m00, m01, tx;
	float m10, m11, ty;
	//    0    0    1
};


namespace Math2D
{
	// ---- 定数 ----
	const float PI = 3.14159265359f;
	const float PI2 = 6.2831853072f;

	inline Vector2D World2Screen(const Vector2D& wpos)
	{
		Vector2D tmp;//temporary=作業領域
		tmp.x = wpos.x;
		tmp.y = WIN_HEIGHT - wpos.y;
		return(tmp);
	}

	// ---- ベクトル基本 ----

	// 2つのベクトルを加算する（a + b）
	inline Vector2D Add(const Vector2D& a, const Vector2D& b)
	{
		return { a.x + b.x, a.y + b.y };
	}

	// 2つのベクトルを減算する（a - b）
	inline Vector2D Sub(const Vector2D& a, const Vector2D& b)
	{
		return {a.x - b.x, a.y - b.y  };
	}

	// ベクトルをスカラー倍する（v × s）
	inline Vector2D Mul(const Vector2D& v, float s)
	{
		return {s*v.x, s*v.y};
	}

	// ---- 長さ ----

	// ベクトルの長さの2乗を求める（√を使わないので高速）
	inline float LengthSq(const Vector2D& v)
	{
		return  (v.x*v.x + v.y*v.y);
	}

	// ベクトルの長さ（ノルム）を求める
	inline float Length(const Vector2D& v)
	{
		return (sqrt( LengthSq(v) ) );
	}

	// ベクトルを正規化（長さ1のベクトルにする）
	// 長さが0に近い場合は、安全のため右向き(1,0)を返す
	inline Vector2D Normalize(const Vector2D& v)
	{
		float len = Length(v);
		if (len < 0.000001f) return { 1.0f, 1.0f }; //エラー ごまかす
		return {v.x/len, v.y/len };
	}

	// ---- 内積・外積 ----

	// 内積（Dot Product）
	// 2つのベクトルの向きの近さを表す
	inline float Dot(const Vector2D& a, const Vector2D& b)
	{
		return(a.x*b.x + a.y*b.y);
	}

	// 外積（2Dなのでスカラー値）
	// 正負で左右関係を判定できる
	inline float Cross(const Vector2D& a, const Vector2D& b)
	{
		return (a.x*b.y - b.x*a.y);
	}

	// ---- 角度 ----

	// ベクトルの向きを角度（ラジアン）で取得する
	// atan2 を使うことで全方向を正しく扱える
	inline float Angle(const Vector2D& v)
	{
		return (atan2(v.y, v.x));
	}

	// 角度（ラジアン）から単位ベクトルを作る
	inline Vector2D FromAngle(float rad)
	{
		return {cos(rad), sin(rad)};
	}

	// ---- 行列生成 ----

	// 単位行列（変換なし）
	inline Mat2 Identity()
	{
		//Mat2 tmp
		//{
		//	1, 0, 0
		//	0, 1, 0
		//  0, 0, 1
		//};
		return { 1, 0, 0, 
			     0, 1, 0 };
	}

	// 回転行列を作る（原点中心で回転）
	inline Mat2 Rotation(float rad)
	{
		float c = std::cos(rad);
		float s = std::sin(rad);
		return {c, -s, 0,
			    s,  c, 0 };
	}

	// 平行移動行列を作る
	inline Mat2 Translation(const Vector2D& t)
	{
		return {1, 0, t.x,
		        0, 1, t.y };
	}

	// ---- 行列合成 ----

	// 2つのアフィン行列を合成する（a の後に b を適用）
	//[a.m00 a.m01, a.tx][b.m00 b.m01, b.tx]
    //[a.m10 a.m11, a.ty][b.m10 b.m11, b.ty]
	//[0,    0,     1   ][0,    0,     1   ]

	inline Mat2 Multiply(const Mat2& a, const Mat2& b)
	{

		//Mat2 res;
		//res.m00 = a.m00 * b.m00 + a.m01 * b.m10 + a.tx * 0;
		//res.m01 = a.m00 * b.m01 + a.m01 * b.m11 + a.tx * 0;
		//res.tx = a.m00 * b.tx  + a.m01 * b.ty  + a.tx * 1;
		//
		//res.m10 = a.m10 * b.m00 + a.m11 * b.m10 + a.ty * 0;
		//res.m11 = a.m10 * b.m01 + a.m11 * b.m11 + a.ty * 0;
		//res.ty =  a.m10 * b.tx  + a.m11 * b.ty  + a.ty * 1;
		//return res;
		return {
			a.m00* b.m00 + a.m01 * b.m10,
			a.m00* b.m01 + a.m01 * b.m11,
			a.m00* b.tx + a.m01 * b.ty + a.tx,

			a.m10* b.m00 + a.m11 * b.m10,
			a.m10* b.m01 + a.m11 * b.m11,
			a.m10* b.tx + a.m11 * b.ty + a.ty
		};
	}

	// ---- 行列 × 点(ベクトル） ----

	// 点（ベクトル）にアフィン変換を適用する
	// 回転・平行移動をまとめて行う
	//[m.m00 m.m01, m.tx][v.x]
	//[m.m10 m.m11, m.ty][v.y] = ????
	//[0,    0,     1   ][1  ]
	inline Vector2D TransformPoint(const Vector2D& v, const Mat2& m)
	{
		Vector2D res(0, 0);
		res.x = m.m00 * v.x + m.m01 * v.y + m.tx;
		res.y = m.m10 * v.x + m.m11 * v.y + m.ty;
		return res;
	}
}
