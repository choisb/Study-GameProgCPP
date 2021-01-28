#include "Game.h"
#include "SDL/SDL_image.h"
#include <algorithm>
#include "Actor.h"

Game::Game()
    : mWindow(nullptr)
    , mRenderer(nullptr)
    , mIsRunning(true)
    , mUpdatingActors(false)
{
}

bool Game::Initialize()
{
    int sdResult = SDL_Init(SDL_INIT_VIDEO);    // ������ 0�� ��ȯ
    if (sdResult != 0)
    {
        // SDL_Log�� C�� Printf�� ���� ���� ���
        // SDL_GetError()�� C ��Ÿ���� ���ڿ��� ���� �޽����� ��ȯ.
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow(
        "Game Programming in C++ (Chapter 2)", // ������ ����
        100,    // �������� ���� ��� x��ǥ
        100,    // �������� ���� ��� y��ǥ
        1024,   // �������� �ʺ�
        768,    // �������� ����
        0       // �÷��� (0�� ��� �÷��׵� �������� ������ �ǹ�)
    );

    if (!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(
        mWindow,    // �������� ���� ������ ������
        -1,         // ����� �׷���ī�� ����. -1�� ��� SDL�� �˾Ƽ� ���� 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC    // �÷��� ��.����ȭ ������ ��� ���ο� ���� ����ȭ�� Ȱ��ȭ ���� ����.
    );
    if (!mRenderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }


    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
        return false;
    }

    mTicksCount = SDL_GetTicks();

    return true;
}

void Game::RunLoop()
{
    while (mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput()
{
    SDL_Event event;

    // ť�� ������ �̺�Ʈ�� ���� �ִ� ����
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            // �̺�Ʈ�� SDL_QUIT�̸� ������ �����Ѵ�.
        case SDL_QUIT:
            mIsRunning = false;
            break;
        }
    }

    // Ű���� ���� ���
    const Uint8* state = SDL_GetKeyboardState(NULL);
    // �̽������� Ű�� �����ٸ� ���� ����
    if (state[SDL_SCANCODE_ESCAPE])
    {
        mIsRunning = false;
    }

}

void Game::UpdateGame()
{
    // ������ ������ ���ķ� 16ms�� ����� �� ���� ���
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
        ;

    // ���� �������� ���� ���� �ð��� ����.
    mTicksCount = SDL_GetTicks();

    // ��Ÿ �ð��� ���� ������ ƽ���� ���� ������ ƽ���� ����.
    // (���� �� ������ ��ȯ)
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    // �ִ� ��Ÿ �ð������� ����
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }

    // ��� ���͸� ����
    mUpdatingActors = true;
    for (auto actor : mActors)
    {
        actor->Update(deltaTime);
    }
    mUpdatingActors = false;

    // ��� ���� ���͸� mActors�� �̵�
    for (auto pending : mPendingActors)
    {
        mActors.emplace_back(pending);
    }
    mPendingActors.clear();

    // ���� ���͸� �ӽ� ���Ϳ� �߰�
    std::vector<Actor*> deadActors;
    for (auto actor : mActors)
    {
        if (actor->GetState() == Actor::EDead)
        {
            deadActors.emplace_back(actor);
        }
    }

    // ���� ���� ����(mActors���� �߷��� ���͵�)
    for (auto actor : deadActors)
    {
        delete actor;
    }
}

void Game::GenerateOutput()
{
    //mRenderer�� (0,0,255,255) �Ķ������� ����
    SDL_SetRenderDrawColor(
        mRenderer,
        0,      // R
        0,      // G
        255,    // B
        255     // A
    );

    // ���� �׸��� �������� �ĸ� ���� �����
    SDL_RenderClear(mRenderer);

    // ���� ����� �׸��� �ڵ尡 �� ��
    //mRenderer�� (255,255,255,255) ������� ����
    SDL_SetRenderDrawColor(
        mRenderer,
        0,      // R
        255,    // G
        0,      // B
        255     // A
    );

    /////////////////////////////////

    // ���� ���ۿ� �ĸ� ���� ��ȯ
    SDL_RenderPresent(mRenderer);
}

void Game::UnloadData()
{
    // delete actors
    while (!mActors.empty())
    {
        delete mActors.back();
    }
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
    // ��ȯ�� texture�� �ּҰ��� ���� ����
    SDL_Texture* tex = nullptr;

    // unordered_map �����̳ʿ� ����Ǿ� �ִ� mTextures���� fileName���� �˻�
    auto iter = mTextures.find(fileName);
    // fileName�� �ش��ϴ� texture�� �����Ѵٸ�
    if (iter != mTextures.end())
    {
        tex = iter->second;
    }
    else
    {
        // ���Ϸκ��� �ε�
        SDL_Surface* surf = IMG_Load(fileName.c_str());
        if (!surf)
        {
            SDL_Log("Failed to load texture file %s", fileName.c_str());
            return nullptr;
        }

        // �ؽ�ó ����
        tex = SDL_CreateTextureFromSurface(mRenderer, surf);
        SDL_FreeSurface(surf);
        if (!tex)
        {
            SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
            return nullptr;
        }

        mTextures.emplace(fileName.c_str(), tex);
    }
    return tex;
}

void Game::Shutdown()
{

    // mWindow ��ü ����.
    SDL_DestroyWindow(mWindow);
    SDL_DestroyRenderer(mRenderer);
    SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
    // ���͸� ���� ���̶�� ��� ���Ϳ� ���͸� �߰�
    if (mUpdatingActors)
    {
        mPendingActors.emplace_back(actor);
    }
    else
    {
        mActors.emplace_back(actor);
    }
}

void Game::RemoveActor(Actor* actor)
{
    // Actor�� ������ 2���� vector actor�� Ž�� �� �����Ѵٸ� ����
    // mActors vector�� ������ ��� ����
    auto iter = find(mActors.begin(), mActors.end(), actor);
    if (iter != mActors.end()) {
        std::iter_swap(iter, mActors.end()-1);
        mActors.pop_back();
    }

    // mPendingActors vector�� ������ ��� ����
    iter = find(mPendingActors.begin(), mPendingActors.end(), actor);
    if (iter != mPendingActors.end()) {
        std::iter_swap(iter, mPendingActors.end()-1);
        mPendingActors.pop_back();
    }

}