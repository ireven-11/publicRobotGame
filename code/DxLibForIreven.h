#pragma once
#include<math.h>
#include"DxLib.h"
#include<cassert>
#include<windows.h>
#include<unordered_map>
#include<string>
#include<algorithm>
#include"ObjectAxis.h"

#define winmainSetting	_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int	//winmainの引数
#define gameRoopSetting ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0	//ゲームループの引数(ゲームループの最初にClearDrawScreen()を最後にScreenFlip()を呼ぶ必要がある)

const VECTOR zero_vector = VGet(0.0f, 0.0f, 0.0f);

//それぞれの軸の向き
const static VECTOR axis_x = VGet(1.0f, 0.0f, 0.0f);
const static VECTOR axis_y = VGet(0.0f, 1.0f, 0.0f);
const static VECTOR axis_z = VGet(0.0f, 0.0f, 1.0f);

//初期のスクリーンの大きさ
const static float  init_screen_width			= 1920;
const static float  init_screen_height			= 1080;
const static VECTOR	init_screen_center_position = VGet(init_screen_width * 0.5f, init_screen_height * 0.5f, 0.0f);

//VECTORの一部演算子オーバーロード
inline bool operator==(const VECTOR vector1, const VECTOR vector2)
{
	const bool equalX = vector1.x == vector2.x;
	const bool equalY = vector1.y == vector2.y;
	const bool equalZ = vector1.z == vector2.z;

	return equalX && equalY && equalZ;
}

/// <summary>
/// dxlib初期化
/// </summary>
/// <param name="screenWidht">スクリーン幅</param>
/// <param name="screenHeight">スクリーン高さ</param>
inline void InitDxLib(const unsigned int screenWidht = init_screen_width, const unsigned int screenHeight = init_screen_height)
{
	ChangeWindowMode(false);					//ウィンドウモードにする
	SetWindowStyleMode(7);						//最大化ボタンが存在するウインドウモードに変更

	if (DxLib_Init() == -1)return;

	// サイズ変更を可能にする
	SetWindowSizeChangeEnableFlag(true, false);

	//pc自体の画面解像度をdxlibのスクリーンサイズに合うように変更
	DEVMODE pcScreenInfo;	//DEVMODEはpcのスクリーン情報を格納する構造体（windowsAPI）
	pcScreenInfo.dmPelsWidth	= screenWidht;
	pcScreenInfo.dmPelsHeight	= screenHeight;
	ChangeDisplaySettings(&pcScreenInfo, CDS_FULLSCREEN);	//第二引数を「CDS_FULLSCREEN」にするとアプリ終了時に元の解像度に戻る

	//ウィンドウのサイズとカラーモードを決める
	SetGraphMode(screenWidht, screenHeight, 32);

	SetMainWindowText("gamename");              //ウィンドウ（白いところ）にゲーム名を書く
	SetDrawScreen(DX_SCREEN_BACK);		        //背景をセットする
}

