#include "raylib.h"
#include <vector>
#include <algorithm>
#include <string>

// -------------------------
// ���� ���� ����
// -------------------------
enum GameState {
    GAME_START,
    GAME_PLAYING,  // ���� ���� ��
    GAME_OVER      // ���� ���� ����
};

// -------------------------
// �÷��̾� ����ü ����
// -------------------------
struct Player {
    Vector2 position;  // ��ġ
    Vector2 size;      // ũ��
    float speed;       // �̵� �ӵ�
    int lives;         // ���� ���� ��
    int maxLives;      // �ִ� ���� ��
};

// -------------------------
// ��ֹ� ����ü ����
// -------------------------
struct Obstacle {
    Vector2 position;
    Vector2 size;
    float speed;

    void Update(float deltaTime) {
        position.y += speed * deltaTime;  // �Ʒ��� �̵�
    }

    bool IsOffScreen(int screenHeight) const {
        return position.y > screenHeight; // ȭ�� �Ʒ��� ������� �˻�
    }
};

// -------------------------
// ���� ������ ����ü ����
// -------------------------
struct LifeItem {
    Vector2 position;
    Vector2 size;
    float speed;

    void Update(float deltaTime) {
        position.y += speed * deltaTime;  // �Ʒ��� �̵�
    }

    bool IsOffScreen(int screenHeight) const {
        return position.y > screenHeight; // ȭ�� �Ʒ��� ������� �˻�
    }
};

// -------------------------
// ���� ����
// -------------------------
const int screenWidth = 800;
const int screenHeight = 450;

GameState gameState;
Player player;
std::vector<Obstacle> obstacles;
std::vector<LifeItem> lifeItems;

float obstacleSpawnTimer = 0.0f;
float obstacleSpawnRate = 1.2f;  // ��ֹ� ���� ���� (��)

float lifeItemSpawnTimer = 0.0f;
float lifeItemSpawnRate = 10.0f; // ���� ������ ���� ���� (��)

float elapsedTime = 0.0f;
float lastScoreTime = 0.0f;

int score = 0;
int finalScore = 0;

int currentStage = 1;
const int scorePerStage = 5; // n������ �������� ����

// -------------------------
// ���� �ʱ�ȭ �Լ�
// -------------------------
void InitGame() {
    gameState = GAME_START;

    player.position = { screenWidth / 2.0f - 20, screenHeight - 60 };
    player.size = { 40, 40 };
    player.speed = 300.0f;
    player.lives = 3;  // ���� ���� �� ���� �κ�
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
// �������� ��� �˻�
// -------------------------
void CheckStageIncrease() {
    int requiredScoreForNextStage = currentStage * scorePerStage;
    if (score >= requiredScoreForNextStage) {
        currentStage++;
        TraceLog(LOG_INFO, TextFormat("Stage Increased! Current Stage: %i", currentStage));
    }
}

// -------------------------
// �浹 �˻� �Լ�
// -------------------------
void CheckCollisions() {
    Rectangle playerRect = { player.position.x, player.position.y, player.size.x, player.size.y };

    // ��ֹ� �浹 �˻� �� ����
    obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [&](const Obstacle& obs) {
        Rectangle rect = { obs.position.x, obs.position.y, obs.size.x, obs.size.y };
        if (CheckCollisionRecs(playerRect, rect)) {
            player.lives--;
            if (player.lives < 0) { //������ 0�� �� ��ֹ��� �� �� �� ������ ���ӿ���
                gameState = GAME_OVER;
                finalScore = score;
            }
            return true; // �浹 �� ����
        }
        return obs.IsOffScreen(screenHeight); // ȭ�� ���̸� ����
        }), obstacles.end());

    // ���� ������ �浹 �˻� �� ����
    lifeItems.erase(std::remove_if(lifeItems.begin(), lifeItems.end(), [&](const LifeItem& item) {
        Rectangle rect = { item.position.x, item.position.y, item.size.x, item.size.y };
        if (CheckCollisionRecs(playerRect, rect)) {
            player.lives++;
            if (player.lives > player.maxLives) player.lives = player.maxLives;
            return true; // ȹ�� �� ����
        }
        return item.IsOffScreen(screenHeight); // ȭ�� ���̸� ����
        }), lifeItems.end());
}

