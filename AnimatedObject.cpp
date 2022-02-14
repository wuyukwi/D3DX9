#include "Engine.h"

AnimatedObject::AnimatedObject(char *meshName, char *meshPath, unsigned long type) : SceneObject(type, meshName, meshPath, false) {
    // アニメーションコントローラーのクローン作成
    if (GetMesh() != NULL)
        GetMesh()->CloneAnimationController(&m_animationController);
    else
        m_animationController = NULL;

    // トラック速度を設定
    if (m_animationController != NULL) {
        m_animationController->SetTrackSpeed(0, 1.0f);
        m_animationController->SetTrackSpeed(1, 1.0f);
    }

    // アニメーション用変数を初期化
    m_currentTrack = 0;
    m_currentTime = 0.0f;
}

//-----------------------------------------------------------------------------
// アニメーションオブジェクトのデストラクタ
//-----------------------------------------------------------------------------

AnimatedObject::~AnimatedObject() {
    SAFE_RELEASE(m_animationController);
}

//-----------------------------------------------------------------------------
// アニメーションオブジェクトの更新処理
//-----------------------------------------------------------------------------

void AnimatedObject::Update(float elapsed, bool addVelocity) {
    // シーンオブジェクトの更新処理
    SceneObject::Update(elapsed, addVelocity);

    // アニメーションコントローラーあるかどうか
    if (m_animationController) {
        // メッシュをアニメーション化し、指定された量だけグローバルアニメーションの時間を進めます。
        m_animationController->AdvanceTime(elapsed, this);

        // 経過時間加算
        m_currentTime += elapsed;
    }

    // メッシュ更新処理.
    if (GetMesh() != NULL)
        GetMesh()->Update();
}

//-----------------------------------------------------------------------------
// 指定された時間で指定されたアニメーションを再生します。
//-----------------------------------------------------------------------------

void AnimatedObject::PlayAnimation(unsigned int animation, float transitionTime, bool loop) {
    // オブジェクトに有効なアニメーションコントローラがあることを確認
    if (m_animationController == NULL)
        return;

    // 遷移時間が常にゼロより大きい
    if (transitionTime <= 0.0f)
        transitionTime = 0.000001f;

    // 新しいアニメーションを再生するトラック
    unsigned int newTrack = (m_currentTrack == 0 ? 1 : 0);

    // 再生するアニメーションセットへのポインタを取得
    ID3DXAnimationSet *as;
    m_animationController->GetAnimationSet(animation, &as);

    // アニメーションセットを新しいトラックに設定
    m_animationController->SetTrackAnimationSet(newTrack, as);

    // トラックに現在設定されているすべてのイベントをクリア
    m_animationController->UnkeyAllTrackEvents(m_currentTrack);
    m_animationController->UnkeyAllTrackEvents(newTrack);

    // このアニメーションをループするか、1回だけ再生するかを確認
    if (loop == true) {
        // 指定された時間になったら新しいトラックを移行します。
        m_animationController->KeyTrackEnable(m_currentTrack, false, m_currentTime + transitionTime);
        m_animationController->KeyTrackWeight(m_currentTrack, 0.0f, m_currentTime, transitionTime, D3DXTRANSITION_LINEAR);
        m_animationController->SetTrackEnable(newTrack, true);
        m_animationController->KeyTrackWeight(newTrack, 1.0f, m_currentTime, transitionTime, D3DXTRANSITION_LINEAR);
    } else {
        // 現在のトラックを停止し、移行せずに新しいトラックを開始する
        m_animationController->SetTrackEnable(m_currentTrack, false);
        m_animationController->SetTrackWeight(m_currentTrack, 0.0f);
        m_animationController->SetTrackEnable(newTrack, true);
        m_animationController->SetTrackWeight(newTrack, 1.0f);
        m_animationController->SetTrackPosition(newTrack, 0.0f);
        m_animationController->KeyTrackEnable(newTrack, false, m_currentTime + as->GetPeriod());
    }

    // アニメーションセットへのポインタをリリース
    as->Release();

    // 新しいトラックがいまのトラックに
    m_currentTrack = newTrack;
}

//-----------------------------------------------------------------------------
// オブジェクトのアニメーションコントローラへのポインタを返します。
//-----------------------------------------------------------------------------

ID3DXAnimationController *AnimatedObject::GetAnimationController() {
    return m_animationController;
}

//-----------------------------------------------------------------------------
// アニメーションコールバックハンドラー。
//-----------------------------------------------------------------------------

HRESULT CALLBACK AnimatedObject::HandleCallback(THIS_ UINT Track, LPVOID pCallbackData) {
    return S_OK;
}