/// <summary>
/// 背景透過動画再生関数
/// </summary>
/// <param name="movieHandle">動画ハンドル</param>
/// <param name="screenHandle">スクリーンハンドル※MakeScreen関数でハンドルを作ってその時に第三引数をTRUEにする必要がある</param>
/// <param name="movieBackColorType">動画背景色のタイプ(0:黒, 1:白を指定する)</param>
/// <param name="isLoop">動画をループするか</param>
/// <param name="movieWidht">動画幅</param>
/// <param name="movieHeght">動画高さ</param>
/// <param name="moviePosition">動画座標</param>
/// <param name="originScreenHandle">元々のスクリーンハンドル</param>
inline bool PlayTransparentMovie(const int movieHandle, const int screenHandle, const unsigned char movieBackColorType = 0, const bool isLoop = true,
	const int movieWidht = init_screen_width, const int movieHeight = init_screen_height, const VECTOR moviePosition = VGet(0.0f, 0.0f, 0.0f), const int originScreenHandle = DX_SCREEN_BACK)
{
	//スクリーンハンドルに動画を描画する
	SetDrawScreen(screenHandle);

	//ループ再生するかどうか
	if (isLoop)
	{
		PlayMovieToGraph(movieHandle, DX_PLAYTYPE_LOOP);
	}
	else
	{
		PlayMovieToGraph(movieHandle);
	}
	DrawExtendGraphF(moviePosition.x, moviePosition.y, moviePosition.x + movieWidht, moviePosition.y + movieHeight, movieHandle, TRUE);

	//元のスクリーンハンドルに戻す
	SetDrawScreen(originScreenHandle);

	//スクリーンハンドルを画像として透過してから描画
	//背景の色によって透過する色を変える
	if (movieBackColorType == 0)//黒
	{
		GraphFilter(screenHandle, DX_GRAPH_FILTER_BRIGHT_CLIP, DX_CMP_LESS, 10, TRUE, GetColor(0, 255, 0), 0);
	}
	else//白
	{
		GraphFilter(screenHandle, DX_GRAPH_FILTER_BRIGHT_CLIP, DX_CMP_GREATER, 245, TRUE, GetColor(0, 255, 0), 0);
	}
	DrawExtendGraphF(moviePosition.x, moviePosition.y, moviePosition.x + movieWidht, moviePosition.y + movieHeight, screenHandle, TRUE);

	//動画が再生してるかを返す
	if (GetMovieStateToGraph(movieHandle))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/// <summary>
/// 距離計算関数
/// </summary>
/// <param name="movieHandle">座標1</param>
/// <param name="screenHandle">座標2</param>
template<typename T>
inline T CalculateDistance(const VECTOR position1, const VECTOR position2 = VGet(0.0f, 0.0f, 0.0f))
{
	//2点間の距離を計算する
	VECTOR tempVector	= VGet(position1.x, position1.y, position1.z);
	tempVector			= VSub(tempVector, position2);
	tempVector			= VGet(tempVector.x * tempVector.x, tempVector.y * tempVector.y, tempVector.z * tempVector.z);
	T value				= tempVector.x + tempVector.y + tempVector.z;
	return static_cast<T>(sqrt(value));
}

/// <summary>
/// 画像アニメーション描画関数
/// </summary>
/// <param name="position">座標</param>
/// <param name="graphHandle">画像ハンドル</param>
/// <param name="graphWidth">画像幅</param>
/// <param name="graphHeight">画像高さ</param>
/// <param name="tilSwitchTime">アニメーションが切り替わるまでの時間</param>
/// <param name="finishGraphNumber">最後の画像がアニメーションの何番目かを表す数値</param>
/// <param name="startGraphNumber">最初の画像がアニメーションの何番目かを表す数値</param>
inline void DrawAnimationGraph(const VECTOR position, const int graphHandle[], const int graphWidth, const int graphHeight,
	const unsigned int tilSwitchTime, const unsigned int finishGraphNumber, const unsigned int startGraphNumber = 0)
{
	//待ちカウントとアニメーションカウントを定義
	static unsigned int waitCount;
	static unsigned int animationCount = startGraphNumber;

	//待ちカウントを進める
	++waitCount;

	//アニメーションする画像が切り替わった時にアニメーションを最初からにする
	static int checkChangeAnim = startGraphNumber;
	if (checkChangeAnim != startGraphNumber)
	{
		waitCount		= 0;
		animationCount	= startGraphNumber;
		checkChangeAnim = startGraphNumber;
	}

	if (waitCount >= tilSwitchTime)
	{
		//アニメーションが切り替わるまでの時間を超えたらアニメーションを進める
		++animationCount;
		waitCount = 0;

		//アニメーションが最後まで行ったら最初に戻す
		if (animationCount > finishGraphNumber)
		{
			animationCount = startGraphNumber;
		}
	}
	DrawExtendGraphF(position.x, position.y, position.x + graphWidth, position.y + graphHeight, graphHandle[animationCount], TRUE);
}

/// <summary>
/// 文字点滅描画関数(フォント指定可能)
/// </summary>
/// <param name="position">描画座標</param>
/// <param name="text">描画したいテキスト</param>
/// <param name="color">文字の色</param>
/// <param name="fontHandle">フォントハンドル</param>
/// <param name="brinkSpeed">点滅スピード(デフォルトは2、0を入れると点滅しない)</param>
inline void DrawBrinkStringToHandle(const VECTOR position, const char* text, const int color, const int fontHandle = 0, const unsigned short brinkSpeed = 2)
{
	//点滅スピードが0だったら点滅しない
	if (brinkSpeed == 0)
	{
		DrawStringToHandle(static_cast<int>(position.x), static_cast<int>(position.y), text, color, fontHandle);
	}
	else
	{
		//点滅カウント
		static int brinkCount;
		brinkCount += 1 * brinkSpeed;
		if (brinkCount > 100)
		{
			brinkCount = 0;
		}

		//点滅カウントが一定値以下のときだけ描画する
		if (brinkCount > 100 / brinkSpeed)
		{
			DrawStringToHandle(static_cast<int>(position.x), static_cast<int>(position.y), text, color, fontHandle);
		}
	}
}

/// <summary>
/// 画像点滅描画関数(拡大縮小可能)
/// </summary>
/// <param name="position">描画座標</param>
/// <param name="widht">画像幅</param>
/// <param name="height">画像高さ</param>
/// <param name="graphHandle">画像反動</param>
/// <param name="brinkSpeed">点滅スピード(デフォルトは2、0を入れると点滅しない)</param>
/// <param name="isTrans">画像の透過処理を許すか</param>
inline void DrawExtendBrinkGraph(const VECTOR position, const float widht, const float height, const int graphHandle,
	const unsigned short brinkSpeed = 2, const bool isTrans = true)
{
	//点滅スピードが0だったら点滅しない
	if (brinkSpeed == 0)
	{
		DrawExtendGraphF(position.x, position.y, position.x + widht, position.y + height, graphHandle, isTrans);
	}
	else
	{
		//点滅カウント
		static int brinkCount;
		brinkCount += 1 * brinkSpeed;
		if (brinkCount > 100)
		{
			brinkCount = 0;
		}

		//点滅カウントが一定値以下のときだけ描画する
		if (brinkCount > 100 / brinkSpeed)
		{
			DrawExtendGraphF(position.x, position.y, position.x + widht, position.y + height, graphHandle, isTrans);
		}
	}
}

/// <summary>
/// ベクトルの値を行列にセット
/// </summary>
/// <param name="matrix">値をセットする行列</param>
/// <param name="vector">セットしたいベクトル</param>
inline void SetMatrixFromVector(MATRIX& matrix, const VECTOR vector)
{
	matrix.m[3][0] = vector.x;
	matrix.m[3][1] = vector.y;
	matrix.m[3][2] = vector.z;
}

/// <summary>
/// モデルボーン指定した値の回転行列取得関数
/// </summary>
/// <param name="MHandle">モデルハンドル</param>
/// <param name="FrameIndex">回転させたいボーン</param>
/// <param name="Xaxis">x軸の回転させる値</param>
/// <param name="Yaxis">y軸の回転させる値</param>
/// <param name="Zaxis">z軸の回転させる値</param>
inline MATRIX MV1GetFrameRotateMatrix(const int MHandle, const int FrameIndex, const double Xaxis, const double Yaxis, const double Zaxis)
{
	auto frame_matrix		= MV1GetFrameLocalMatrix(MHandle, FrameIndex);
	auto result_matrix		= MGetIdent();
	const auto frame_pos	= MGetTranslateElem(frame_matrix);

	CreateRotationZXYMatrix(&result_matrix, static_cast<float>(Xaxis), static_cast<float>(Yaxis), static_cast<float>(Zaxis));
	SetMatrixFromVector(result_matrix, frame_pos);

	return result_matrix;
}

/// <summary>
/// 2D用の外積を求める
/// </summary>
/// <param name="a">外積の計算に使う座標1</param>
/// <param name="b">外積の計算に使う座標2</param>
inline float Cross2D(const VECTOR a, const VECTOR b)
{
	return a.x * b.z - a.z * b.x;
}

/// <summary>
/// 点が三角形ABCの内部にあるか（辺上も含む）
/// </summary>
/// <param name="A">三角形の座標１</param>
/// <param name="B">三角形の座標２</param>
/// <param name="C">三角形の座標３</param>
/// <param name="P">判定したい座標</param>
inline bool HitTriangleAndPixel(const VECTOR A, const VECTOR B, const VECTOR C, const VECTOR P)
{
	VECTOR AB = { B.x - A.x, 0, B.z - A.z };
	VECTOR BC = { C.x - B.x, 0, C.z - B.z };
	VECTOR CA = { A.x - C.x, 0, A.z - C.z };

	VECTOR AP = { P.x - A.x, 0, P.z - A.z };
	VECTOR BP = { P.x - B.x, 0, P.z - B.z };
	VECTOR CP = { P.x - C.x, 0, P.z - C.z };

	float c1 = Cross2D(AB, AP);
	float c2 = Cross2D(BC, BP);
	float c3 = Cross2D(CA, CP);

	// すべて同じ符号なら内部（0は辺上）
	if ((c1 >= 0 && c2 >= 0 && c3 >= 0) || (c1 <= 0 && c2 <= 0 && c3 <= 0)) return true;

	return false;
}

/// <summary>
/// 円が三角形ABCの内部にあるか（辺上も含む）
/// </summary>
/// <param name="trianglePos1">三角形の座標１</param>
/// <param name="trianglePos2">三角形の座標２</param>
/// <param name="trianglePos3">三角形の座標３</param>
/// <param name="circlePos">判定したい円の座標</param>
/// <param name="radius">判定したい円の半径</param>
inline bool CheckHitCircleWithTriangle(const VECTOR trianglePos1, const VECTOR trianglePos2, const VECTOR trianglePos3, const VECTOR circlePos, const float radius)
{
	VECTOR edge;				//線分
	VECTOR edgeStartToCirclePos;//線分の始点から円の座標まで伸びるベクトル
	VECTOR edgeEndToCirclePos;	//線分の終点から円の座標まで伸びるベクトル
	VECTOR crossProduct;		//外積
	float shortestDistance;		//線分との最短距離

	for (auto i = 0; i < 3; i++)
	{
		switch (i)
		{
		case 0:
			edge					= VSub(trianglePos2, trianglePos1);
			edgeStartToCirclePos	= VSub(circlePos, trianglePos1);
			edgeEndToCirclePos		= VSub(circlePos, trianglePos2);
			crossProduct			= VCross(edge, edgeStartToCirclePos);
			shortestDistance		= CalculateDistance<float>(crossProduct) / CalculateDistance<float>(edge);
			break;
		case 1:
			edge					= VSub(trianglePos3, trianglePos2);
			edgeStartToCirclePos	= VSub(circlePos, trianglePos2);
			edgeEndToCirclePos		= VSub(circlePos, trianglePos3);
			crossProduct			= VCross(edge, edgeStartToCirclePos);
			shortestDistance		= CalculateDistance<float>(crossProduct) / CalculateDistance<float>(edge);
			break;
		case 2:
			edge					= VSub(trianglePos1, trianglePos3);
			edgeStartToCirclePos	= VSub(circlePos, trianglePos3);
			edgeEndToCirclePos		= VSub(circlePos, trianglePos1);
			crossProduct			= VCross(edge, edgeStartToCirclePos);
			shortestDistance		= CalculateDistance<float>(crossProduct) / CalculateDistance<float>(edge);
			break;
		}

		//最短距離が半径より小さかったら当たってる可能性がある
		if (shortestDistance <= radius)
		{
			//線分の外に円の中心があるときは角度が両方鈍角だったら当たってる
			if (VDot(edge, edgeStartToCirclePos) * VDot(edge, edgeEndToCirclePos) <= 0) return true;

			//半径が円へのベクトルの距離より大きかったら当たってる
			if (radius > CalculateDistance<float>(edgeStartToCirclePos) || radius > CalculateDistance<float>(edgeEndToCirclePos)) return true;
		}

		//三角形の中に円の中心点があるか
		if (HitTriangleAndPixel(trianglePos1, trianglePos2, trianglePos3, circlePos)) return true;
	}

	return false;
}

/// <summary>
/// 3d上に六角形を平面描画する関数
/// </summary>
/// <param name="standardPosition">六角形の基準となる座標</param>
/// <param name="sideX">六角形のｘの長さ</param>
/// <param name="sideZ">六角形のｚの長さ</param>
/// <param name="sideZ2">六角形の何でいえばいいんだろ、こう、上と下の三角形の部分あるじゃん。そこまでのｚの長さっていえばいいのか？</param>
/// <param name="color"></param>
/// <param name="fill"></param>
inline void DrawHexagon3D(const VECTOR standardPosition, const float sideX, const float sideZ, const float sideZ2, const int color, const bool fill)
{
	const float width_adjust	= 2.0f;
	const float height_adjust	= 1.5f;

	DrawTriangle3D(VGet(standardPosition.x - sideX / width_adjust, 0.0f, standardPosition.z + sideZ2 / height_adjust),
		VGet(standardPosition.x + sideX / width_adjust, 0.0f, standardPosition.z + sideZ2 / height_adjust),
		VGet(standardPosition.x, 0.0f, standardPosition.z + sideZ / height_adjust), color, fill);
	DrawTriangle3D(VGet(standardPosition.x - sideX / width_adjust, 0.0f, standardPosition.z + sideZ2 / height_adjust),
		VGet(standardPosition.x + sideX / width_adjust, 0.0f, standardPosition.z + sideZ2 / height_adjust),
		VGet(standardPosition.x + sideX / width_adjust, 0.0f, standardPosition.z - sideZ2 / height_adjust), color, fill);
	DrawTriangle3D(VGet(standardPosition.x - sideX / width_adjust, 0.0f, standardPosition.z + sideZ2 / height_adjust),
		VGet(standardPosition.x - sideX / width_adjust, 0.0f, standardPosition.z - sideZ2 / height_adjust),
		VGet(standardPosition.x + sideX / width_adjust, 0.0f, standardPosition.z - sideZ2 / height_adjust), color, fill);
	DrawTriangle3D(VGet(standardPosition.x - sideX / width_adjust, 0.0f, standardPosition.z - sideZ2 / height_adjust),
		VGet(standardPosition.x + sideX / width_adjust, 0.0f, standardPosition.z - sideZ2 / height_adjust),
		VGet(standardPosition.x, 0.0f, standardPosition.z - sideZ / height_adjust), color, fill);
}

/// <summary>
/// ベクトルをY軸回転する
/// </summary>
/// <param name="vector">回転させるベクトル</param>
/// <param name="rotationAngle">回転角度</param>
/// <returns></returns>
inline VECTOR RotationVectorY(const VECTOR vector, const double rotationAngle)
{
	//角度法だとエラーが出るようにする
	//assert((-DX_PI_F < rotationAngle && rotationAngle < DX_PI_F) && "回転角度が弧度法（ラジアン）になってないです！これだとベクトルがよくわからんぐらい回転します！");

	VECTOR tempVector = vector;

	//回転
	MATRIX rotationMatrix	= MGetRotY(static_cast<float>(rotationAngle));
	tempVector				= VTransform(vector, rotationMatrix);

	return tempVector;
}

/// <summary>
/// 一定速度で上下移動させる
/// </summary>
/// <param name="positionY">基準Y座標</param>
/// <param name="speed">移動速度(px/frame)</param>
/// <param name="amplitude">振れ幅</param>
/// <returns>移動後のY座標</returns>
inline float UpDownPositionY(float positionY, const float speed = 0.5f, const float amplitude = 10.0f)
{
	static float offset = 0.0f;
	static bool isUp = true;

	if (isUp)
	{
		offset += speed;

		if (offset >= amplitude)
		{
			offset = amplitude;
			isUp = false;
		}
	}
	else
	{
		offset -= speed;

		if (offset <= -amplitude)
		{
			offset = -amplitude;
			isUp = true;
		}
	}

	return positionY + offset;
}

/// <summary>
/// モデルのすべてのボーンを保存する
/// </summary>
/// <param name="modelHandle"></param>
/// <param name="modelBoneIndex"></param>
inline void MV1SaveModelNameAndIndex(const int modelHandle, std::unordered_map<std::string, int>& modelBoneIndex)
{
	const int frameNum = MV1GetFrameNum(modelHandle);
	for (int frameIndex = 0; frameIndex < frameNum; ++frameIndex)
	{
		const TCHAR* frameName = MV1GetFrameName(modelHandle, frameIndex);
		modelBoneIndex[frameName] = frameIndex;
	}
}

/// <summary>
/// 0～1の間の比率を計算して返す
/// </summary>
/// <typeparam name="t"></typeparam>
/// <param name="value"></param>
/// <param name="minimumValue"></param>
/// <param name="maxValue"></param>
/// <returns></returns>
template<typename t>
inline t NormalizeBetween(const t value, const t minimumValue, const t maxValue)
{
	return ((std::min)(value, maxValue) - minimumValue) / (maxValue - minimumValue);
}

/// <summary>
/// ベクトルの値を行列にセット
/// </summary>
/// <param name="matrix">値をセットする行列</param>
/// <param name="vector">セットしたいベクトル</param>
inline void SetVectorFromMatrix(const MATRIX& matrix, VECTOR& vector)
{
	vector.x = matrix.m[3][0];
	vector.y = matrix.m[3][1];
	vector.z = matrix.m[3][2];
}

/// <summary>
/// 3つのベクトルから回転行列を作る
/// </summary>
/// <param name="rightVector">右ベクトル</param>
/// <param name="upVector">上ベクトル</param>
/// <param name="forwardVector">前ベクトル</param>
/// <returns></returns>
inline MATRIX CreatRotationMatrixFromVector(const VECTOR rightVector, const VECTOR upVector, const VECTOR forwardVector)
{
	MATRIX rotationMatrix = MGetIdent();

	rotationMatrix.m[0][0] = rightVector.x;
	rotationMatrix.m[0][1] = rightVector.y;
	rotationMatrix.m[0][2] = rightVector.z;

	rotationMatrix.m[1][0] = upVector.x;
	rotationMatrix.m[1][1] = upVector.y;
	rotationMatrix.m[1][2] = upVector.z;

	rotationMatrix.m[2][0] = forwardVector.x;
	rotationMatrix.m[2][1] = forwardVector.y;
	rotationMatrix.m[2][2] = forwardVector.z;

	return rotationMatrix;
}

/// <summary>
/// 特定の方向を見る行列を求める
/// </summary>
/// <param name="fromVector">求めたい方向の座標1</param>
/// <param name="toVector">求めたい方向の座標2</param>
/// <param name="upVector">上方向</param>
/// <returns></returns>
inline MATRIX LookAt(const VECTOR fromVector, const VECTOR toVector, const VECTOR upVector = axis_y)
{
	const VECTOR fowardDirection	= VNorm(VSub(toVector, fromVector));				//前方向（向くべき方向）
	const VECTOR sideDirection		= VNorm(VCross(upVector, fowardDirection));			//前を基準とした左右方向
	const VECTOR correctUpDiretion	= VNorm(VCross(fowardDirection, sideDirection));	//前方向と右方向で正しい上方向を求める

	//求めた3つの方向と見たい方向の元となる座標から回転と座標をセットすることでlookat行列を求める
	const MATRIX lookAtMatrix = MMult(CreatRotationMatrixFromVector(sideDirection, correctUpDiretion, fowardDirection), MGetTranslate(fromVector));

	return lookAtMatrix;
}

/// <summary>
/// 回転行列の値を行列にセット
/// </summary>
/// <param name="matrix">セットされる行列</param>
/// <param name="rotataionMatrix">セットする回転行列</param>
inline void SetRotationToMatrix(MATRIX& matrix, const MATRIX& rotataionMatrix)
{
	/*const VECTOR scale = MGetSize(matrix);*/
	const VECTOR scale = VGet(1.0f, 1.0f, 1.0f);    //てすと

	//値をセットするだけだとスケールがぶっ壊れる可能性がある？ので後からスケールで割る
	matrix.m[0][0] = rotataionMatrix.m[0][0] / scale.x;
	matrix.m[1][0] = rotataionMatrix.m[1][0] / scale.y;
	matrix.m[2][0] = rotataionMatrix.m[2][0] / scale.z;

	matrix.m[0][1] = rotataionMatrix.m[0][1] / scale.x;
	matrix.m[1][1] = rotataionMatrix.m[1][1] / scale.y;
	matrix.m[2][1] = rotataionMatrix.m[2][1] / scale.z;

	matrix.m[0][2] = rotataionMatrix.m[0][2] / scale.x;
	matrix.m[1][2] = rotataionMatrix.m[1][2] / scale.y;
	matrix.m[2][2] = rotataionMatrix.m[2][2] / scale.z;
}

/// <summary>
/// 行列から回転成分だけ（座標やスケールを無視した値）を取得する
/// </summary>
/// <param name="matrix"></param>
inline MATRIX MGetRotationOnly(const MATRIX& matrix)
{
	/*const VECTOR matrixScale = MGetSize(matrix);*/
	const VECTOR matrixScale = VGet(1.0f, 1.0f, 1.0f);    //てすと

	MATRIX rotationOnly = MGetIdent();

	//値をセットするだけだとスケールがぶっ壊れる可能性がある？ので後からスケールで割る
	rotationOnly.m[0][0] = matrix.m[0][0] / matrixScale.x;
	rotationOnly.m[1][0] = matrix.m[1][0] / matrixScale.y;
	rotationOnly.m[2][0] = matrix.m[2][0] / matrixScale.z;

	rotationOnly.m[0][1] = matrix.m[0][1] / matrixScale.x;
	rotationOnly.m[1][1] = matrix.m[1][1] / matrixScale.y;
	rotationOnly.m[2][1] = matrix.m[2][1] / matrixScale.z;

	rotationOnly.m[0][2] = matrix.m[0][2] / matrixScale.x;
	rotationOnly.m[1][2] = matrix.m[1][2] / matrixScale.y;
	rotationOnly.m[2][2] = matrix.m[2][2] / matrixScale.z;

	return rotationOnly;
}

inline FLOAT4 QuaternionFromMatrix(const MATRIX& matrix)
{
	const MATRIX rotationMatrix = MGetRotationOnly(matrix);

	FLOAT4 quaternion;

	float trace = rotationMatrix.m[0][0] + rotationMatrix.m[1][1] + rotationMatrix.m[2][2];

	if (trace > 0.0f)
	{
		float scale = sqrtf(trace + 1.0f) * 2.0f;

		quaternion.w = 0.25f * scale;
		quaternion.x = -(rotationMatrix.m[2][1] - rotationMatrix.m[1][2]) / scale;
		quaternion.y = -(rotationMatrix.m[0][2] - rotationMatrix.m[2][0]) / scale;
		quaternion.z = -(rotationMatrix.m[1][0] - rotationMatrix.m[0][1]) / scale;
	}
	else if (rotationMatrix.m[0][0] > rotationMatrix.m[1][1] && rotationMatrix.m[0][0] > rotationMatrix.m[2][2])
	{
		float scale = sqrtf(1.0f + rotationMatrix.m[0][0] - rotationMatrix.m[1][1] - rotationMatrix.m[2][2]) * 2.0f;

		quaternion.w = -(rotationMatrix.m[2][1] - rotationMatrix.m[1][2]) / scale;
		quaternion.x = 0.25f * scale;
		quaternion.y = (rotationMatrix.m[0][1] + rotationMatrix.m[1][0]) / scale;
		quaternion.z = (rotationMatrix.m[0][2] + rotationMatrix.m[2][0]) / scale;
	}
	else if (rotationMatrix.m[1][1] > rotationMatrix.m[2][2])
	{
		float scale = sqrtf(1.0f + rotationMatrix.m[1][1] - rotationMatrix.m[0][0] - rotationMatrix.m[2][2]) * 2.0f;

		quaternion.w = -(rotationMatrix.m[0][2] - rotationMatrix.m[2][0]) / scale;
		quaternion.x = (rotationMatrix.m[0][1] + rotationMatrix.m[1][0]) / scale;
		quaternion.y = 0.25f * scale;
		quaternion.z = (rotationMatrix.m[1][2] + rotationMatrix.m[2][1]) / scale;
	}
	else
	{
		float scale = sqrtf(1.0f + rotationMatrix.m[2][2] - rotationMatrix.m[0][0] - rotationMatrix.m[1][1]) * 2.0f;

		quaternion.w = -(rotationMatrix.m[1][0] - rotationMatrix.m[0][1]) / scale;
		quaternion.x = (rotationMatrix.m[0][2] + rotationMatrix.m[2][0]) / scale;
		quaternion.y = (rotationMatrix.m[1][2] + rotationMatrix.m[2][1]) / scale;
		quaternion.z = 0.25f * scale;
	}

	//正規化
	float length = sqrtf(static_cast<float>(std::pow(quaternion.x, 2) + std::pow(quaternion.y, 2) + std::pow(quaternion.z, 2) + std::pow(quaternion.w, 2)));
	if (length > 0.0f)
	{
		quaternion.x /= length;
		quaternion.y /= length;
		quaternion.z /= length;
		quaternion.w /= length;
	}

	return quaternion;
}

inline void QuaternionToAxisAngle(const FLOAT4 quaternion, VECTOR axis, float angle)
{
	FLOAT4 nQuaternion = quaternion;
	float length = sqrtf(static_cast<float>(std::pow(nQuaternion.x, 2) + std::pow(nQuaternion.y, 2) + std::pow(nQuaternion.z, 2)));

	if (length < 0.0001f)
	{
		axis = VGet(1, 0, 0);
		angle = 0;
		return;
	}

	axis = VGet(nQuaternion.x / length, nQuaternion.y / length, nQuaternion.z / length);
	angle = 2.0f * atan2f(length, nQuaternion.w);
}

/// <summary>
/// クォータニオンから行列を取得
/// </summary>
/// <param name="quaternion"></param>
/// <returns></returns>
inline MATRIX GetMatrixFromQuaternion(const FLOAT4 quaternion)
{
	MATRIX matrix = MGetIdent();

	float xx = static_cast<float>(std::pow(quaternion.x, 2));
	float yy = static_cast<float>(std::pow(quaternion.y, 2));
	float zz = static_cast<float>(std::pow(quaternion.z, 2));

	float xy = quaternion.x * quaternion.y;
	float xz = quaternion.x * quaternion.z;
	float yz = quaternion.y * quaternion.z;

	float wx = quaternion.w * quaternion.x;
	float wy = quaternion.w * quaternion.y;
	float wz = quaternion.w * quaternion.z;

	//三角関数の2倍角の公式を用いることで、行列の各要素にクォータニオンの各要素を用いた値に置き換える
	matrix.m[0][0] = 1.0f - 2.0f * (yy + zz);
	matrix.m[0][1] = 2.0f * (xy + wz);
	matrix.m[0][2] = 2.0f * (xz - wy);

	matrix.m[1][0] = 2.0f * (xy - wz);
	matrix.m[1][1] = 1.0f - 2.0f * (xx + zz);
	matrix.m[1][2] = 2.0f * (yz + wx);

	matrix.m[2][0] = 2.0f * (xz + wy);
	matrix.m[2][1] = 2.0f * (yz - wx);
	matrix.m[2][2] = 1.0f - 2.0f * (xx + yy);

	return matrix;
}

/// <summary>
/// 行列の回転を制限する
/// </summary>
/// <param name="matrix">制限する行列</param>
/// <param name="limitRotation">制限したい回転値（x,y,z）※値は-π/2～π/2にすること</param>
/// <returns></returns>
inline MATRIX LimitMatrixRotation(const MATRIX& matrix, const VECTOR limitRotation)
{
	//元の行列の回転を取得
	const MATRIX originMatrixRotation = MGetRotationOnly(matrix);

	//取得した行列の回転からx,y,zそれぞれどのくらい回転しているかの値を保存(ラジアン)
	float rotationX = atan2(originMatrixRotation.m[2][1], originMatrixRotation.m[2][2]);
	float rotationY = asin(-originMatrixRotation.m[2][0]);
	float rotationZ = atan2(originMatrixRotation.m[1][0], originMatrixRotation.m[0][0]);

	//それぞれの値を制限する
	rotationX = std::clamp(rotationX, -limitRotation.x, limitRotation.x);
	rotationY = std::clamp(rotationY, -limitRotation.y, limitRotation.y);
	rotationZ = std::clamp(rotationZ, -limitRotation.z, limitRotation.z);

	//制限した回転行列を作成
	MATRIX limitedRotationMatrix = MGetIdent();
	CreateRotationZYXMatrix(&limitedRotationMatrix, rotationX, rotationY, rotationZ);

	//最終的に元の行列に宣言した回転を入れる
	MATRIX resultMatrix = matrix;
	SetRotationToMatrix(resultMatrix, limitedRotationMatrix);

	return resultMatrix;
}

inline MATRIX LimitRotationQuaternion(const MATRIX currentMatrix, const MATRIX targetMatrix, float limitAngle)
{
	MATRIX currentRotation = MGetRotationOnly(currentMatrix);
	MATRIX targetRotation = MGetRotationOnly(targetMatrix);

	FLOAT4 currentQuaternion = QuaternionFromMatrix(currentRotation);
	FLOAT4 targetQuaternion = QuaternionFromMatrix(targetRotation);

	// 差分
	FLOAT4 delta = QTCross(targetQuaternion, QTConj(currentQuaternion));

	// 軸角
	VECTOR axis;
	float angle;
	QuaternionToAxisAngle(delta, axis, angle);

	// 制限
	angle = std::clamp(angle, -limitAngle, limitAngle);

	// 再構築
	FLOAT4 limitedDelta = QTRot(axis, angle);
	FLOAT4 resultQuaternion = QTCross(limitedDelta, currentQuaternion);

	return GetMatrixFromQuaternion(resultQuaternion);
}

inline void OneBoneIK(const int modelHandle, const int ikBoneIndex, const VECTOR ikTargetPosition, const VECTOR adjustAxisUp, const VECTOR adjustAxisAim, VECTOR limitRotation)
{
	//回転させるボーンの行列、座標を取得
	const MATRIX boneWorldMatrix = MV1GetFrameLocalWorldMatrix(modelHandle, ikBoneIndex);
	VECTOR boneWorldPosition;
	SetVectorFromMatrix(boneWorldMatrix, boneWorldPosition);

	//対象の方向を見る
	const MATRIX lookAtMatrix = LookAt(boneWorldPosition, ikTargetPosition);
	MATRIX lookAtRotationMatrix = MGetRotationOnly(lookAtMatrix);

	//軸の補正（モデルによって補正の値が違うのでモデルビューアーなどでボーンの軸を確認必須）
	const MATRIX adjustMatrix = CreatRotationMatrixFromVector(VCross(adjustAxisUp, adjustAxisAim), adjustAxisUp, adjustAxisAim);
	lookAtRotationMatrix = MMult(MInverse(adjustMatrix), lookAtRotationMatrix);

	//回したいボーンの親ボーンを使いローカル行列に変換
	const MATRIX parentWorldMatrix = MV1GetFrameLocalWorldMatrix(modelHandle, MV1GetFrameParent(modelHandle, ikBoneIndex));
	const MATRIX parentWorldRotationMatrix = MGetRotationOnly(parentWorldMatrix);
	MATRIX boneLocalRotationMatrix = MMult(lookAtRotationMatrix, MInverse(parentWorldRotationMatrix));

	//回転に制限をかける
	//boneLocalRotationMatrix = LimitMatrixRotation(boneLocalRotationMatrix, limitRotation);
   /* MATRIX currentLocalMatrix   = MV1GetFrameLocalMatrix(modelHandle, ikBoneIndex);
	MATRIX currentRot           = MGetRotationOnly(currentLocalMatrix);
	boneLocalRotationMatrix     = LimitRotationQuaternion(currentRot, boneLocalRotationMatrix, limitRotation.x);*/

	//ローカル行列に回転行列を設定
	MATRIX boneLocalMatrix = MV1GetFrameLocalMatrix(modelHandle, ikBoneIndex);
	SetRotationToMatrix(boneLocalMatrix, boneLocalRotationMatrix);

	//ローカル行列として適用し、ボーンをターゲットに向ける
	MV1SetFrameUserLocalMatrix(modelHandle, ikBoneIndex, boneLocalMatrix);
}

/// <summary>
/// グリッドを描画
/// </summary>
/// <param name="groundSize">大きさ</param>
/// <param name="divideNum">分割数</param>
/// <param name="gridColor">色</param>
inline void DrawGrid(const float groundSize, const unsigned int divideNum, const int gridColor = GetColor(255, 0, 0))
{
	VECTOR start, end;
	int lineColor = gridColor;
	start = end = VGet(0, 0, 0);

	float halfWidth = groundSize * 0.5f;					//グリッド線を引く範囲の半分の幅
	float addWidth	= groundSize / (float)divideNum;		//グリッド線を引く間隔

	//縦線を引く 
	start.z = -halfWidth;
	end.z	= +halfWidth;
	for (int ix = 0; ix < divideNum + 1; ix++)
	{
		start.x = end.x = -halfWidth + ix * addWidth;
		DrawLine3D(start, end, lineColor);
	}

	//横線
	start.x = -halfWidth;
	end.x	= +halfWidth;
	for (int iz = 0; iz < divideNum + 1; iz++)
	{
		start.z = end.z = -halfWidth + iz * addWidth;
		DrawLine3D(start, end, lineColor);
	}
}

/// <summary>
/// はみ出した部分を結果ベクトルに加算する
/// </summary>
/// <param name="resultVector">結果ベクトル</param>
/// <param name="pointPosition">点の座標</param>
/// <param name="boxCenterPosition">OBBの中心座標</param>
/// <param name="length">判定している軸の長さ</param>
/// <param name="axis">判定している軸の方向</param>
inline void addOverVector(VECTOR& resultVector, const VECTOR pointPosition, const VECTOR boxCenterPosition, const float length, const VECTOR axis)
{
	if (length <= 0) return;

	float scale = VDot(VSub(pointPosition, boxCenterPosition), axis) / length;

	//スケールの絶対値を出す
	scale = std::fabs(scale);

	//scaleの値から、はみ出した(1を超えてるとはみ出してる判定)部分があれば、そのベクトルを加算する
	if (scale <= 1) return;
	resultVector = VAdd(resultVector, VScale(axis, (1 - scale) * length));
}

/// <summary>
/// OBBと点の最短距離を求める
/// </summary>
/// <param name="pointPosition">点の座標</param>
/// <param name="boxCenterPosition">OBBの中心座標</param>
/// <param name="boxSizeXYZ">ボックスの各軸の大きさ</param>
/// <param name="boxAxis">ボックスの各軸の方向</param>
/// <returns></returns>
inline float CalculateOBBToPoint(const VECTOR pointPosition, const VECTOR boxCenterPosition, const VECTOR boxSizeXYZ, const ObjectAxis& boxAxis)
{
	VECTOR	resultVector = VGet(0.0f, 0.0f, 0.0f);
	
	//x軸方向の重心から線分の端までの長さを求め、はみ出すベクトルを結果に加算
	const VECTOR	boxCenterPositionX	= VGet(boxCenterPosition.x, 0.0f, 0.0f);
	const VECTOR	boxOffSetPositionX	= VGet(boxCenterPosition.x + boxSizeXYZ.x * 0.5f, 0.0f, 0.0f);
	const float		lengthX				= VSub(boxOffSetPositionX, boxCenterPositionX).x;
	addOverVector(resultVector, pointPosition, boxCenterPosition, lengthX, boxAxis.getRightAxis());

	//y軸方向の重心から線分の端までの長さを求め、はみ出すベクトルを結果に加算
	const VECTOR	boxCenterPositionY	= VGet(0.0f, boxCenterPosition.y, 0.0f);
	const VECTOR	boxOffSetPositionY	= VGet(0.0f, boxCenterPosition.y + boxSizeXYZ.y * 0.5f, 0.0f);
	const float		lengthY				= VSub(boxOffSetPositionY, boxCenterPositionY).y;
	addOverVector(resultVector, pointPosition, boxCenterPosition, lengthY, boxAxis.getUpAxis());

	//z軸方向の重心から線分の端までの長さを求め、はみ出すベクトルを結果に加算
	const VECTOR	boxCenterPositionZ	= VGet(0.0f, 0.0f, boxCenterPosition.z);
	const VECTOR	boxOffSetPositionZ	= VGet(0.0f, 0.0f, boxCenterPosition.z + boxSizeXYZ.z * 0.5f);
	const float		lengthZ				= VSub(boxOffSetPositionZ, boxCenterPositionZ).z;
	addOverVector(resultVector, pointPosition, boxCenterPosition, lengthZ, boxAxis.getFowardAxis());

	return VSize(resultVector);
}

/// <summary>
/// VECTOR用lerp
/// </summary>
/// <param name="start">スタートベクトル</param>
/// <param name="end">エンドベクトル</param>
/// <param name="time">タイム（0.0f～1.0fにするのが基本）</param>
/// <returns></returns>
inline VECTOR LerpVector(const VECTOR start, const VECTOR end, const float time)
{
	const auto currentTime = std::clamp(time, 0.0f, 1.0f);
	return VGet(start.x + (end.x - start.x) * currentTime, start.y + (end.y - start.y) * currentTime, start.z + (end.z - start.z) * currentTime);
}

/// <summary>
/// OBBと線分の最短距離を求める
/// </summary>
/// <param name="segmanetPosition1">線分座標１</param>
/// <param name="segmanetPosition2">線分座標2</param>
/// <param name="boxCenterPosition">OBBの中心座標</param>
/// <param name="boxSizeXYZ">OBBの各軸の大きさ</param>
/// <param name="boxAxis">OBBの各軸方向</param>
/// <param name="divSegment">線分の分割数（分割数を多くするほど最短距離の正確度が上がる。処理も重くなる）</param>
/// <returns></returns>
inline float CalculateSegmentWithOBBDistance(const VECTOR segmanetPosition1, const VECTOR segmanetPosition2,
	const VECTOR boxCenterPosition, const VECTOR boxSizeXYZ, const ObjectAxis& boxAxis, unsigned short divSegmentNumber = 10)
{
	float minimumDistance = 0;

	for (int i = 0; i < divSegmentNumber; i++)
	{
		//線分を分割した一点を求める
		const float		time = static_cast<float>(i) / divSegmentNumber;
		const VECTOR	divSegmentPosition = LerpVector(segmanetPosition1, segmanetPosition2, time);

		//現在の距離を求める
		const float currentDistance = CalculateOBBToPoint(divSegmentPosition, boxCenterPosition, boxSizeXYZ, boxAxis);

		//最初は必ず最短距離になる
		if (i == 0)
		{
			minimumDistance = currentDistance;
		}
		else
		{
			//最短距離を更新
			if (minimumDistance < currentDistance) continue;
			minimumDistance = currentDistance;
		}
	}

	return minimumDistance;
}

/// <summary>
/// OBBとカプセルが当たってるか検出
/// </summary>
/// <param name="capsuleTop">カプセルの上部の座標</param>
/// <param name="capsuleBottom">カプセルの下部の座標</param>
/// <param name="capsuleRadius">カプセルの半径</param>
/// <param name="boxCenterPosition">OBBの中心座標</param>
/// <param name="boxSizeXYZ">OBBの各軸の大きさ</param>
/// <param name="boxAxis">OBBの各軸方向</param>
/// <returns></returns>
inline bool IsHitingColliderCapsuleWithOBB(const VECTOR capsuleTop, const VECTOR capsuleBottom, const float capsuleRadius, 
	const VECTOR boxCenterPosition, const VECTOR boxSizeXYZ, const ObjectAxis& boxAxis)
{
	//カプセルの線分とOBB都の最短距離(ざっくり)を求める
	const float minimumDistance = CalculateSegmentWithOBBDistance(capsuleTop, capsuleBottom,
		boxCenterPosition, boxSizeXYZ, boxAxis);

	//最短距離がカプセルの半径より小さかったら衝突
	if (minimumDistance > capsuleRadius) return false;
	return true;
}

/// <summary>
/// OBBと球が当たってるかの検出
/// </summary>
/// <param name="spherePosition">球の座標</param>
/// <param name="sphereRadius">球の半径</param>
/// <param name="boxCenterPosition">OBBの中心座標</param>
/// <param name="boxSizeXYZ">OBBの各軸の大きさ</param>
/// <param name="boxAxis">OBBの各軸方向</param>
/// <returns></returns>
inline bool IsHitingColliderSphereWithOBB(const VECTOR spherePosition, const float sphereRadius,
const VECTOR boxCenterPosition, const VECTOR boxSizeXYZ, const ObjectAxis& boxAxis)
{
	//求めたOBBとの距離が球の半径より小さかったら衝突してる
	const float sphereToOBBDistance = CalculateOBBToPoint(spherePosition, boxCenterPosition, boxSizeXYZ, boxAxis);
	if (sphereToOBBDistance > sphereRadius) return false;

	return true;
}

/// <summary>
/// アクシスを利用し、基準座標からOBBのワールド座標を求める
/// </summary>
/// <param name="centerPosition">OBBの中心座標</param>
/// <param name="axis">OBBの各軸方向</param>
/// <param name="halfSizeX">OBBの幅（ｘ）の半分</param>
/// <param name="halfSizeY">OBBの高さ（ｙ）の半分</param>
/// <param name="halfSizeZ">OBBの深さ（ｚ）の半分</param>
/// <returns></returns>
inline [[nodiscard]] VECTOR GetOBBWorldPosition(const VECTOR centerPosition, const ObjectAxis& axis,
	const float halfSizeX, const float halfSizeY, const float halfSizeZ)
{
	VECTOR tempPosition = centerPosition;
	tempPosition = VAdd(tempPosition, VScale(axis.getRightAxis(), halfSizeX));
	tempPosition = VAdd(tempPosition, VScale(axis.getUpAxis(), halfSizeY));
	tempPosition = VAdd(tempPosition, VScale(axis.getFowardAxis(), halfSizeZ));

	return tempPosition;
}

/// <summary>
/// 3d空間に直方体を描画（回転に対応）
/// </summary>
/// <param name="centerPosition">直方体の中心座標</param>
/// <param name="sizeXYZ">直方体の幅(x)、高さ(y)、深さ(z)の大きさ</param>
/// <param name="cuboidAxis">直方体の各軸方向</param>
/// <param name="color">直方体の色</param>
inline void DrawCuboid3D(const VECTOR centerPosition, const VECTOR sizeXYZ, const ObjectAxis& axis, const int color = GetColor(255, 255, 255))
{
	float halfSizeX = sizeXYZ.x * 0.5f;
	float halfSizeY = sizeXYZ.y * 0.5f;
	float halfSizeZ = sizeXYZ.z * 0.5f;

	//手前左下
	const VECTOR point1 = GetOBBWorldPosition(centerPosition, axis, -halfSizeX, -halfSizeY, -halfSizeZ);
	//奥左下
	const VECTOR point2 = GetOBBWorldPosition(centerPosition, axis, -halfSizeX, -halfSizeY, halfSizeZ);
	//奥右下
	const VECTOR point3 = GetOBBWorldPosition(centerPosition, axis, halfSizeX, -halfSizeY, halfSizeZ);
	//手前右下
	const VECTOR point4 = GetOBBWorldPosition(centerPosition, axis, halfSizeX, -halfSizeY, -halfSizeZ);
	//手前左上
	const VECTOR point5 = GetOBBWorldPosition(centerPosition, axis, -halfSizeX, halfSizeY, -halfSizeZ);
	//奥左上
	const VECTOR point6 = GetOBBWorldPosition(centerPosition, axis, -halfSizeX, halfSizeY, halfSizeZ);
	//奥右上
	const VECTOR point7 = GetOBBWorldPosition(centerPosition, axis, halfSizeX, halfSizeY, halfSizeZ);
	//手前右上
	const VECTOR point8 = GetOBBWorldPosition(centerPosition, axis, halfSizeX, halfSizeY, -halfSizeZ);
	
	DrawLine3D(point1, point2, color);
	DrawLine3D(point1, point4, color);
	DrawLine3D(point1, point5, color);
	DrawLine3D(point2, point3, color);
	DrawLine3D(point2, point6, color);
	DrawLine3D(point3, point4, color);
	DrawLine3D(point3, point7, color);
	DrawLine3D(point4, point8, color);
	DrawLine3D(point5, point6, color);
	DrawLine3D(point5, point8, color);
	DrawLine3D(point6, point7, color);
	DrawLine3D(point7, point8, color);
}

/// <summary>
/// OBBとスフィアの押し戻しベクトルを求める（求めた値はスフィアを押し戻す前提）
/// </summary>
/// <param name="spherePosition">球座標</param>
/// <param name="radius">球の半径</param>
/// <param name="boxCenterPosition">OBBの中心座標</param>
/// <param name="boxSizeXYZ">OBBの各軸の大きさ</param>
/// <param name="boxAxis">OBBの各軸方向</param>
/// <returns></returns>
inline VECTOR CalculatePushBackVectorSphereWithOBB(const VECTOR spherePosition, const float radius,
	const VECTOR boxCenterPosition, const VECTOR boxSizeXYZ, const ObjectAxis& boxAxis)
{
	//OBBの中心座標を原点としたスフィアの座標
	const VECTOR originOBBToSpherePosition = VSub(spherePosition, boxCenterPosition);

	//スフィアのローカル座標を求める
	VECTOR localSpherePosition;
	localSpherePosition.x = VDot(originOBBToSpherePosition, boxAxis.getRightAxis());
	localSpherePosition.y = VDot(originOBBToSpherePosition, boxAxis.getUpAxis());
	localSpherePosition.z = VDot(originOBBToSpherePosition, boxAxis.getFowardAxis());

	//一番近い点を求める
	float halfBoxWidth	= boxSizeXYZ.x * 0.5f;
	float halfBoxHeight = boxSizeXYZ.y * 0.5f;
	float halfBoxDepth	= boxSizeXYZ.z * 0.5f;
	VECTOR localClosest;
	localClosest.x = std::clamp(localSpherePosition.x, -halfBoxWidth, halfBoxWidth);
	localClosest.y = std::clamp(localSpherePosition.y, -halfBoxHeight, halfBoxHeight);
	localClosest.z = std::clamp(localSpherePosition.z, -halfBoxDepth, halfBoxDepth);

	//ワールド座標に変換
	VECTOR warldClosest = VAdd(boxCenterPosition, VScale(boxAxis.getRightAxis(), localClosest.x));
	warldClosest = VAdd(warldClosest, VScale(boxAxis.getUpAxis(), localClosest.y));
	warldClosest = VAdd(warldClosest, VScale(boxAxis.getFowardAxis(), localClosest.z));

	//衝突判定
	const VECTOR	distanceSphereOBB	= VSub(spherePosition, warldClosest);
	const float		distanceSqrt		= std::pow(distanceSphereOBB.x, 2) + std::pow(distanceSphereOBB.y, 2) + std::pow(distanceSphereOBB.z, 2);
	const float		distance			= std::sqrt(distanceSqrt);

	//ディレクションとめり込み漁を計算
	VECTOR	direction	= VNorm(distanceSphereOBB);
	float	penetration	= radius - distance;

	//OBB内部にスフィアのローカル座標がない場合はそのまま押し戻しベクトルを求める
	VECTOR pushVector;
	if (localClosest != localSpherePosition)
	{
		pushVector = VScale(direction, penetration);
		return pushVector;
	}

	//OBB内部にスフィアのローカル座標がある場合は法線ベクトルが作れないので最も近い面を使って押し戻す
	//各面までの距離を求める
	const float distanceX = halfBoxWidth - std::abs(localSpherePosition.x);
	const float distanceY = halfBoxHeight - std::abs(localSpherePosition.y);
	const float distanceZ = halfBoxDepth - std::abs(localSpherePosition.z);

	//どの軸が最短距離か求めて押し出す方向を求める
	float minimumDistance = distanceX;
	direction = (localSpherePosition.x > 0) ? boxAxis.getRightAxis() : VScale(boxAxis.getRightAxis(), -1.0f);
	if (distanceY < minimumDistance)
	{
		minimumDistance = distanceY;
		direction = (localSpherePosition.y > 0) ? boxAxis.getUpAxis() : VScale(boxAxis.getUpAxis(), -1.0f);
	}
	if (distanceZ < minimumDistance)
	{
		minimumDistance = distanceZ;
		direction = (localSpherePosition.z > 0) ? boxAxis.getFowardAxis() : VScale(boxAxis.getFowardAxis(), -1.0f);
	}

	//めり込み量を求める
	penetration = radius + minimumDistance;

	//押し戻しベクトルを求める
	pushVector = VScale(direction, penetration);
	return pushVector;
}

/// <summary>
/// カプセルとOBBの押し戻し
/// </summary>
/// <param name="capsuleTopPosition">カプセルの上部座標</param>
/// <param name="capsuleBottomPosition">カプセルの下部座標</param>
/// <param name="radius">カプセルの半径</param>
/// <param name="boxCenterPosition">OBBの中心座標</param>
/// <param name="boxSizeXYZ">OBBの各軸の大きさ</param>
/// <param name="boxAxis">OBBの各軸方向</param>
/// <returns></returns>
inline VECTOR CalculatePushBackVectorCapsuleWithOBB(const VECTOR capsuleTopPosition, const VECTOR capsuleBottomPosition, const float radius,
	const VECTOR boxCenterPosition, const VECTOR boxSizeXYZ, const ObjectAxis& boxAxis)
{
	float	minimumDistance = 0;
	VECTOR	minimumDistancePosition;

	const short divSegmentNumber = 10;
	for (int i = 0; i < divSegmentNumber; i++)
	{
		//線分を分割した一点を求める
		const float		time = static_cast<float>(i) / divSegmentNumber;
		const VECTOR	divSegmentPosition = LerpVector(capsuleTopPosition, capsuleBottomPosition, time);

		//現在の距離を求める
		const float currentDistance = CalculateOBBToPoint(divSegmentPosition, boxCenterPosition, boxSizeXYZ, boxAxis);

		//最初は必ず最短距離になる
		if (i == 0)
		{
			minimumDistance			= currentDistance;
			minimumDistancePosition = divSegmentPosition;
		}
		else
		{
			//最短距離の分割座標を記録
			if (minimumDistance < currentDistance) continue;
			minimumDistance			= currentDistance;
			minimumDistancePosition = divSegmentPosition;
		}
	}

	const VECTOR pushBackVector = CalculatePushBackVectorSphereWithOBB(minimumDistancePosition, radius, boxCenterPosition, boxSizeXYZ, boxAxis);
	return pushBackVector;
}

/// <summary>
/// スフィアとスフィアの押し戻し（1方向の押し戻ししか取れないので呼び出し側で多少工夫が必要。マイナススケール倍するとかね）
/// </summary>
/// <param name="position1">スフィア１の座標</param>
/// <param name="radius1">スフィア１の半径</param>
/// <param name="position2">スフィア２の座標</param>
/// <param name="radius2">スフィア２の半径</param>
/// <returns></returns>
inline VECTOR CalculatePushBackVectorSphereWithSphere(const VECTOR position1, const float radius1, const VECTOR position2, const float radius2)
{
	const VECTOR	length		= VSub(position1, position2);
	const float		distance	= CalculateDistance<float>(position1, position2);

	//衝突方向を求める
	VECTOR hitingDirection = VGet(0.0f, 0.0f, 0.0f);
	if (distance == 0.0f)	//ゼロ除算を防ぐ
	{
		return hitingDirection;
	}
	hitingDirection.x = length.x / distance;
	hitingDirection.y = length.y / distance;
	hitingDirection.z = length.z / distance;

	//めり込み量を求める
	float penetration = radius1 + radius2 - distance;

	//押し戻しベクトルを求める
	const VECTOR pushBackVector = VScale(hitingDirection, penetration);
	return pushBackVector;
}

/// <summary>
/// スフィアとカプセルの押し戻し（1方向の押し戻ししか取れないので呼び出し側で多少工夫が必要。マイナススケール倍するとかね）
/// </summary>
/// <param name="spherePosition">スフィア座標</param>
/// <param name="sphereRadius">スフィア半径</param>
/// <param name="topPosition">カプセル上部座標</param>
/// <param name="bottomPosition">カプセル下部座標</param>
/// <param name="capsuleRadius">カプセル半径</param>
/// <returns></returns>
inline VECTOR CalculatePushBackVectorSphereWithCapsule(const VECTOR spherePosition, const float sphereRadius,
	const VECTOR topPosition, const VECTOR bottomPosition, const float capsuleRadius)
{
	float	minimumDistance = 0;
	VECTOR	minimumDistancePosition;

	const short divSegmentNumber = 10;
	for (int i = 0; i < divSegmentNumber; i++)
	{
		//線分を分割した一点を求める
		const float		time = static_cast<float>(i) / divSegmentNumber;
		const VECTOR	divSegmentPosition = LerpVector(topPosition, bottomPosition, time);

		//現在の距離を求める
		const float currentDistance = CalculateDistance<float>(spherePosition, divSegmentPosition);

		//最初は必ず最短距離になる
		if (i == 0)
		{
			minimumDistance = currentDistance;
			minimumDistancePosition = divSegmentPosition;
		}
		else
		{
			//最短距離の分割座標を記録
			if (minimumDistance < currentDistance) continue;
			minimumDistance = currentDistance;
			minimumDistancePosition = divSegmentPosition;
		}
	}

	const VECTOR pushBackVector = CalculatePushBackVectorSphereWithSphere(spherePosition, sphereRadius, minimumDistancePosition, capsuleRadius);
	return pushBackVector;
}

/// <summary>
/// カプセルとカプセルの押し戻し（1方向の押し戻ししか取れないので呼び出し側で多少工夫が必要。マイナススケール倍するとかね）
/// </summary>
/// <param name="topPosition1">カプセル１の上部座標</param>
/// <param name="bottomPosition1">カプセル１の下部座標</param>
/// <param name="radius1">カプセル１の半径</param>
/// <param name="topPosition2">カプセル2の上部座標</param>
/// <param name="bottomPosition2">カプセル２の下部座標</param>
/// <param name="radius2">カプセル２の半径</param>
/// <returns></returns>
inline VECTOR CalculatePushBackVectorCapsuleWithCapsule(const VECTOR topPosition1, const VECTOR bottomPosition1, const float radius1,
	const VECTOR topPosition2, const VECTOR bottomPosition2, const float radius2)
{
	//線分同士の最短距離を求める
	const float minimumDistance = Segment_Segment_MinLength(topPosition1, bottomPosition1, topPosition2, bottomPosition2);

	//めり込み量を求める
	float penetration = radius1 + radius2 - minimumDistance;

	//それぞれの最近点を求める
	SEGMENT_SEGMENT_RESULT closestPoint;
	Segment_Segment_Analyse(&topPosition1, &bottomPosition1, &topPosition2, &bottomPosition2, &closestPoint);
	const VECTOR closestPosition1 = closestPoint.SegA_MinDist_Pos;
	const VECTOR closestPosition2 = closestPoint.SegB_MinDist_Pos;

	//押し戻しベクトルを求める
	const VECTOR pushBackDirection	= VNorm(VSub(closestPosition1, closestPosition2));
	const VECTOR pushBackVector		= VScale(pushBackDirection, penetration);
	return pushBackVector;
}

/// <summary>
/// カプセルと線分の最短距離を求める
/// </summary>
/// <param name="segmentPosition1">線分の座標１</param>
/// <param name="segmentPosition2">線分の座標２</param>
/// <param name="topPosition">カプセルの上部座標</param>
/// <param name="bottomPosition">カプセルの下部座標</param>
/// <param name="radius">カプセル半径</param>
/// <returns></returns>
inline float CalculateMinimumDistanceSegmentToCapsule(const VECTOR segmentPosition1, const VECTOR segmentPosition2,
	const VECTOR topPosition, const VECTOR bottomPosition, const float radius)
{
	float minimumDistance			= 0;
	const short divSegmentNumber	= 10;
	for (int i = 0; i < divSegmentNumber; i++)
	{
		//線分を分割した一点を求める
		const float		time = static_cast<float>(i) / divSegmentNumber;
		const VECTOR	divSegmentPosition = LerpVector(bottomPosition, topPosition, time);

		//現在の距離を求める
		const float currentDistance = Segment_Point_MinLength(segmentPosition1, segmentPosition2, divSegmentPosition);

		//最初は必ず最短距離になる
		if (i == 0)
		{
			minimumDistance = currentDistance;
		}
		else
		{
			//最短距離を更新
			if (minimumDistance < currentDistance) continue;
			minimumDistance = currentDistance;
		}
	}

	//点と線分の最短距離から円の半径を引いたものがカプセルと線分の最短距離
	return minimumDistance - radius;
}

/// <summary>
/// 線分とOBBの当たり判定
/// </summary>
/// <param name="position1"></param>
/// <param name="position2"></param>
/// <param name="boxCenterPosition"></param>
/// <param name="boxSizeXYZ"></param>
/// <param name="boxAxis"></param>
/// <returns></returns>
inline bool IsHitSegmentWithOBB(const VECTOR position1, const VECTOR position2, const VECTOR boxCenterPosition, const VECTOR boxSizeXYZ, const ObjectAxis& boxAxis)
{
	//線分がワールド座標なのでOBBのローカル座標に変換する（OBB中心から見た座標にする）
	const VECTOR localSegmentStart		= VSub(position1, boxCenterPosition);
	const VECTOR localSegmentEnd		= VSub(position2, boxCenterPosition);
	const VECTOR localPosition 
		= VGet(VDot(localSegmentStart, boxAxis.getRightAxis()), VDot(localSegmentStart, boxAxis.getUpAxis()), VDot(localSegmentStart, boxAxis.getFowardAxis()));

	//OBBの各軸へ投影（正射影ベクトルの長さを計算）する
	const VECTOR localSegmentDirection	= VSub(localSegmentEnd, localSegmentStart);
	const VECTOR localAxisLength
		= VGet(VDot(localSegmentDirection, boxAxis.getRightAxis()), VDot(localSegmentDirection, boxAxis.getUpAxis()), VDot(localSegmentDirection, boxAxis.getFowardAxis()));

	//サイズの半分を求めておく（スラブ法で使う）
	const VECTOR boxSizeHalf = VGet(boxSizeXYZ.x * 0.5f, boxSizeXYZ.y * 0.5f, boxSizeXYZ.z * 0.5f);

	//スラブ法の線分の初期有効期間
	float segmentActiveStart	= 0.0f;	//線分全体の始点
	float segmentActiveEnd		= 1.0f;	//線分全体の終点

	//線分がほぼ動いてなければ実質平行なので、座標が箱のサイズの外にいれば当たってない判定（1e-6fは0.000001）
	if (fabsf(localAxisLength.x) < 1e-6f)
	{
		if (localPosition.x < -boxSizeHalf.x || localPosition.x > boxSizeHalf.x) return false;
	}
	else
	{
		//近い交点と遠い交点を求める
		float nearX = (-boxSizeHalf.x - localPosition.x) / localAxisLength.x;
		float farX	= (boxSizeHalf.x - localPosition.x) / localAxisLength.x;

		//線分が交差に入る向きが逆の可能性を考える
		if (nearX > farX)
		{
			std::swap(nearX, farX);
		}

		//有効期間を更新
		segmentActiveStart	= (std::max)(segmentActiveStart, nearX);
		segmentActiveEnd	= (std::min)(segmentActiveEnd, farX);

		//重なってる期間がなくなってたら当たってない判定
		if (segmentActiveStart > segmentActiveEnd) return false;
	}
	
	//線分がほぼ動いてなければ実質平行なので、座標が箱のサイズの外にいれば当たってない判定（1e-6fは0.000001）
	if (fabsf(localAxisLength.y) < 1e-6f)
	{
		if (localPosition.y < -boxSizeHalf.y || localPosition.y > boxSizeHalf.y) return false;
	}
	else
	{
		//近い交点と遠い交点を求める
		float nearY = (-boxSizeHalf.y - localPosition.y) / localAxisLength.y;
		float farY	= (boxSizeHalf.y - localPosition.y) / localAxisLength.y;

		//線分が交差に入る向きが逆の可能性を考える
		if (nearY > farY)
		{
			std::swap(nearY, farY);
		}

		//有効期間を更新
		segmentActiveStart	= (std::max)(segmentActiveStart, nearY);
		segmentActiveEnd	= (std::min)(segmentActiveEnd, farY);

		//重なってる期間がなくなってたら当たってない判定
		if (segmentActiveStart > segmentActiveEnd) return false;
	}
	
	//線分がほぼ動いてなければ実質平行なので、座標が箱のサイズの外にいれば当たってない判定（1e-6fは0.000001）
	if (fabsf(localAxisLength.z) < 1e-6f)
	{
		if (localPosition.z < -boxSizeHalf.z || localPosition.z > boxSizeHalf.z) return false;
	}
	else
	{
		//近い交点と遠い交点を求める
		float nearZ = (-boxSizeHalf.z - localPosition.z) / localAxisLength.z;
		float farZ	= (boxSizeHalf.z - localPosition.z) / localAxisLength.z;

		//線分が交差に入る向きが逆の可能性を考える
		if (nearZ > farZ)
		{
			std::swap(nearZ, farZ);
		}

		//有効期間を更新
		segmentActiveStart	= (std::max)(segmentActiveStart, nearZ);
		segmentActiveEnd	= (std::min)(segmentActiveEnd, farZ);

		//重なってる期間がなくなってたら当たってない判定
		if (segmentActiveStart > segmentActiveEnd) return false;
	}
	
	//すべての軸で交差している
	return true;
}

/// <summary>
/// モデルのボーンからx->z->y軸回転の回転値を取得する
/// </summary>
/// <param name="modelhandle"></param>
/// <param name="boneName"></param>
/// <returns></returns>
inline const VECTOR GetBoneRotationAngleXZY(const int modelhandle, const std::string& boneName)noexcept
{
	VECTOR rotationAngle = VGet(0.0f, 0.0f, 0.0f);

	const MATRIX boneMatrix = MV1GetFrameLocalWorldMatrix(modelhandle, MV1SearchFrame(modelhandle, boneName.c_str()));

	//モデルのボーンがどのくらい回転してるかを得る
	GetMatrixXZYRotation(&boneMatrix, &rotationAngle.x, &rotationAngle.y, &rotationAngle.z);

	return rotationAngle;
}

/// <summary>
/// dxlibのdrawstring系でstd::stringを文字化けなしで使えるようにするやつ
/// </summary>
/// <param name="utf8"></param>
/// <returns></returns>
inline std::string Utf8ToSJIS(const std::string& utf8)
{
	int size = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, NULL, 0);
	std::wstring wide(size, 0);
	MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, &wide[0], size);

	size = WideCharToMultiByte(CP_ACP, 0, wide.c_str(), -1, NULL, 0, NULL, NULL);
	std::string sjis(size, 0);
	WideCharToMultiByte(CP_ACP, 0, wide.c_str(), -1, &sjis[0], size, NULL, NULL);
	sjis.pop_back(); // 終止ニル文字除去
	return sjis;
}

