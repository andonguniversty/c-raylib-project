#include "raylib.h"
#include <vector>
#include <algorithm>
#include <string>

// -------------------------
// 게임 상태 정의
// -------------------------
enum GameState {
    GAME_START,
    GAME_PLAYING,  // 게임 진행 중
    GAME_OVER      // 게임 종료 상태
};

// -------------------------
// 플레이어 구조체 정의
// -------------------------
struct Player {
    Vector2 position;  // 위치
    Vector2 size;      // 크기
    float speed;       // 이동 속도
    int lives;         // 현재 생명 수
    int maxLives;      // 최대 생명 수
};

// -------------------------
// 장애물 구조체 정의
// -------------------------
struct Obstacle {
    Vector2 position;
    Vector2 size;
    float speed;

    void Update(float deltaTime) {
        position.y += speed * deltaTime;  // 아래로 이동
    }

    bool IsOffScreen(int screenHeight) const {
        return position.y > screenHeight; // 화면 아래로 벗어났는지 검사
    }
};

// -------------------------
// 생명 아이템 구조체 정의
// -------------------------
struct LifeItem {
    Vector2 position;
    Vector2 size;
    float speed;

    void Update(float deltaTime) {
        position.y += speed * deltaTime;  // 아래로 이동
    }

    bool IsOffScreen(int screenHeight) const {
        return position.y > screenHeight; // 화면 아래로 벗어났는지 검사
    }
};

// -------------------------
// 전역 변수
// -------------------------
const int screenWidth = 800;
const int screenHeight = 450;

GameState gameState;
Player player;
std::vector<Obstacle> obstacles;
std::vector<LifeItem> lifeItems;

float obstacleSpawnTimer = 0.0f;
float obstacleSpawnRate = 1.2f;  // 장애물 생성 간격 (초)

float lifeItemSpawnTimer = 0.0f;
float lifeItemSpawnRate = 10.0f; // 생명 아이템 생성 간격 (초)

float elapsedTime = 0.0f;
float lastScoreTime = 0.0f;

int score = 0;
int finalScore = 0;

int currentStage = 1;
const int scorePerStage = 5; // n점마다 스테이지 증가

// -------------------------
// 게임 초기화 함수
// -------------------------
void InitGame() {
    gameState = GAME_START;

    player.position = { screenWidth / 2.0f - 20, screenHeight - 60 };
    player.size = { 40, 40 };
    player.speed = 300.0f;
    player.lives = 3;  // 시작 생명 수 조정 부분
    player.maxLives = 5;

    obstacles.clear();
    lifeItems.clear();

    obstacleSpawnTimer = 0.0f;
    lifeItemSpawnTimer = 0.0f;
    elapsedTime = 0.0f;
    lastScoreTime = 0.0f;

    score = 0;
    finalScore = 0;
    currentStage = 1;
}

// -------------------------
// 스테이지 상승 검사
// -------------------------
void CheckStageIncrease() {
    int requiredScoreForNextStage = currentStage * scorePerStage;
    if (score >= requiredScoreForNextStage) {
        currentStage++;
        TraceLog(LOG_INFO, TextFormat("Stage Increased! Current Stage: %i", currentStage));
    }
}

// -------------------------
// 충돌 검사 함수
// -------------------------
void CheckCollisions() {
    Rectangle playerRect = { player.position.x, player.position.y, player.size.x, player.size.y };

    // 장애물 충돌 검사 및 삭제
    obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [&](const Obstacle& obs) {
        Rectangle rect = { obs.position.x, obs.position.y, obs.size.x, obs.size.y };
        if (CheckCollisionRecs(playerRect, rect)) {
            player.lives--;
            if (player.lives < 0) { //생명이 0일 때 장애물을 한 번 더 맞으면 게임오버
                gameState = GAME_OVER;
                finalScore = score;
            }
            return true; // 충돌 시 제거
        }
        return obs.IsOffScreen(screenHeight); // 화면 밖이면 제거
        }), obstacles.end());

    // 생명 아이템 충돌 검사 및 삭제
    lifeItems.erase(std::remove_if(lifeItems.begin(), lifeItems.end(), [&](const LifeItem& item) {
        Rectangle rect = { item.position.x, item.position.y, item.size.x, item.size.y };
        if (CheckCollisionRecs(playerRect, rect)) {
            player.lives++;
            if (player.lives > player.maxLives) player.lives = player.maxLives;
            return true; // 획득 시 제거
        }
        return item.IsOffScreen(screenHeight); // 화면 밖이면 제거
        }), lifeItems.end());
}

