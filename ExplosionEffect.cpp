#include "ExplosionEffect.h"
#include "DxLib.h"
#include "easefunction.h"

namespace
{
	const float PATICLE_LIFE = 1.0f;
	const float PATICLE_RADIUS = 1.5f;
	const float PATICLE_SPEED[3] = { 50.0f, 80.0f, 120.0f };
	const float PATICLE_DECAY = 0.95f;//パーティクルの速さの減衰率
	const float PATICLE_LIFE_FADE_START = PATICLE_LIFE / 2.0f;
}

ExplosionEffect::ExplosionEffect(const Vector2D& pos, int particleCount)
	:Base(pos, { 0.0f, 0.0f }, GetColor(255, 255, 255))
{
	particles_.clear();
	isFinished_ = false;

	//パーティクル1個の初期化
	for (int i = 0; i < particleCount; i++)
	{
		Particle particle;
		particle.Offset = { 0.0f, 0.0f };
		particle.vel = { 0.0f, 0.0f };
		particle.life = PATICLE_LIFE;
		particle.radius = PATICLE_RADIUS;
		particle.alpha = 1.0f;//府透明度1.0f

		float angle_rad = (float)GetRand(360) * (Math2D::PI / 180.0f);
		Vector2D direction = Math2D::FromAngle(angle_rad);
		particle.vel = Math2D::Mul(direction, PATICLE_SPEED[GetRand(2)]);

		//リストに追加
		//particles_[0] = particle;
		particles_.push_back(particle);
	}
	SetObjType(EFFECT);
}

void ExplosionEffect::Update()
{
	float dt = GetDeltaTime();
	bool allDead = true;

	for (auto& particle : particles_)
	{
		//寿命が減ってたら更新
		if (particle.life > 0.0f)
		{
			allDead = false;

			//位置更新
			particle.Offset = Math2D::Add(particle.Offset, Math2D::Mul(particle.vel, dt));

			//速度減衰
			//particle.vel = Math2D::Mul(particle.vel, PATICLE_DECAY);

			//寿命減少
			particle.life -= dt;
			if (particle.life < 0.0f)
				particle.life = 0.0f;
		}

		//不透明度の更新
	/*	if (particle.life < PATICLE_LIFE_FADE_START)
		{
			particle.alpha = particle.life / PATICLE_LIFE_FADE_START;
		}
		else
		{
			particle.alpha = 1.0f;
		}*/

		float lifeRatio = 1.0f - particle.life / PATICLE_LIFE;
		particle.alpha = 1.0f - Direct3D::EaseFunc["InExpo"](lifeRatio);

	}

	//全パーティクルが寿命切れならエフェクト終了
	if (allDead == true)
	{
		isFinished_ = true;
	}
}

void ExplosionEffect::Draw()
{

	int col[3];
	GetColor2(GetCharaColor(), &col[0], &col[1], &col[2]);

	if (isFinished_) return;

	for (auto& particle : particles_)
	{
		if (particle.life > 0.0f)
		{
			Vector2D drawPos = Math2D::Add(GetPos(), particle.Offset);
			Vector2D screenPos = Math2D::World2Screen(drawPos);
			int particleColor = GetColor(
				(int)(col[0] * particle.alpha),
				(int)(col[1] * particle.alpha),
				(int)(col[2] * particle.alpha)
			);
			DrawCircle((int)screenPos.x,
				(int)screenPos.y,
				particle.radius,
				particleColor);
		}
	}
}
