#pragma once
#include "SDL/SDL.h"


class Game
{
public:
    Game();
    bool Initialize();  // ���� �ʱ�ȭ
    void RunLoop();     // ������ ������ ������ ���� ���� ����
    void Shutdown();    // ���� ����

private:
    // ���� ������ ���� ���� �Լ�
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

    SDL_Window* mWindow;    // SDL�� ������ window
    bool mIsRunning;        // ������ ��� ����ž� �ϴ����� �Ǵ�.

    SDL_Renderer* mRenderer;

    const int thickness = 15;
    const int paddleH = 100;


    struct Vector2
    {
        float x;
        float y;
    };

    // ��Ÿ Ÿ�� ���ϱ� ���� ����
    Uint32 mTicksCount;

    // ���� �е��� �̵�
    int mPaddle1_Dir;
    int mPaddle2_Dir;
    Vector2 mBallVel;

    // ���� �е��� ��ġ
    Vector2 mPaddle1_Pos;
    Vector2 mPaddle2_Pos;

    Vector2 mBallPos;

};