// -------------------------
// 메인 함수
// -------------------------
int main() {
    InitWindow(screenWidth, screenHeight, "Raylib Game");
    SetTargetFPS(60);

    // 지면 정의
    Rectangle ground = { 0, screenHeight - 20, (float)screenWidth, 20 };

    InitGame();

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        // ---------------------
        // 입력 처리
        // ---------------------
        if (IsKeyPressed(KEY_ESCAPE)) break;

        if (gameState == GAME_START) {
            if (IsKeyPressed(KEY_SPACE)) {
                gameState = GAME_PLAYING;
            }
        }
        else if (gameState == GAME_PLAYING) {
            if (IsKeyDown(KEY_LEFT)) player.position.x -= player.speed * deltaTime;
            if (IsKeyDown(KEY_RIGHT)) player.position.x += player.speed * deltaTime;

            // 플레이어가 화면 밖으로 나가지 않도록 제한
            if (player.position.x < 0) player.position.x = 0;
            if (player.position.x > screenWidth - player.size.x) player.position.x = screenWidth - player.size.x;
        }
        else if (gameState == GAME_OVER) {
            if (IsKeyPressed(KEY_SPACE)) InitGame();
        }

        // ---------------------
        // 게임 로직 업데이트
        // ---------------------
        if (gameState == GAME_PLAYING) {
            elapsedTime += deltaTime;

            // 일정 시간마다 점수 증가
            if (elapsedTime - lastScoreTime >= 5.0f) { //5.0초 마다 점수 1씩 증가(해당부분 수정 가능)
                score++;
                lastScoreTime = elapsedTime;
                CheckStageIncrease();
            }

            // 장애물 생성 간격 계산 (스테이지가 올라갈수록 빨라짐)
            float currentObstacleRate = obstacleSpawnRate - (currentStage - 1) * 0.1f;
            if (currentObstacleRate < 0.5f) currentObstacleRate = 0.5f;

            // 장애물 생성
            obstacleSpawnTimer += deltaTime;
            if (obstacleSpawnTimer >= currentObstacleRate) {
                Obstacle obs;
                //다양한 크기의 장애물 생성
                float minSize = 30.0f;
                float maxSize = 40.0f + currentStage * 5.0f;
                if (maxSize > 100.0f) maxSize = 100.0f;

                float sizeX = GetRandomValue((int)minSize, (int)maxSize);
                float sizeY = GetRandomValue((int)minSize, (int)maxSize);
                obs.size = { sizeX, sizeY };

                obs.position = { (float)GetRandomValue(0, screenWidth - (int)obs.size.x), -obs.size.y };

                // 스테이지에 따라 다양한 속도로 생성 (느린/빠른 혼합)
                float minSpeed = 80.0f + currentStage * 10.0f;
                float maxSpeed = 150.0f + currentStage * 20.0f;
                if (maxSpeed > 500.0f) maxSpeed = 500.0f;

                obs.speed = GetRandomValue((int)minSpeed, (int)maxSpeed);

                obstacles.push_back(obs);
                obstacleSpawnTimer = 0.0f;
            }

            // 생명 아이템 생성
            lifeItemSpawnTimer += deltaTime;
            if (lifeItemSpawnTimer >= lifeItemSpawnRate) {
                LifeItem item;
                item.size = { 30, 30 };
                item.position = { (float)GetRandomValue(0, screenWidth - (int)item.size.x), -item.size.y };
                item.speed = 120;
                lifeItems.push_back(item);
                lifeItemSpawnTimer = 0.0f;
            }

            // 모든 오브젝트 이동 업데이트
            for (auto& obs : obstacles) obs.Update(deltaTime);
            for (auto& item : lifeItems) item.Update(deltaTime);

            // 충돌 처리
            CheckCollisions();
        }

        // ---------------------
        // 그리기 시작
        // ---------------------
        BeginDrawing();
        ClearBackground(BLACK);

        if (gameState == GAME_START) {
            const char* titleText = "Box Dodge Game";
            const char* startText = "Press SPACE to Start";
            const char* exitText = "Press ESC to Exit";

            ClearBackground(BLACK);
            DrawText(titleText, screenWidth / 2 - MeasureText(titleText, 40) / 2, screenHeight / 2 - 80, 40, SKYBLUE);
            DrawText(startText, screenWidth / 2 - MeasureText(startText, 20) / 2, screenHeight / 2 - 20, 20, GRAY);
            DrawText(exitText, screenWidth / 2 - MeasureText(exitText, 20) / 2, screenHeight / 2 + 10, 20, GRAY);
        }

        if (gameState == GAME_PLAYING) {
            // 플레이어 및 오브젝트 그리기
            DrawRectangleV(player.position, player.size, BLUE);

            for (const auto& obs : obstacles)
                DrawRectangleV(obs.position, obs.size, RED);
            for (const auto& item : lifeItems)
                DrawRectangleV(item.position, item.size, GREEN);

            // 지면 그리기
            DrawRectangleRec(ground, DARKGRAY);

            // UI 정보 표시
            DrawText(TextFormat("Score: %i", score), 10, 10, 20, WHITE);
            DrawText(TextFormat("Lives: %i", player.lives), 10, 30, 20, WHITE);
            DrawText(TextFormat("Time: %.0f", elapsedTime), 10, 50, 20, WHITE);
            DrawText(TextFormat("Stage: %i", currentStage), screenWidth - 120, 10, 20, RAYWHITE);
            DrawText("Press ESC to Exit", 10, 430, 20, RAYWHITE);
        }
        else if (gameState == GAME_OVER) {
            // 게임 오버 화면
            const char* gameOverText = "Game Over!";
            DrawText(gameOverText, screenWidth / 2 - MeasureText(gameOverText, 40) / 2, screenHeight / 2 - 60, 40, RED);
            DrawText(TextFormat("Final Score: %i", finalScore), screenWidth / 2 - 100, screenHeight / 2, 30, RAYWHITE);
            DrawText(TextFormat("Total Time: %.0fs", elapsedTime), screenWidth / 2 - 100, screenHeight / 2 + 40, 30, RAYWHITE);
            DrawText("Press SPACE to Restart", screenWidth / 2 - 100, screenHeight / 2 + 100, 20, DARKGRAY);
            DrawText("Press ESC to Exit", screenWidth / 2 - 100, screenHeight / 2 + 120, 20, DARKGRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
