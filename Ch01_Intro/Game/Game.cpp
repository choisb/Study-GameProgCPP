#include "Game.h"

Game::Game()
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
    "Game Programming in C++ (Chapter 1)", // ������ ����
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

    return true;
}

void Game::Shutdown()
{
    // mWindow ��ü ����.
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void Game::RunLoop()
{
    mIsRunning = true;
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

}
void Game::GenerateOutput()
{

}