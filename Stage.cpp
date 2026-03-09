#include "Stage.h"
#include "Math2D.h"
#include "Player.h"
#include "globals.h"
#include <DxLib.h>
#include "Bullet.h"
#include "Input.h"
#include <vector>
#include "Enemy.h"
#include "ExplosionEffect.h"
#include "Base.h"


namespace
{
	const Vector2D START_POS = { WIN_WIDTH / 2, WIN_HEIGHT / 2 };
	const Vector2D START_VEL = { 0.0f, 0.0f };
	const Vector2D STAR_DIR = { 0.0f, -1.0f };
	const float START_RADIUS = 30.0f;
	const float START_OMEGA = 2.0f;
	const unsigned int START_COLOR = GetColor(255, 0, 0);
	const float PLAYER_COLLISION_RADIUS = 15.0f;
	const unsigned int ENEMY_MAX = 100;
	const unsigned int ENEMY_NUM = 3;//最初に登場する敵

	int stageState = 0;
	//Player* player = nullptr;
	//Enemy* enemy = nullptr;
	//std::vector<Bullet*> bullets;
	//std::vector<Enemy*> enemies;
	//std::vector <ExplosionEffect*> effects;

	std::vector<Base*> objects;//すべてのオブジェクトの保管庫

	void AddObject(Base* obf)
	{
		objects.push_back(obf);
	}
	void UpdateAllObjects()
	{
		for (auto& obj : objects)
		{
			obj->Update();
		}
	}
	void DrawAllObjects()
	{
		for (auto& obj : objects)
		{
			obj->Draw();
		}
	}
}

Stage::Stage()
{
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
	objects.clear();

	stageState = 0;

	gameScore_ = 0;

	Player* player = new Player(START_POS, START_VEL, START_COLOR,
		STAR_DIR, START_RADIUS, START_OMEGA);

	player->SetCollisionRadius(PLAYER_COLLISION_RADIUS);

	AddObject(player);

	for (int i = 0; i < ENEMY_NUM; i++)
	{
		Enemy* e = new Enemy(Enemy::Size::LARGE, 8);
		AddObject(e);
	}
}

void Stage::TitleUpdate()
{
	if (Input::IsKeyDown(KEY_INPUT_SPACE))
	{
		stageState = 1;
	}
}

void Stage::PlayUpdate()
{
	Player_VS_Enemy();

	Enemy_VS_Bullet();

	DeletBullet();

	DeletEnemy();

	UpdateAllObjects();

	SpawnEnemy();

	if (Input::IsKeyDown(KEY_INPUT_SPACE))
	{
		ShootBullet();
	}

	DeadUpdate();

}

void Stage::GameOverUpdate()
{
	static bool canPush = false;

	if (!Input::IsKeepKeyDown(KEY_INPUT_SPACE))
	{
		canPush = true;
	}

	if (canPush && Input::IsKeepKeyDown(KEY_INPUT_SPACE))
	{
		Initialize();
		canPush = false;
	}
}

void Stage::TitleDraw()
{
	int fsize = GetFontSize();
	SetFontSize(80);
	SetFontThickness(10);
	DrawString(WIN_WIDTH / 2 - 176, WIN_HEIGHT / 2 - 84, "ASTEROIDS", GetColor(255, 0, 0));
	DrawString(WIN_WIDTH / 2 - 180, WIN_HEIGHT / 2 - 80, "ASTEROIDS", GetColor(255, 255, 255));
	SetFontSize(fsize);
}

void Stage::PlayDraw()
{
	DrawAllObjects();
	int fsize = GetFontSize();
	SetFontSize(fsize * 2);
	DrawFormatString(10, 10, GetColor(255, 255, 255), "SCORE:%020lld", gameScore_);
	SetFontSize(fsize);
}

void Stage::GameOverDraw()
{
	int fsize = GetFontSize();
	SetFontSize(80);
	SetFontThickness(10);
	DrawString(WIN_WIDTH / 2 - 176, WIN_HEIGHT / 2 - 84, "GAMEOVER", GetColor(0, 0, 0));
	DrawString(WIN_WIDTH / 2 - 180, WIN_HEIGHT / 2 - 80, "GAMEOVER", GetColor(255, 0, 0));
	SetFontSize(fsize);
}

