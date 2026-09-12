#include"DxLibForIreven.h"
#include"Game.h"

const float		camera_near = 1.0f;
const float		caera_far = 750.0f;

void camera(VECTOR position, VECTOR targetPosition)
{
	//奥行1～100までをカメラの描画範囲とする
	SetCameraNearFar(camera_near, caera_far);

	//カメラの注視点を設定
	SetCameraPositionAndTarget_UpVecY(position, targetPosition);
}

int WINAPI WinMain(winmainSetting)
{
    InitDxLib();

	VECTOR playerPos = VGet(0.0f, 0.0f, 0.0f);
	VECTOR capsuleTop = VGet(10.0f, 0.0f, 0.0f);
	VECTOR capsuleTop2 = VGet(0.0f, 0.0f, 0.0f);
	VECTOR spherePos = VGet(0.0f, 0.0f, 0.0f);
	VECTOR spherePos2 = VGet(10.0f, 0.0f, 10.0f);
	VECTOR capsulePos = VGet(10.0f, 0.0f, 0.0f);
	VECTOR capsulePos2 = VGet(-10.0f, 0.0f, -10.0f);
	const float collRadius = 2.0f;
	
	ObjectAxis cuboidAxis;
	const float cuboidSize = 5.0f;

	//ゼットバッファを有効にする
	SetUseZBufferFlag(true);
	//Ｚバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	while (gameRoopSetting)
	{
		//画面に描かれた物を消す(ゲームループの最初に呼ぶ)
		ClearDrawScreen();

		const float moveSpeed = 0.25f;
		if(CheckHitKey(KEY_INPUT_RIGHT))
		{
			playerPos.x += moveSpeed;
		}
		if (CheckHitKey(KEY_INPUT_LEFT))
		{
			playerPos.x -= moveSpeed;
		}
		if (CheckHitKey(KEY_INPUT_UP))
		{
			playerPos.z += moveSpeed;
		}
		if (CheckHitKey(KEY_INPUT_DOWN))
		{
			playerPos.z -= moveSpeed;
		}
		capsuleTop = VAdd(capsulePos, VGet(0.0f, 5.0f, 0.0f));
		capsuleTop2 = VAdd(capsulePos2, VGet(0.0f, 5.0f, 0.0f));

		if (CheckHitKey(KEY_INPUT_1))
		{
			cuboidAxis.rotationX(DX_PI_F * 0.25f);
		}
		if (CheckHitKey(KEY_INPUT_2))
		{
			cuboidAxis.rotationY(DX_PI_F * 0.25f);
		}
		if (CheckHitKey(KEY_INPUT_3))
		{
			cuboidAxis.rotationZ(DX_PI_F * 0.25f);
		}

		DrawCuboid3D(playerPos, VGet(cuboidSize, cuboidSize, cuboidSize), cuboidAxis);
		DrawLine3D(playerPos, VAdd(playerPos, VScale(cuboidAxis.fowardAxis_, cuboidSize * 2.0f)), GetColor(255, 0, 0));
		DrawLine3D(playerPos, VAdd(playerPos, VScale(cuboidAxis.upAxis_, cuboidSize * 2.0f)), GetColor(0, 255, 0));
		DrawLine3D(playerPos, VAdd(playerPos, VScale(cuboidAxis.rightAxis_, cuboidSize * 2.0f)), GetColor(0, 0, 255));
		DrawSphere3D(spherePos, collRadius, 32, GetColor(255, 255, 255), GetColor(255, 255, 255), false);
		DrawSphere3D(spherePos2, collRadius, 32, GetColor(255, 255, 255), GetColor(255, 255, 255), false);
		DrawCapsule3D(capsuleTop, capsulePos, collRadius, 32, GetColor(255, 255, 255), GetColor(255, 255, 255), false);
		DrawCapsule3D(capsuleTop2, capsulePos2, collRadius, 32, GetColor(255, 255, 255), GetColor(255, 255, 255), false);

		bool isHitCapsule = IsHitingColliderCapsuleWithOBB(capsuleTop, capsulePos, collRadius,
			playerPos, VGet(cuboidSize, cuboidSize, cuboidSize), cuboidAxis);
		if (isHitCapsule)
		{
			DrawString(1000.0f, 500.0f, "hit!!!", GetColor(255, 0, 0));
			const VECTOR pushVector = CalculatePushBackVectorCapsuleWithOBB(capsuleTop,capsulePos,collRadius, playerPos,
				VGet(cuboidSize, cuboidSize, cuboidSize), cuboidAxis);
			capsulePos = VAdd(capsulePos, pushVector);
		}

		bool isHitCapsule2 = IsHitingColliderCapsuleWithOBB(capsuleTop2, capsulePos2, collRadius,
			playerPos, VGet(cuboidSize, cuboidSize, cuboidSize), cuboidAxis);
		if (isHitCapsule2)
		{
			DrawString(1000.0f, 500.0f, "hit!!!", GetColor(255, 0, 0));
			const VECTOR pushVector = CalculatePushBackVectorCapsuleWithOBB(capsuleTop2, capsulePos2, collRadius, playerPos,
				VGet(cuboidSize, cuboidSize, cuboidSize), cuboidAxis);
			capsulePos2 = VAdd(capsulePos2, pushVector);
		}

		bool isHitSphere = IsHitingColliderSphereWithOBB(spherePos, collRadius,
			playerPos, VGet(cuboidSize, cuboidSize, cuboidSize), cuboidAxis);
		if (isHitSphere)
		{
			const VECTOR pushVector = CalculatePushBackVectorSphereWithOBB(spherePos, collRadius, playerPos,
				VGet(cuboidSize, cuboidSize, cuboidSize), cuboidAxis);
			spherePos = VAdd(spherePos, pushVector);
			DrawString(1000.0f, 500.0f, "hit!!!", GetColor(255, 0, 0));
		}

		bool isHitSphere2 = IsHitingColliderSphereWithOBB(spherePos2, collRadius,
			playerPos, VGet(cuboidSize, cuboidSize, cuboidSize), cuboidAxis);
		if (isHitSphere2)
		{
			const VECTOR pushVector = CalculatePushBackVectorSphereWithOBB(spherePos2, collRadius, playerPos,
				VGet(cuboidSize, cuboidSize, cuboidSize), cuboidAxis);
			spherePos2 = VAdd(spherePos2, pushVector);
			DrawString(1000.0f, 500.0f, "hit!!!", GetColor(255, 0, 0));
		}

		bool isHitCapsuleWithSphere = HitCheck_Sphere_Capsule(spherePos, collRadius, capsulePos, capsuleTop, collRadius);
		if (isHitCapsuleWithSphere)
		{
			const VECTOR pushVector = CalculatePushBackVectorSphereWithCapsule(spherePos, collRadius, capsulePos, capsuleTop, collRadius);
			spherePos	= VAdd(spherePos, pushVector);
			capsulePos	= VAdd(capsulePos, VScale(pushVector, -1.0f));
			DrawString(1000.0f, 500.0f, "hit!!!", GetColor(255, 0, 0));
		}

		bool isHitCapsuleWithSphere2 = HitCheck_Sphere_Capsule(spherePos2, collRadius, capsulePos, capsuleTop, collRadius);
		if (isHitCapsuleWithSphere2)
		{
			const VECTOR pushVector = CalculatePushBackVectorSphereWithCapsule(spherePos2, collRadius, capsulePos, capsuleTop, collRadius);
			spherePos2 = VAdd(spherePos2, pushVector);
			capsulePos = VAdd(capsulePos, VScale(pushVector, -1.0f));
			DrawString(1000.0f, 500.0f, "hit!!!", GetColor(255, 0, 0));
		}

		bool isHitSphereWithSphere = HitCheck_Sphere_Sphere(spherePos, collRadius, spherePos2, collRadius);
		if (isHitSphereWithSphere)
		{
			const VECTOR pushVector = CalculatePushBackVectorSphereWithSphere(spherePos, collRadius, spherePos2, collRadius);
			spherePos = VAdd(spherePos, pushVector);
			spherePos2 = VAdd(spherePos2, VScale(pushVector, -1.0f));
			DrawString(1000.0f, 500.0f, "hit!!!", GetColor(255, 0, 0));
		}
		
		bool isHitCapsuleWithCapsule = HitCheck_Capsule_Capsule(capsulePos, capsuleTop, collRadius, capsulePos2, capsuleTop2, collRadius);
		if (isHitCapsuleWithCapsule)
		{
			const VECTOR pushVector = CalculatePushBackVectorCapsuleWithCapsule(capsulePos, capsuleTop, collRadius, capsulePos2, capsuleTop2, collRadius);
			capsulePos = VAdd(capsulePos, pushVector);
			capsulePos2 = VAdd(capsulePos2, VScale(pushVector, -1.0f));
			DrawString(1000.0f, 500.0f, "hit!!!", GetColor(255, 0, 0));
		}

		DrawFormatString(0.0f, 0.0f, GetColor(255, 255, 255), "upAxisX:%f, upAxisY:%f, upAxisZ:%f", cuboidAxis.upAxis_.x, cuboidAxis.upAxis_.y, cuboidAxis.upAxis_.z);
		DrawFormatString(0.0f, 20.0f, GetColor(255, 255, 255), "fowardAxisX:%f, fowardY:%f, fowardZ:%f", cuboidAxis.fowardAxis_.x, cuboidAxis.fowardAxis_.y, cuboidAxis.fowardAxis_.z);
		DrawFormatString(0.0f, 40.0f, GetColor(255, 255, 255), "rightAxisX:%f, rightAxisY:%f, rightAxisZ:%f", cuboidAxis.rightAxis_.x, cuboidAxis.rightAxis_.y, cuboidAxis.rightAxis_.z);

		camera(VAdd(playerPos, VGet(0.0f, 15.0f, -30.0f)), playerPos);
		
		//裏画面の内容を表画面に反映(ゲームループの最後に呼ぶ)
		ScreenFlip();
	}

    DxLib_End();

    return 0;
}