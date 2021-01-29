#include "AnimSpriteComponent.h"
#include "Actor.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder = 100)
    :SpriteComponent(owner, drawOrder)
    , mCurrFrame(0.0f)
    , mAnimFPS(0.0f)
{

}

void AnimSpriteComponent::Update(float deltaTime)
{
    SpriteComponent::Update(deltaTime);

    if (mAnimTextures.size() > 0)
    {
        // ������ ����Ʈ�� ��Ÿ �ð��� �������
        // ���� ������ ����
        mCurrFrame += mAnimFPS * deltaTime;

        // �ִϸ��̼� �ؽ�ó ���� �ʰ��ϸ� ���� �������� �����Ѵ�.
        while (mCurrFrame >= mAnimTextures.size())
        {
            mCurrFrame -= mAnimTextures.size();
        }

        // ���� �ؽ�ó�� ����
        SetTexture(mAnimTextures[static_cast<int> (mCurrFrame)]);
    }
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures)
{
    mAnimTextures = textures;
    if (mAnimTextures.size() > 0)
    {
        // Set the active texture to first frame
        mCurrFrame = 0.0f;
        SetTexture(mAnimTextures[0]);
    }
}