/// <summary>
/// 中央ぞろえで文字を描画する座標を取得する
/// </summary>
/// <param name="textPosition">文字を描画したい座標</param>
/// <param name="text">描画したい文字</param>
/// <param name="fontHandle">指定したいフォント（あれば）</param>
/// <returns></returns>
inline VECTOR GetCenteredTextPosition(const VECTOR textPosition, const std::string& text, const int fontHandle = -1)
{
	int textWidth;
	int textHeight;

	//テキストの幅と高さを取得
	if (fontHandle == -1)
	{
		textWidth	= GetDrawStringWidth(text.c_str(), text.length());
		textHeight	= GetFontSize();
	}
	else
	{
		textWidth	= GetDrawStringWidthToHandle(text.c_str(), text.length(), fontHandle);
		textHeight	= GetFontSizeToHandle(fontHandle);
	}

	const VECTOR resultPosition = VGet(textPosition.x - textWidth * 0.5f, textPosition.y - textHeight * 0.5f, 0.0f);
	return resultPosition;
}

/// <summary>
/// 指定の日本語文字列から、指定以上の長さ部分を分割して取得する
/// </summary>
/// <param name="text"></param>
/// <param name="maxLength"></param>
/// <returns></returns>
inline std::string SplitOverLength(const std::string& text, const int maxLength)
{
	int charCount		= 0;
	size_t byteIndex	= 0;

	//maxLength文字分を進める
	while (byteIndex < text.size() && charCount < maxLength)
	{
		unsigned char c = static_cast<unsigned char>(text[byteIndex]);

		// Shift-JISの2バイト文字
		if ((c >= 0x81 && c <= 0x9F) ||
			(c >= 0xE0 && c <= 0xEF))
		{
			byteIndex += 2;
		}
		// ASCII
		else
		{
			byteIndex += 1;
		}

		charCount++;
	}

	// maxLength文字以降を取得
	return text.substr(byteIndex);
}

/// <summary>
/// 指定の文字列が一定以上の長さだったら、はみ出している部分を削除した文字列を取得する
/// </summary>
/// <param name="text"></param>
/// <param name="maxLength"></param>
inline std::string TruncateString(const std::string& text, int maxLength)
{
	std::string tempText	= text;
	int charCount			= 0;
	size_t byteIndex		= 0;

	while (byteIndex < tempText.size() && charCount < maxLength)
	{
		unsigned char c = static_cast<unsigned char>(text[byteIndex]);

		// Shift-JISの2バイト文字
		if ((c >= 0x81 && c <= 0x9F) ||
			(c >= 0xE0 && c <= 0xEF))
		{
			byteIndex += 2;
		}
		// ASCII
		else
		{
			byteIndex += 1;
		}

		charCount++;
	}

	return text.substr(0, byteIndex);
}