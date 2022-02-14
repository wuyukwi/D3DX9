#include "Engine.h"

AnimatedObject::AnimatedObject(char *meshName, char *meshPath, unsigned long type) : SceneObject(type, meshName, meshPath, false) {
    // �A�j���[�V�����R���g���[���[�̃N���[���쐬
    if (GetMesh() != NULL)
        GetMesh()->CloneAnimationController(&m_animationController);
    else
        m_animationController = NULL;

    // �g���b�N���x��ݒ�
    if (m_animationController != NULL) {
        m_animationController->SetTrackSpeed(0, 1.0f);
        m_animationController->SetTrackSpeed(1, 1.0f);
    }

    // �A�j���[�V�����p�ϐ���������
    m_currentTrack = 0;
    m_currentTime = 0.0f;
}

//-----------------------------------------------------------------------------
// �A�j���[�V�����I�u�W�F�N�g�̃f�X�g���N�^
//-----------------------------------------------------------------------------

AnimatedObject::~AnimatedObject() {
    SAFE_RELEASE(m_animationController);
}

//-----------------------------------------------------------------------------
// �A�j���[�V�����I�u�W�F�N�g�̍X�V����
//-----------------------------------------------------------------------------

void AnimatedObject::Update(float elapsed, bool addVelocity) {
    // �V�[���I�u�W�F�N�g�̍X�V����
    SceneObject::Update(elapsed, addVelocity);

    // �A�j���[�V�����R���g���[���[���邩�ǂ���
    if (m_animationController) {
        // ���b�V�����A�j���[�V���������A�w�肳�ꂽ�ʂ����O���[�o���A�j���[�V�����̎��Ԃ�i�߂܂��B
        m_animationController->AdvanceTime(elapsed, this);

        // �o�ߎ��ԉ��Z
        m_currentTime += elapsed;
    }

    // ���b�V���X�V����.
    if (GetMesh() != NULL)
        GetMesh()->Update();
}

//-----------------------------------------------------------------------------
// �w�肳�ꂽ���ԂŎw�肳�ꂽ�A�j���[�V�������Đ����܂��B
//-----------------------------------------------------------------------------

void AnimatedObject::PlayAnimation(unsigned int animation, float transitionTime, bool loop) {
    // �I�u�W�F�N�g�ɗL���ȃA�j���[�V�����R���g���[�������邱�Ƃ��m�F
    if (m_animationController == NULL)
        return;

    // �J�ڎ��Ԃ���Ƀ[�����傫��
    if (transitionTime <= 0.0f)
        transitionTime = 0.000001f;

    // �V�����A�j���[�V�������Đ�����g���b�N
    unsigned int newTrack = (m_currentTrack == 0 ? 1 : 0);

    // �Đ�����A�j���[�V�����Z�b�g�ւ̃|�C���^���擾
    ID3DXAnimationSet *as;
    m_animationController->GetAnimationSet(animation, &as);

    // �A�j���[�V�����Z�b�g��V�����g���b�N�ɐݒ�
    m_animationController->SetTrackAnimationSet(newTrack, as);

    // �g���b�N�Ɍ��ݐݒ肳��Ă��邷�ׂẴC�x���g���N���A
    m_animationController->UnkeyAllTrackEvents(m_currentTrack);
    m_animationController->UnkeyAllTrackEvents(newTrack);

    // ���̃A�j���[�V���������[�v���邩�A1�񂾂��Đ����邩���m�F
    if (loop == true) {
        // �w�肳�ꂽ���ԂɂȂ�����V�����g���b�N���ڍs���܂��B
        m_animationController->KeyTrackEnable(m_currentTrack, false, m_currentTime + transitionTime);
        m_animationController->KeyTrackWeight(m_currentTrack, 0.0f, m_currentTime, transitionTime, D3DXTRANSITION_LINEAR);
        m_animationController->SetTrackEnable(newTrack, true);
        m_animationController->KeyTrackWeight(newTrack, 1.0f, m_currentTime, transitionTime, D3DXTRANSITION_LINEAR);
    } else {
        // ���݂̃g���b�N���~���A�ڍs�����ɐV�����g���b�N���J�n����
        m_animationController->SetTrackEnable(m_currentTrack, false);
        m_animationController->SetTrackWeight(m_currentTrack, 0.0f);
        m_animationController->SetTrackEnable(newTrack, true);
        m_animationController->SetTrackWeight(newTrack, 1.0f);
        m_animationController->SetTrackPosition(newTrack, 0.0f);
        m_animationController->KeyTrackEnable(newTrack, false, m_currentTime + as->GetPeriod());
    }

    // �A�j���[�V�����Z�b�g�ւ̃|�C���^�������[�X
    as->Release();

    // �V�����g���b�N�����܂̃g���b�N��
    m_currentTrack = newTrack;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̃A�j���[�V�����R���g���[���ւ̃|�C���^��Ԃ��܂��B
//-----------------------------------------------------------------------------

ID3DXAnimationController *AnimatedObject::GetAnimationController() {
    return m_animationController;
}

//-----------------------------------------------------------------------------
// �A�j���[�V�����R�[���o�b�N�n���h���[�B
//-----------------------------------------------------------------------------

HRESULT CALLBACK AnimatedObject::HandleCallback(THIS_ UINT Track, LPVOID pCallbackData) {
    return S_OK;
}

