#include "DxLib.h"
#include "globals.h"
#include "input.h"
#include "Stage.h"

namespace
{
	const int BGCOLOR[3] = {0, 0, 51}; // 背景色{ 255, 250, 205 }; // 背景色
	int crrTime;
	int prevTime;

	Stage* stage = nullptr;
}


float gDeltaTime = 0.0f; // フレーム間の時間差

void DxInit()
{
	ChangeWindowMode(true);
	SetWindowSizeChangeEnableFlag(false, false);
	SetMainWindowText("TITLE");
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);
	SetWindowSizeExtendRate(1.0);
	SetBackgroundColor(BGCOLOR[0],BGCOLOR[1],BGCOLOR[2]);

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		DxLib_End();
	}

	SetDrawScreen(DX_SCREEN_BACK);
}

void Initialize()
{
	// 独自の初期化処理をここに記述
	stage = new Stage();
	stage->Initialize();
}

void Update()
{
	
	// 独自の更新処理をここに記述
	stage->Update();
}
void Draw()
{
	// 独自の描画処理をここに記述
	stage->Draw();
}
void Release()
{
	// 独自の終了処理をここに記述
	stage->Release();
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	DxInit();
	crrTime = GetNowCount();
	prevTime = GetNowCount();

	Initialize(); // 独自の初期化処理

	while (true)
	{
		ClearDrawScreen();
		Input::KeyStateUpdate(); // キー入力の状態を更新

		crrTime = GetNowCount(); // 現在の時間を取得
		// 前回の時間との差分を計算
		float deltaTime = (crrTime - prevTime) / 1000.0f; // 秒単位に変換
		gDeltaTime = deltaTime; // グローバル変数に保存
		
		//ここにやりたい処理を書く
		Update(); // 独自の更新処理
		Draw(); // 独自の描画処理

		ScreenFlip();
		WaitTimer(16);

		prevTime = crrTime; // 現在の時間を前回の時間として保存

		if (ProcessMessage() == -1)
			break;
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
			break;
	}

	Release(); // 独自の終了処理
	DxLib_End();
	return 0;
}