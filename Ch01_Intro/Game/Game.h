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

};