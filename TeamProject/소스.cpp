#include <SDL.h>
#include <iostream>
#include <vector>
#include "SDL_ttf.h"

// ������ ũ�� ����
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// �� ����
const int BALL_SIZE = 10;
float ballX = WINDOW_WIDTH / 2;
float ballY = WINDOW_HEIGHT / 2;
float ballVelX = 5.0f; // ���� �̵� �ӵ� ����
float ballVelY = 5.0f;

// �е� ����
const int PADDLE_WIDTH = 100;
const int PADDLE_HEIGHT = 20;
const int PADDLE_Y = WINDOW_HEIGHT - 50;
float paddleX = WINDOW_WIDTH / 2 - PADDLE_WIDTH / 2;
const float PADDLE_SPEED = 5.0f; // �е��� �̵� �ӵ� ����

// ��� ����
const int BLOCK_ROWS = 5;
const int BLOCK_COLUMNS = 10;
const int BLOCK_WIDTH = 60;
const int BLOCK_HEIGHT = 20;
std::vector<SDL_Rect> blocks;

// ���� ���� ����
int lives = 5;
bool gameRunning = true;

void resetBall() {
    ballX = WINDOW_WIDTH / 2;
    ballY = WINDOW_HEIGHT / 2;
    ballVelX = 5.0f;
    ballVelY = 5.0f;
}

void initBlocks() {
    blocks.clear();
    for (int i = 0; i < BLOCK_ROWS; ++i) {
        for (int j = 0; j < BLOCK_COLUMNS; ++j) {
            SDL_Rect block;
            block.x = j * (BLOCK_WIDTH + 10) + 35;
            block.y = i * (BLOCK_HEIGHT + 10) + 50;
            block.w = BLOCK_WIDTH;
            block.h = BLOCK_HEIGHT;
            blocks.push_back(block);
        }
    }
}

bool checkButtonClick(SDL_Rect button, int mouseX, int mouseY) {
    return mouseX >= button.x && mouseX <= button.x + button.w &&
        mouseY >= button.y && mouseY <= button.y + button.h;
}

void renderText(SDL_Renderer* renderer, const char* text, int x, int y, TTF_Font* font, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect rect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// renderGameOver �Լ����� �ؽ�Ʈ �������� �߰��� ����
void renderGameOver(SDL_Renderer* renderer, bool* restartGame, TTF_Font* font, SDL_Rect* restartBtn, SDL_Rect* quitBtn) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect rect = { WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 };
    SDL_RenderFillRect(renderer, &rect);

    // "GAME OVER" �ؽ�Ʈ ������
    SDL_Color textColor = { 255, 255, 255, 255 };
    renderText(renderer, "GAME OVER", WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT / 4 + 20, font, textColor);

    // �ٽ��ϱ� ��ư
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, restartBtn);
    // "�ٽ��ϱ�" �ؽ�Ʈ ������
    renderText(renderer, "�ٽ��ϱ�", WINDOW_WIDTH / 2 - 40, WINDOW_HEIGHT / 2 - 40, font, textColor);

    // ������ ��ư
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, quitBtn);
    // "������" �ؽ�Ʈ ������
    renderText(renderer, "������", WINDOW_WIDTH / 2 - 30, WINDOW_HEIGHT / 2 + 30, font, textColor);

    SDL_RenderPresent(renderer);

    SDL_Event event;
    while (SDL_WaitEvent(&event)) {
        if (event.type == SDL_QUIT) {
            *restartGame = false;
            return;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX = event.button.x;
            int mouseY = event.button.y;

            if (checkButtonClick(*restartBtn, mouseX, mouseY)) {
                *restartGame = true;
                return;
            }
            else if (checkButtonClick(*quitBtn, mouseX, mouseY)) {
                *restartGame = false;
                return;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    // SDL_ttf ���̺귯�� �ʱ�ȭ
    if (TTF_Init() == -1) {
        std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
        return 1;
    }

    // ��Ʈ �ε�
    TTF_Font* font = TTF_OpenFont("path/to/your/font.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Brick Breaker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool restartGame = true;
    SDL_Rect restartButton = { 0, 0, 0, 0 }; // �ٽ��ϱ� ��ư �ʱ�ȭ
    SDL_Rect quitButton = { 0, 0, 0, 0 };    // ������ ��ư �ʱ�ȭ
    while (restartGame) {
        initBlocks();
        resetBall();
        lives = 5;
        gameRunning = true;

        SDL_Event event;

        while (gameRunning) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    gameRunning = false;
                    restartGame = false;
                }
                // ���� ���� ���°� �ƴϸ鼭 ���콺 ��ư�� ������ ��
                else if (event.type == SDL_MOUSEBUTTONDOWN && !restartGame) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;

                    if (checkButtonClick(restartButton, mouseX, mouseY)) {
                        restartGame = true;
                    }
                    else if (checkButtonClick(quitButton, mouseX, mouseY)) {
                        restartGame = false;
                        gameRunning = false; // ����
                    }
                }
            }

            // ������ �ڵ�� ���⿡ �־�� �մϴ�.

            SDL_Delay(16); // 60 fps
        }

        // ���� ���� ȭ�� ������
        renderGameOver(renderer, &restartGame, font, &restartButton, &quitButton);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}