#include "Enemy.h"
#include "globals.h"
#include <DxLib.h>
#include "Math2D.h"

namespace
{
	const int SEGMENT_NUM = 8;//円の分割数
	//位置はコンストラクタでランダムに決定
	//速度もコンストラクタでランダムに決定（範囲あり、方向と大きさ）
	//半径もコンストラクタでランダムに決定（範囲あり）
	const float MIN_RADIUS = 25.0f;//最低半径
	const float MAX_OMEGA = 2.0f;
	const float SMALL_RADIUS_MIN = 20.0f;//小
	const float SMALL_RADIUS_MAX = 40.0f;
	const float MEDIUM_RADIUS_MIN = 41.0f;//中
	const float MEDIUM_RADIUS_MAX = 60.0f;
	const float LARGE_RADIUS_MIN = 61.0f;//大
	const float LARGE_RADIUS_MAX = 80.0f;
}


Enemy::Enemy(int segment)
	:Base(), segment_(segment)
{
	spawnTimer_ = 0.0f;
	spawnInterval_ = 2.0f;
	shouldSpawn_ = false;
	//pos_を画面内のランダムな位置に設定
	pos_ = { (float)GetRand(WIN_WIDTH - 1), (float)GetRand(WIN_HEIGHT - 1) };
	//x,yともに-100から100の間のランダムな速度
	vel_ = { (float)(GetRand(200) - 100),(float)(GetRand(200) - 100) };
	Color_ = GetColor(255, 255, 255);//白
	if (segment_ < SEGMENT_NUM)
		segment_ = SEGMENT_NUM;//最低8分割
	radius_ = (float)(GetRand(50) + MIN_RADIUS);//25から75の間のランダムな半径
	size_ = checkSize();
	vertex_.resize(segment_);//頂点配列のサイズ変更　.resize(segment_)で変更できる
	omega_ = ((float)GetRand(200) - 100) / 100.0f * DX_PI;//
	isAlive_ = true;
	MakeShape();

	SetObjType(ENEMY);
}

Enemy::Enemy(Size size, int segment)
	:Base(), segment_(segment), isAlive_(true), size_(size)
{
	spawnTimer_ = 0.0f;
	spawnInterval_ = 2.0f;
	shouldSpawn_ = false;
	//pos_を画面内のランダムな位置に設定
	pos_ = { (float)GetRand(WIN_WIDTH - 1), (float)GetRand(WIN_HEIGHT - 1) };
	//x,yともに-100から100の間のランダムな速度
	vel_ = { (float)(GetRand(200) - 100),(float)(GetRand(200) - 100) };
	Color_ = GetColor(255, 255, 255);//白
	if (segment_ < SEGMENT_NUM)
		segment_ = SEGMENT_NUM;//最低8分割
	radius_ = RandomRadius(size_);
	vertex_.resize(segment_);
	angle_ = 0.0f;
	omega_ = ((float)GetRand(200) - 100) / 100.0f * DX_PI;//
	MakeShape();
	SetObjType(ENEMY);
}

Enemy::Enemy(const Vector2D& pos, const Vector2D& vel, Size size, int segment)
	:Base(pos, vel, GetColor(255, 255, 255)), segment_(segment), isAlive_(true), size_(size)
{
	spawnTimer_ = 0.0f;
	spawnInterval_ = 2.0f;
	shouldSpawn_ = false;
	if (segment_ < SEGMENT_NUM)
		segment_ = SEGMENT_NUM;//最低8分割
	radius_ = RandomRadius(size_);
	vertex_.resize(segment_);
	angle_ = 0.0f;
	omega_ = ((float)GetRand(200) - 100) / 100.0f * DX_PI;//
	MakeShape();
	SetObjType(ENEMY);
}

void Enemy::Update()
{
	if (size_ == Size::LARGE)
	{
		spawnTimer_ += 1.0f / 60.0f;

		if (spawnTimer_ >= spawnInterval_)
		{
			spawnTimer_ = 0.0f;
			shouldSpawn_ = true;
		}
	}

	if (IsAlive() == false)
		return;
	float dt = GetDeltaTime();
	//
	//
	// SMALL のときだけ減速
	if (size_ == Size::SMALL)
	{
		float friction = 1.5f;

		vel_.x -= vel_.x * friction * dt;
		vel_.y -= vel_.y * friction * dt;

		// ほぼ0になったら完全停止
		if (fabs(vel_.x) < 1.0f) vel_.x = 0.0f;
		if (fabs(vel_.y) < 1.0f) vel_.y = 0.0f;
	}
	pos_ = Math2D::Add(pos_, Math2D::Mul(vel_, dt));
	//
	if (pos_.x < 0) pos_.x = WIN_WIDTH;
	if (pos_.x > WIN_WIDTH) pos_.x = 0;
	if (pos_.y < 0) pos_.y = WIN_HEIGHT;
	if (pos_.y > WIN_HEIGHT)pos_.y = 0;

	angle_ = angle_ + omega_ * dt;
}

void Enemy::Draw()
{
	if (IsAlive() == false)
		return;
	//スクリーン座標に変換した頂点配列を作る
	std::vector<Vector2D> scrVertex(segment_);

	for (int i = 0; i < segment_; i++)
	{
		Mat2 rotMat = Math2D::Rotation(angle_);
		scrVertex[i] = Math2D::TransformPoint(vertex_[i], rotMat);
		scrVertex[i] = Math2D::Add(pos_, scrVertex[i]);
		scrVertex[i] = Math2D::World2Screen(scrVertex[i]);
	}
	for (int i = 0; i < segment_; i++)
	{
		Vector2D Start = scrVertex[i];
		Vector2D End = scrVertex[(i + 1) % segment_];

		DrawLineAA(
			Start.x, Start.y,
			End.x, End.y,
			GetColor(255, 255, 255), 1.0f);
	}
}

Enemy::Size Enemy::checkSize() const
{
	if (radius_ <= SMALL_RADIUS_MAX)
		return Size::SMALL;
	else if (radius_ <= MEDIUM_RADIUS_MAX)
		return Size::MEDIUM;
	else
		return Size::LARGE;
}

void Enemy::MakeShape()
{
	for (int i = 0; i < segment_; i++)
	{
		float angle = (2.0f * DX_PI / segment_) * i;
		float r2 = radius_ / 2.0f;
		float length = r2 + r2 * (float)(GetRand(100)) / 100.0f;

		vertex_[i] = { length * cosf(angle), length * sinf(angle) };
	}
}

float Enemy::RandomRadius(Size size)
{
	switch (size)
	{
	case Size::SMALL:
		return  SMALL_RADIUS_MIN + (float)GetRand((int)SMALL_RADIUS_MAX - SMALL_RADIUS_MIN);
	case Size::MEDIUM:
		return  MEDIUM_RADIUS_MIN + (float)GetRand((int)MEDIUM_RADIUS_MAX - MEDIUM_RADIUS_MIN);
	case Size::LARGE:
		return  LARGE_RADIUS_MIN + (float)GetRand((int)LARGE_RADIUS_MAX - LARGE_RADIUS_MIN);
	default:
		return 1.0f;
	}
}

bool Enemy::ShouldSpawn()
{
	if (shouldSpawn_)
	{
		shouldSpawn_ = false;  // 一回だけにする
		return true;
	}
	return false;
}
