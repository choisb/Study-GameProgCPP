#include "Game.h"
#include <iostream>
using namespace std;

Game::Game()
{
    mBallPos.x = 512;
    mBallPos.y = 384;

    mPaddle1_Pos.x = 20;
    mPaddle1_Pos.y = 384;

    mPaddle2_Pos.x = 1004;
    mPaddle2_Pos.y = 384;

    mTicksCount = 0;

    mBallVel.x = -300.0f;
    mBallVel.y = 205.0f;
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

    mRenderer = SDL_CreateRenderer(
        mWindow,    // �������� ���� ������ ������
        -1,         // ����� �׷���ī�� ����. -1�� ��� SDL�� �˾Ƽ� ���� 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC    // �÷��� ��.����ȭ ������ ��� ���ο� ���� ����ȭ�� Ȱ��ȭ ���� ����.
        );

    return true;
}

void Game::Shutdown()
{
    // mWindow ��ü ����.
    SDL_DestroyWindow(mWindow);
    SDL_DestroyRenderer(mRenderer);
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

    mPaddle1_Dir = 0;
    if (state[SDL_SCANCODE_W])
    {
        mPaddle1_Dir -= 1;
    }
    if (state[SDL_SCANCODE_S])
    {
        mPaddle1_Dir += 1;
    }

    mPaddle2_Dir = 0;
    if (state[SDL_SCANCODE_UP])
    {
        mPaddle2_Dir -= 1;
    }
    if (state[SDL_SCANCODE_DOWN])
    {
        mPaddle2_Dir += 1;
    }
}
void Game::UpdateGame()
{
    // ������ ������ ���ķ� 16ms�� ����� �� ���� ���
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
        ;

    // ��Ÿ �ð��� ���� ������ ƽ���� ���� ������ ƽ���� ����.
    // (���� �� ������ ��ȯ)
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    // �ִ� ��Ÿ �ð������� ����
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }

    if (mPaddle1_Dir != 0)
    {
        mPaddle1_Pos.y += mPaddle1_Dir * 300.0f * deltaTime;
        cout <<"mPaddle1_Dir: "<< mPaddle1_Dir<< " | mPaddle1_Pos.y: " << mPaddle1_Pos.y << endl;
        
        // �е��� ȭ�� ������ ���Ƴ����� ����
        if (mPaddle1_Pos.y < (paddleH / 2.0f + thickness))
        {
            mPaddle1_Pos.y = paddleH / 2.0f + thickness;
        }
        else if (mPaddle1_Pos.y > (768.0f - paddleH / 2.0f - thickness))
        {
            mPaddle1_Pos.y = 768.0f - paddleH / 2.0f - thickness;
        }
    }

    if (mPaddle2_Dir != 0)
    {
        mPaddle2_Pos.y += mPaddle2_Dir * 300.0f * deltaTime;
        cout << "mPaddle2_Dir: " << mPaddle2_Dir << " | mPaddle2_Pos.y: " << mPaddle2_Pos.y << endl;

        // �е��� ȭ�� ������ ���Ƴ����� ����
        if (mPaddle2_Pos.y < (paddleH / 2.0f + thickness))
        {
            mPaddle2_Pos.y = paddleH / 2.0f + thickness;
        }
        else if (mPaddle2_Pos.y > (768.0f - paddleH / 2.0f - thickness))
        {
            mPaddle2_Pos.y = 768.0f - paddleH / 2.0f - thickness;
        }
    }

    mBallPos.x += mBallVel.x * deltaTime;
    mBallPos.y += mBallVel.y * deltaTime;

    if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
    {
        mBallVel.y *= -1;
    }
    else if (mBallPos.y >= 768.0f - thickness && mBallVel.y > 0.0f)
    {
        mBallVel.y *= -1;
    }
    

    float diff;
    if (mBallPos.y > mPaddle1_Pos.y)
        diff = mBallPos.y - mPaddle1_Pos.y;
    else
        diff = mPaddle1_Pos.y - mBallPos.y;
    
    if (
        // y ���� ����� �۰�
        diff <= paddleH / 2.0f &&
        // ���� �ùٸ� x ���� ���� �ְ�
        mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
        // ���� �������� �̵��ϰ� �ִٸ�
        mBallVel.x < 0.0f)
    {
        mBallVel.x *= -1.0f;
    }
    
    if (mBallPos.y > mPaddle2_Pos.y)
        diff = mBallPos.y - mPaddle2_Pos.y;
    else
        diff = mPaddle2_Pos.y - mBallPos.y;

    if (
        // y ���� ����� �۰�
        diff <= paddleH / 2.0f &&
        // ���� �ùٸ� x ���� ���� �ְ�
        mBallPos.x <= 1024.0f - thickness && mBallPos.x >= 1004.0f - thickness &&
        // ���� �������� �̵��ϰ� �ִٸ�
        mBallVel.x > 0.0f)
    {
        mBallVel.x *= -1.0f;
    }
    // ���� �������� ���� ƽ���� ���
    mTicksCount = SDL_GetTicks();

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

    // ��ܺ�
    SDL_Rect tWall{
    0,          // ���� ��� x
    0,          // ���� ��� y
    1024,       // �ʺ�
    thickness   // ����
    };

    // �ϴ� ��
    SDL_Rect bWall{
        0,
        768 - thickness,
        1024,
        thickness
    };

    // ���� ��
    SDL_Rect rWall{
        1024 - thickness,
        0,
        thickness,
        768
    };

    // ��
    SDL_Rect ball{
        static_cast<int>(mBallPos.x - thickness/2),
        static_cast<int>(mBallPos.y - thickness/2),
        thickness,
        thickness
    };
    
    // �е�
    SDL_Rect paddle1{
        static_cast<int>(mPaddle1_Pos.x - thickness / 2),
        static_cast<int>(mPaddle1_Pos.y - paddleH / 2),
        thickness,
        paddleH
    };

    SDL_Rect paddle2{
        static_cast<int>(mPaddle2_Pos.x - thickness / 2),
        static_cast<int>(mPaddle2_Pos.y - paddleH / 2),
        thickness,
        paddleH
    };

    SDL_RenderFillRect(mRenderer, &tWall);
    SDL_RenderFillRect(mRenderer, &bWall);
    SDL_RenderFillRect(mRenderer, &ball);
    SDL_RenderFillRect(mRenderer, &paddle1);
    SDL_RenderFillRect(mRenderer, &paddle2);


    /////////////////////////////////

    // ���� ���ۿ� �ĸ� ���� ��ȯ
    SDL_RenderPresent(mRenderer);

}