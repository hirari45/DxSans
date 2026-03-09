#include "Bullet.h"
#include "globals.h"
#include <Dxlib.h>

Bullet::Bullet(const Vector2D& pos, const Vector2D& vel,
	unsigned int color, float radius, float life)
	:Base(pos, vel, color), radius_(radius), life_(life)
{
	SetObjType(BULLET);
}

void Bullet::Update()
{
	if(IsDead() == true)
		return; //死んでたらスルー

	float dt = GetDeltaTime();
	//pos_.x = pos_.x + vel_.x * dt;
	//pos_.y = pos_.y + vel_.y * dt;
	pos_ = Math2D::Add(pos_, Math2D::Mul(vel_, dt));

	//画面はじわーぷ
	if (pos_.x < 0) pos_.x = WIN_WIDTH;
	if (pos_.x > WIN_WIDTH) pos_.x = 0;
	if (pos_.y < 0) pos_.y = WIN_HEIGHT;
	if (pos_.y > WIN_HEIGHT) pos_.y = 0;
	//弾が何秒生きるか
	life_ = life_ - dt;

}

void Bullet::Draw()
{
	if (IsDead() == true)
		return; //死んでたらスルー
	Vector2D sPos = Math2D::World2Screen(pos_);
	DrawCircle(sPos.x, sPos.y, radius_, Color_, TRUE);
}
