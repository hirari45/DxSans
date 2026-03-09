#pragma once
#include <vector>

class Player;
class Stage
{
public:
	Stage();
	~Stage();
	void Initialize();
	void TitleUpdate();
	void PlayUpdate();
	void GameOverUpdate();

	void TitleDraw();
	void PlayDraw();
	void GameOverDraw();

	void Update();
	void Draw();
	void Release();
	void DeletBullet();
	void DeletEnemy();
	void SpawnEnemy();
	void ShootBullet();
private:
	Player* player_;
	long long gameScore_;

	void Enemy_VS_Bullet();
	void Player_VS_Enemy();
	void DeadUpdate();
};