// -------------------------
// ���� �Լ�
// -------------------------
int main() {
    InitWindow(screenWidth, screenHeight, "Raylib Game");
    SetTargetFPS(60);

    // ���� ����
    Rectangle ground = { 0, screenHeight - 20, (float)screenWidth, 20 };

    InitGame();

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        // ---------------------
        // �Է� ó��
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

            // �÷��̾ ȭ�� ������ ������ �ʵ��� ����
            if (player.position.x < 0) player.position.x = 0;
            if (player.position.x > screenWidth - player.size.x) player.position.x = screenWidth - player.size.x;
        }
        else if (gameState == GAME_OVER) {
            if (IsKeyPressed(KEY_SPACE)) InitGame();
        }

        // ---------------------
        // ���� ���� ������Ʈ
        // ---------------------
        if (gameState == GAME_PLAYING) {
            elapsedTime += deltaTime;

            // ���� �ð����� ���� ����
            if (elapsedTime - lastScoreTime >= 5.0f) { //5.0�� ���� ���� 1�� ����(�ش�κ� ���� ����)
                score++;
                lastScoreTime = elapsedTime;
                CheckStageIncrease();
            }

            // ��ֹ� ���� ���� ��� (���������� �ö󰥼��� ������)
            float currentObstacleRate = obstacleSpawnRate - (currentStage - 1) * 0.1f;
            if (currentObstacleRate < 0.5f) currentObstacleRate = 0.5f;

            // ��ֹ� ����
            obstacleSpawnTimer += deltaTime;
            if (obstacleSpawnTimer >= currentObstacleRate) {
                Obstacle obs;
                //�پ��� ũ���� ��ֹ� ����
                float minSize = 30.0f;
                float maxSize = 40.0f + currentStage * 5.0f;
                if (maxSize > 100.0f) maxSize = 100.0f;

                float sizeX = GetRandomValue((int)minSize, (int)maxSize);
                float sizeY = GetRandomValue((int)minSize, (int)maxSize);
                obs.size = { sizeX, sizeY };

                obs.position = { (float)GetRandomValue(0, screenWidth - (int)obs.size.x), -obs.size.y };

                // ���������� ���� �پ��� �ӵ��� ���� (����/���� ȥ��)
                float minSpeed = 80.0f + currentStage * 10.0f;
                float maxSpeed = 150.0f + currentStage * 20.0f;
                if (maxSpeed > 500.0f) maxSpeed = 500.0f;

                obs.speed = GetRandomValue((int)minSpeed, (int)maxSpeed);

                obstacles.push_back(obs);
                obstacleSpawnTimer = 0.0f;
            }

            // ���� ������ ����
            lifeItemSpawnTimer += deltaTime;
            if (lifeItemSpawnTimer >= lifeItemSpawnRate) {
                LifeItem item;
                item.size = { 30, 30 };
                item.position = { (float)GetRandomValue(0, screenWidth - (int)item.size.x), -item.size.y };
                item.speed = 120;
                lifeItems.push_back(item);
                lifeItemSpawnTimer = 0.0f;
            }

            // ��� ������Ʈ �̵� ������Ʈ
            for (auto& obs : obstacles) obs.Update(deltaTime);
            for (auto& item : lifeItems) item.Update(deltaTime);

            // �浹 ó��
            CheckCollisions();
        }

        // ---------------------
        // �׸��� ����
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
            // �÷��̾� �� ������Ʈ �׸���
            DrawRectangleV(player.position, player.size, BLUE);

            for (const auto& obs : obstacles)
                DrawRectangleV(obs.position, obs.size, RED);
            for (const auto& item : lifeItems)
                DrawRectangleV(item.position, item.size, GREEN);

            // ���� �׸���
            DrawRectangleRec(ground, DARKGRAY);

            // UI ���� ǥ��
            DrawText(TextFormat("Score: %i", score), 10, 10, 20, WHITE);
            DrawText(TextFormat("Lives: %i", player.lives), 10, 30, 20, WHITE);
            DrawText(TextFormat("Time: %.0f", elapsedTime), 10, 50, 20, WHITE);
            DrawText(TextFormat("Stage: %i", currentStage), screenWidth - 120, 10, 20, RAYWHITE);
            DrawText("Press ESC to Exit", 10, 430, 20, RAYWHITE);
        }
        else if (gameState == GAME_OVER) {
            // ���� ���� ȭ��
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