void Stage::Update()
{
	if (stageState == 0)
	{
		TitleUpdate();
	}
	else if (stageState == 1)
	{
		PlayUpdate();

	}
	else if (stageState == 2)
	{
		GameOverUpdate();
	}

}

void Stage::Draw()
{
	if (stageState == 0)
	{
		TitleDraw();
	}
	else if (stageState == 1)
	{
		PlayDraw();
	}
	else if (stageState == 2)
	{
		GameOverDraw();
	}
}

void Stage::Release()
{
	//if (player != nullptr)
	//	delete player;
	//if (enemy != nullptr)
	//	delete enemy;
}

void Stage::DeletBullet()
{
	//賞味期限切れの弾を消す
	//まず、箱の中身を確認して、死んでる弾があったらdeleteする
	// （箱は残るので、nullptrを入れておく）
	for (auto& itr : objects)
	{
		if (itr->GetType() == OBJ_TYPE::BULLET)
		{
			//base->継承クラスの時は、ちゃんと継承クラスのポインタに変換してあげないと、継承クラスのメンバ関数は呼び出せない
			//継承クラス→bassクラスの変換は暗黙的に行われる
			Bullet* b = (Bullet*)(itr);
			if (b->IsDead())
			{
				delete b;
				itr = nullptr;
			}
		}
	}
	//次に、箱の中身を確認して、nullptrがあったら箱から消す
	// （箱自体を詰める）
	for (auto it = objects.begin(); it != objects.end();)
	{
		if (*it == nullptr)
		{
			it = objects.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void Stage::DeletEnemy()
{
	for (auto& itr : objects)
	{
		if (itr->GetType() == OBJ_TYPE::ENEMY)
		{
			//base->継承クラスの時は、ちゃんと継承クラスのポインタに変換してあげないと、継承クラスのメンバ関数は呼び出せない
			//継承クラス→bassクラスの変換は暗黙的に行われる
			Enemy* b = (Enemy*)(itr);
			if (!b->IsAlive())
			{
				delete b;
				itr = nullptr;
			}
		}
	}
	//次に、箱の中身を確認して、nullptrがあったら箱から消す
	// （箱自体を詰める）
	for (auto it = objects.begin(); it != objects.end();)
	{
		if (*it == nullptr)
		{
			it = objects.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void Stage::SpawnEnemy()
{
	std::vector<Enemy*> spawnEnemies;

	for (auto& obj : objects)
	{
		if (obj->GetType() == OBJ_TYPE::ENEMY)
		{
			Enemy* e = (Enemy*)obj;

			if (e->IsAlive() && e->ShouldSpawn())
			{
				spawnEnemies.push_back(e);
			}
		}
	}

	//生成
	for (auto& parent : spawnEnemies)
	{
		Enemy* SMALL = new Enemy(Enemy::Size::SMALL, 8);

		SMALL->SetPos(parent->GetPos());

		SMALL->SetVel({
			(float)(GetRand(200) - 100),
			(float)(GetRand(200) - 100)
			});

		AddObject(SMALL);
	}
}

void Stage::ShootBullet()
{
	Player* player = nullptr;
	for (auto& obj : objects)
	{
		if (obj->GetType() == OBJ_TYPE::PLAYER)
		{
			player = (Player*)obj;
			break;
		}
	}
	Vector2D pos = player->GetPos();
	Vector2D v = Math2D::Mul(player->GetDirVec(), 300.0f);
	unsigned int bcol = GetColor(255, 255, 255);
	float r = 2;
	float life = 2.0f;

	Bullet* b = new Bullet(pos, v, bcol, r, life);
	AddObject(b);
}

void Stage::Enemy_VS_Bullet()
{
	std::vector<Enemy*> aliveEnemies;
	std::vector<Bullet*>aliveBullets;

	aliveEnemies.clear();//念のため、毎フレームaliveEnemiesを空にする
	aliveBullets.clear();

	//for (int i = 0; i < objects.size(); i++)
	for (auto& obj : objects)
	{
		if (obj->GetType() == OBJ_TYPE::ENEMY)
		{
			Enemy* e = (Enemy*)obj;
			if (e->IsAlive()) {
				aliveEnemies.push_back(e);
			}
		}
		else if (obj->GetType() == OBJ_TYPE::BULLET)
		{
			Bullet* b = (Bullet*)obj;
			if (!b->IsDead()) {
				aliveBullets.push_back(b);
			}
		}
	}
	//for(int i=0; i<aliveBullets.size(); i++)
	for (auto& bullet : aliveBullets)
	{
		for (auto& enemy : aliveEnemies)
		{
			float dist = Math2D::Length(Math2D::Sub(bullet->GetPos(), enemy->GetPos()));
			if (dist < enemy->GetCollisionRadius())
			{
				//当たった
				enemy->Dead();//敵を消す(生存フラグをfalseに）
				int sc[3] = { 20, 50, 100 };
				gameScore_ += sc[enemy->GetSize()];
				//			//TODO:
				//分裂の処理をここでやりたい
				//大か中か小かを判定して
				//大なら中を2~4つ、中なら小を2~4つ、小なら消してエフェクト生成
				if (enemy->GetSize() != Enemy::Size::SMALL)
				{
					int num = GetRand(3) + 2; //2~4のランダムな数
					//大きさによって、分裂数変えると素敵です。
					for (int i = 0;i < num;i++)
					{
						Enemy* e = nullptr;
						if (enemy->GetSize() == Enemy::Size::LARGE)
						{
							e = new Enemy(Enemy::Size::MEDIUM, 8);
						}
						else
						{
							e = new Enemy(Enemy::Size::SMALL, 8);
						}
						e->SetPos(enemy->GetPos());
						//速さの設定は必要
						e->SetVel({ (float)(GetRand(200) - 100), (float)(GetRand(200) - 100) });
						AddObject(e);
					}
				}
				else
				{
					ExplosionEffect* effect = new ExplosionEffect(enemy->GetPos());
					effect->SetCharaColor(GetColor(GetRand(255), GetRand(255), GetRand(255)));
					AddObject(effect);
				}
				bullet->Dead();
			}
		}

	}
}

void Stage::Player_VS_Enemy()
{
	std::vector<Enemy*> aliveEnemies;
	aliveEnemies.clear();
	Player* player = nullptr;

	for (auto& obj : objects)
	{
		if (obj->GetType() == OBJ_TYPE::PLAYER)
		{
			player = (Player*)obj;
		}
		if (obj->GetType() == OBJ_TYPE::ENEMY)
		{
			Enemy* e = (Enemy*)obj;
			if (e->IsAlive()) {
				aliveEnemies.push_back(e);
			}
		}
	}

	if (player == nullptr || player->IsAlive() == false)
		return;

	for (auto& enemy : aliveEnemies)
	{
		float dist = Math2D::Length(Math2D::Sub(player->GetPos(), enemy->GetPos()));
		float collisionDist = player->GetCollisionRadius() + enemy->GetCollisionRadius();
		if (dist < collisionDist)
		{
			player->Dead();
			ExplosionEffect* effect = new ExplosionEffect(player->GetPos(), 50);
			effect->SetCharaColor(GetColor(255, 0, 0));
			AddObject(effect);
			break;
		}
	}
}

void Stage::DeadUpdate()
{
	Player* player = nullptr;
	bool effectAlive = false;

	for (auto& obj : objects)
	{
		if (obj->GetType() == OBJ_TYPE::PLAYER)
		{
			player = (Player*)obj;
		}
		else if (obj->GetType() == OBJ_TYPE::EFFECT)
		{
			ExplosionEffect* e = (ExplosionEffect*)obj;
			if (!e->IsFinished())   // ← ここ重要
			{
				effectAlive = true;
			}
		}
	}

	if (player == nullptr)
		return;

	if (!player->IsAlive() && !effectAlive)
	{
		stageState = 2;
	}
}
