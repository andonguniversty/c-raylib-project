네, 요청하신 대로 Raylib 장애물 피하기 게임의 모든 기능 설명과 해당 코드 스니펫을 하나의 마크다운 문서 안에 명확하게 정리하여 보여드리겠습니다.

````markdown
# 🎮 Raylib 장애물 피하기 게임

## 📌 프로젝트 개요
- **팀명**: 9팀
- **팀원**: 전재민(팀장), 김준용, 김무진
- **개발 기간**: 2025년 05월 14일 ~ 2025년 05월 18일 (총 5일)
- **주요 기술**: C++, raylib 라이브러리, 게임 루프, 충돌 감지, 객체 관리 등

## 👥 팀원 역할 분담

| 이름 | 역할 | 주요 담당 업무 |
|------|------|----------------|
| 전재민 | 프로젝트 관리 / 기획 및 설계 | 전체 게임 구조 설계, 핵심 기능 정의, 개발 방향 제시, Git 관리 |
| 김무진 | 개발 | 게임 핵심 기능 구현 (주인공, 장애물, 충돌, 점수), 속도/크기 변화 로직 코딩, raylib 기능 활용 |
| 김준용 | 문서 / 테스트 | 프로젝트 보고서 작성, README 문서 관리, 테스트 계획 수립 및 실행, 버그 리포팅 |

## 🎮 게임 설명

* **조작:** 좌우 방향키로 파란색 플레이어 바를 조작합니다.
* **목표:** 위에서 떨어지는 빨간색 장애물을 피하고, 초록색 생명 박스를 획득하여 생명을 유지하며 최대한 오래 버팁니다.
* **생명:** 게임 시작 시 3개의 생명을 가집니다.
* **장애물 (빨간색 박스):** 닿으면 생명이 1 감소합니다.
* **생명 박스 (초록색 박스):** 획득하면 생명이 1 증가합니다 (최대 생명 제한 있음).
* **게임 오버:** 생명이 0이 되면 게임이 종료되고 게임 오버 화면이 표시됩니다.
* **점수:** 일정 시간마다 점수를 획득하며, 장애물을 피하거나 생명 박스를 획득하는 것으로는 직접적인 점수를 얻지 않습니다.
* **스테이지:** 일정 점수마다 스테이지가 증가하며, 스테이지에 따라 게임 난이도가 상승합니다.

## ✨ 주요 기능

1.  **플레이어 조작:** 좌우 방향키를 사용한 플레이어 이동 및 화면 경계 제한.
2.  **장애물/아이템 관리:** 화면 상단에서 생성되어 하단으로 이동하는 장애물(빨간색) 및 생명 회복 아이템(초록색) 생성, 이동, 화면 밖 제거.
3.  **충돌 감지:** 플레이어와 장애물/아이템 간의 충돌 감지 및 생명 증감 처리.
4.  **생명 시스템:** 초기 생명 설정, 생명 감소 및 증가, 게임 오버 로직.
5.  **점수 시스템:** 시간 경과에 따른 점수 획득 및 총 점수 표시.
6.  **시간 표시:** 현재 경과 시간을 초 단위로 표시.
7.  **스테이지 시스템:** 점수에 따른 스테이지 증가, 장애물 낙하 속도 및 생성 빈도 증가를 통한 난이도 조절.
8.  **게임 상태 관리:** 게임 플레이, 게임 오버 상태 전환 및 각 상태에 맞는 처리.
9.  **UI:** 게임 중 생명/점수/시간/스테이지 표시, 게임 오버 화면 출력.
10. **재시작/종료:** 게임 오버 시 Space 키로 재시작, 모든 상태에서 Esc 키로 종료.

## 🛠️ 구현 구조 및 코드 스니펫 (예시)

이 게임은 Raylib의 메인 루프 구조를 따르며, `GameManager` 클래스 등을 활용하여 게임 상태 및 객체들을 관리합니다.

### 1. 게임 초기화 및 메인 루프 구성

Raylib 창을 생성하고 게임의 기본 상태를 초기화합니다. `InitGame()`, `UpdateGame()`, `DrawGame()` 함수를 게임 루프 내에서 호출하여 게임 로직 업데이트 및 화면 그리기를 수행합니다.

```cpp
// 예시: main 함수 일부
#include "raylib.h" // raylib 헤더 포함
#include <vector>   // std::vector 사용
#include <string>   // TextFormat 등 사용
#include <algorithm> // std::remove_if 사용

// 게임 상태 열거형 (GameManager 클래스 내에 정의될 수 있음)
enum GameState {
    GAME_PLAYING,
    GAME_OVER
};

// 임시 Player, Obstacle, LifeItem 구조체/클래스 선언 (실제 구현은 더 복잡할 수 있음)
struct Player {
    Vector2 position = { 0, 0 };
    Vector2 size = { 0, 0 };
    Color color = BLUE;
    float speed = 300.0f; // 이동 속도
    int lives = 3;
    int maxLives = 5; // 최대 생명
    // TODO: 무적 시간 관련 멤버 추가 가능
    void Update(float deltaTime, int screenWidth, int screenHeight);
    void Draw();
    // void HandleInput(); // GameManager에서 처리하거나 Player 내부에서 처리
};

struct Obstacle {
    Vector2 position = { 0, 0 };
    Vector2 size = { 0, 0 };
    Color color = RED;
    float speed = 200.0f; // 낙하 속도
    void Update(float deltaTime);
    void Draw();
    bool IsOffScreen(int screenHeight);
};

struct LifeItem {
    Vector2 position = { 0, 0 };
    Vector2 size = { 0, 0 };
    Color color = GREEN;
    float speed = 150.0f; // 낙하 속도 (장애물보다 약간 느리게)
    void Update(float deltaTime);
    void Draw();
    bool IsOffScreen(int screenHeight);
};

// 임시 GameManager 클래스 선언
class GameManager {
public:
    GameState gameState = GAME_PLAYING;
    Player player;
    std::vector<Obstacle> obstacles;
    std::vector<LifeItem> lifeItems;

    int screenWidth;
    int screenHeight;

    int score = 0;
    int finalScore = 0; // 게임 오버 시 최종 점수
    float elapsedTime = 0.0f;
    float lastScoreTime = 0.0f; // 마지막으로 점수 획득한 시간

    int currentStage = 1;
    int scorePerStage = 10; // 스테이지 증가에 필요한 점수 기준

    float baseObstacleSpeed = 200.0f; // 기본 장애물 속도
    float speedIncreasePerStage = 50.0f; // 스테이지당 증가 속도
    float baseSpawnRate = 2.0f; // 기본 생성 간격 (초)
    float spawnRateDecreasePerStage = 0.2f; // 스테이지당 생성 간격 감소량

    float timeSinceLastObstacle = 0.0f; // 마지막 장애물 생성 후 경과 시간
    float timeSinceLastLifeItem = 0.0f; // 마지막 생명 아이템 생성 후 경과 시간
    float lifeItemSpawnRate = 10.0f; // 생명 아이템 생성 간격 (초)

    bool shouldClose = false; // 게임 종료 플래그

    GameManager() {}

    void InitGame(int width, int height) {
        screenWidth = width;
        screenHeight = height;

        gameState = GAME_PLAYING;
        score = 0;
        finalScore = 0;
        elapsedTime = 0.0f;
        lastScoreTime = 0.0f;
        currentStage = 1;
        timeSinceLastObstacle = 0.0f;
        timeSinceLastLifeItem = 0.0f;

        // 플레이어 초기화
        player.size = { 80, 20 };
        player.position = { (float)screenWidth / 2 - player.size.x / 2, (float)screenHeight - player.size.y - 10 };
        player.lives = 3;
        player.maxLives = 5;
        player.speed = 300.0f;

        // 장애물 및 아이템 목록 초기화
        obstacles.clear();
        lifeItems.clear();

        shouldClose = false;
    }

    void HandleInput() {
        // Esc 키는 어떤 상태에서든 게임 종료
        if (IsKeyPressed(KEY_ESCAPE)) {
            shouldClose = true;
        }

        if (gameState == GAME_PLAYING) {
            // 플레이 중 상태에서의 입력 (플레이어 이동)
            // 플레이어 클래스의 HandleInput 함수를 호출하거나 여기서 직접 처리
            if (IsKeyDown(KEY_RIGHT)) player.position.x += player.speed * GetFrameTime();
            if (IsKeyDown(KEY_LEFT)) player.position.x -= player.speed * GetFrameTime();

            // 화면 경계 체크 (Update에서 처리하는 것이 일반적이지만 입력과 함께 처리 가능)
            if (player.position.x < 0) player.position.x = 0;
            if (player.position.x > screenWidth - player.size.x) player.position.x = screenWidth - player.size.x;

        } else if (gameState == GAME_OVER) {
            // 게임 오버 상태에서의 입력 (재시작)
            if (IsKeyPressed(KEY_SPACE)) {
                InitGame(screenWidth, screenHeight); // 게임 재시작
            }
        }
    }

    void UpdateGame(float deltaTime) {
        if (gameState == GAME_PLAYING) {
            elapsedTime += deltaTime; // 경과 시간 누적

            // 1초마다 점수 획득
            if (elapsedTime - lastScoreTime >= 1.0f) { // 1초 간격으로 체크
                score++;
                lastScoreTime = elapsedTime; // 마지막 점수 획득 시간 업데이트

                // 스테이지 증가 조건 체크 및 처리
                CheckStageIncrease();
            }

            // 플레이어 업데이트 (경계 체크 등)
            player.Update(deltaTime, screenWidth, screenHeight);

            // 장애물 생성
            timeSinceLastObstacle += deltaTime;
            // 현재 스테이지에 맞는 생성 간격 계산
            float currentSpawnRate = baseSpawnRate - (currentStage - 1) * spawnRateDecreasePerStage;
            if (currentSpawnRate < 0.5f) currentSpawnRate = 0.5f; // 최소 제한

            if (timeSinceLastObstacle >= currentSpawnRate) {
                Obstacle newObstacle;
                newObstacle.size = { 30, 30 }; // 예시 크기
                newObstacle.position = { (float)GetRandomValue(0, screenWidth - (int)newObstacle.size.x), -newObstacle.size.y }; // 화면 상단 임의 위치
                newObstacle.speed = baseObstacleSpeed + (currentStage - 1) * speedIncreasePerStage; // 현재 스테이지 속도 적용
                obstacles.push_back(newObstacle);
                timeSinceLastObstacle = 0.0f; // 타이머 리셋
            }

            // 생명 아이템 생성
            timeSinceLastLifeItem += deltaTime;
             if (timeSinceLastLifeItem >= lifeItemSpawnRate) {
                LifeItem newLifeItem;
                newLifeItem.size = { 25, 25 }; // 예시 크기
                newLifeItem.position = { (float)GetRandomValue(0, screenWidth - (int)newLifeItem.size.x), -newLifeItem.size.y }; // 화면 상단 임의 위치
                newLifeItem.speed = player.speed * 0.5f; // 플레이어 속도의 절반 (예시)
                lifeItems.push_back(newLifeItem);
                timeSinceLastLifeItem = 0.0f; // 타이머 리셋
            }


            // 장애물 업데이트 및 화면 밖 제거
            for (auto& obs : obstacles) {
                obs.Update(deltaTime);
            }
            obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [&](const Obstacle& o){
                return o.IsOffScreen(screenHeight);
            }), obstacles.end());

            // 생명 아이템 업데이트 및 화면 밖 제거
             for (auto& item : lifeItems) {
                item.Update(deltaTime);
            }
            lifeItems.erase(std::remove_if(lifeItems.begin(), lifeItems.end(), [&](const LifeItem& i){
                return i.IsOffScreen(screenHeight);
            }), lifeItems.end());


            // 충돌 체크
            CheckCollisions();

            // 게임 오버 조건 체크는 CheckCollisions에서 수행됨

        } else if (gameState == GAME_OVER) {
            // 게임 오버 상태에서는 업데이트할 내용 없음 (입력만 처리)
        }
    }

    void DrawGame() {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (gameState == GAME_PLAYING) {
            // 플레이어 그리기
            player.Draw();

            // 장애물 그리기
            for (const auto& obs : obstacles) {
                obs.Draw();
            }

            // 생명 아이템 그리기
             for (const auto& item : lifeItems) {
                item.Draw();
            }

            // UI 표시 (점수, 생명, 시간, 스테이지)
            DrawText(TextFormat("Score: %i", score), 10, 10, 20, BLACK);
            DrawText(TextFormat("Lives: %i", player.lives), 10, 30, 20, BLACK);
            DrawText(TextFormat("Time: %.0f", elapsedTime), 10, 50, 20, BLACK); // 초 단위 정수로 표시
            DrawText(TextFormat("Stage: %i", currentStage), screenWidth - 100, 10, 20, BLACK);

        } else if (gameState == GAME_OVER) {
            const char* gameOverText = "Game Over!";
            DrawText(gameOverText, screenWidth/2 - MeasureText(gameOverText, 40)/2, screenHeight/2 - 40, 40, RED); // 텍스트 위치 조정
            DrawText(TextFormat("Final Score: %i", finalScore), screenWidth/2 - MeasureText(TextFormat("Final Score: %i", finalScore), 30)/2, screenHeight/2 + 0, 30, BLACK); // 텍스트 크기 조정
            DrawText("Press SPACE to Restart", screenWidth/2 - MeasureText("Press SPACE to Restart", 20)/2, screenHeight/2 + 40, 20, DARKGRAY);
            DrawText("Press ESC to Exit", screenWidth/2 - MeasureText("Press ESC to Exit", 20)/2, screenHeight/2 + 60, 20, DARKGRAY);
        }

        EndDrawing();
    }

    void CheckCollisions() {
        // 게임 오버 상태에서는 충돌 체크를 하지 않음
        if (gameState != GAME_PLAYING) return;

        // 주인공의 충돌 영역 (사각형)
        Rectangle playerRect = { player.position.x, player.position.y, player.size.x, player.size.y };

        // --- 장애물과의 충돌 체크 (생명 감소) ---
        // 장애물 목록을 순회하며 충돌 체크 및 제거
        obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [&](const Obstacle& obs){
            Rectangle obstacleRect = { obs.position.x, obs.position.y, obs.size.x, obs.size.y };
            bool collided = CheckCollisionRecs(playerRect, obstacleRect);

            if (collided) {
                // TODO: 플레이어 무적 상태가 아닐 때만 생명 감소
                // if (!player.isInvincible) {
                    player.lives--; // Player 클래스에 lives 멤버가 있다고 가정
                    TraceLog(LOG_INFO, TextFormat("Hit Obstacle! Lives: %i", player.lives));
                    // TODO: 플레이어에게 짧은 무적 시간 부여 (무적 타이머 시작 등)
                    return true; // 충돌한 장애물 제거
                // }
            }
            return obs.IsOffScreen(screenHeight); // 화면 밖으로 나간 장애물 제거
        }), obstacles.end());


        // --- 생명 회복 아이템과의 충돌 체크 (생명 증가) ---
        // 생명 회복 아이템 목록을 순회하며 충돌 체크 및 제거
        lifeItems.erase(std::remove_if(lifeItems.begin(), lifeItems.end(), [&](const LifeItem& item){
             Rectangle lifeItemRect = { item.position.x, item.position.y, item.size.x, item.size.y };
             bool collided = CheckCollisionRecs(playerRect, lifeItemRect);

            if (collided) {
                // 충돌 발생 시 생명 증가 (최대 생명 제한)
                player.lives++; // 생명 증가
                 if (player.lives > player.maxLives) {
                    player.lives = player.maxLives; // 최대 생명으로 제한
                }
                TraceLog(LOG_INFO, TextFormat("Got Life Item! Lives: %i", player.lives));
                return true; // 충돌한 아이템 제거
            }
            return item.IsOffScreen(screenHeight); // 화면 밖 아이템 제거
        }), lifeItems.end());

        // 모든 충돌 체크 및 생명 상태 확인 후 게임 오버 상태 전환
        if (player.lives <= 0) {
            gameState = GAME_OVER;
            finalScore = score; // 게임 오버 시 최종 점수 저장
            TraceLog(LOG_INFO, TextFormat("Game Over! Final Score: %i", finalScore));
            // TODO: 게임 오버 상태로 전환 시 필요한 추가 작업 (예: 타이머 중지)
        }
    }

    void CheckStageIncrease() {
        // 다음 스테이지에 필요한 점수 계산
        int requiredScoreForNextStage = currentStage * scorePerStage;

        if (score >= requiredScoreForNextStage) {
            currentStage++;
            TraceLog(LOG_INFO, TextFormat("Stage Increased! Current Stage: %i", currentStage));

            // TODO: 스테이지 증가에 따른 속도/크기 변화 적용 (장애물 생성 시 반영하거나 현재 존재하는 장애물 업데이트)
            // 현재 존재하는 장애물들의 속도 업데이트 (예시)
            float newObstacleSpeed = baseObstacleSpeed + (currentStage - 1) * speedIncreasePerStage;
             for (auto& obs : obstacles) { obs.speed = newObstacleSpeed; }

            // 스테이지 증가에 따른 생성 빈도 증가 (생성 간격 감소)는 UpdateGame에서 timeSinceLastObstacle 로직에 의해 자동 적용됨
        }
    }

    bool ShouldClose() const { return shouldClose || WindowShouldClose(); }
};

// 임시 Player, Obstacle, LifeItem 함수 정의 (간단한 구현)
void Player::Update(float deltaTime, int screenWidth, int screenHeight) {
    // 입력 처리는 GameManager::HandleInput에서 이미 수행된다고 가정하거나,
    // Player::HandleInput() 함수를 별도로 호출하여 처리합니다.
    // 화면 경계 체크
    if (position.x < 0) position.x = 0;
    if (position.x > screenWidth - size.x) position.x = screenWidth - size.x;
    // TODO: 무적 시간 업데이트 로직 추가
}

void Player::Draw() {
    DrawRectangleV(position, size, color);
    // TODO: 생명 표시 등을 Player 클래스 내부 또는 GameManager에서 처리
}

void Obstacle::Update(float deltaTime) {
    position.y += speed * deltaTime;
}

void Obstacle::Draw() {
    DrawRectangleV(position, size, color);
}

bool Obstacle::IsOffScreen(int screenHeight) {
    return position.y > screenHeight;
}

void LifeItem::Update(float deltaTime) {
    position.y += speed * deltaTime;
}

void LifeItem::Draw() {
    DrawRectangleV(position, size, color);
}

bool LifeItem::IsOffScreen(int screenHeight) {
    return position.y > screenHeight;
}


// main 함수 구현
int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Raylib Avoid Game");
    SetTargetFPS(60); // 프레임 속도 설정

    GameManager game;
    game.InitGame(screenWidth, screenHeight); // 게임 초기화

    while (!game.ShouldClose()) { // Esc 키 또는 창 닫기 버튼 클릭 시 종료
        game.HandleInput();              // 입력 처리
        game.UpdateGame(GetFrameTime()); // 게임 로직 업데이트
        game.DrawGame();                 // 게임 화면 그리기
    }

    CloseWindow(); // 창 닫기
    return 0;
}

````

위 코드는 게임의 핵심 구조와 기능을 보여주는 예시입니다. 실제 프로젝트에서는 각 클래스(Player, Obstacle, LifeItem, GameManager)를 별도의 헤더 파일(`.h`) 및 소스 파일(`.cpp`)로 분리하여 관리하는 것이 일반적입니다. 또한, Raylib 기능을 사용하기 위해 `#include "raylib.h"`가 필요하며, `GetFrameTime()` 함수를 사용하여 프레임 간 경과 시간을 얻어 객체 이동 및 타이머 업데이트에 사용해야 합니다.

### 2\. 주인공 이동 구현

`Player` 클래스의 `Update` 함수에서 좌우 방향키 입력을 받아 `position.x` 값을 변경하고, 화면 경계를 벗어나지 않도록 제한합니다.

```cpp
// Player 클래스 Update 함수 (위 main.cpp 내에 포함되어 있음)
void Player::Update(float deltaTime, int screenWidth, int screenHeight) {
    // 좌우 화살표 키 입력에 따른 수평 위치 변경은 GameManager::HandleInput에서 처리하거나 여기서 처리
    // if (IsKeyDown(KEY_RIGHT)) position.x += speed * deltaTime;
    // if (IsKeyDown(KEY_LEFT))  position.x -= speed * deltaTime;

    // 화면 좌우 경계 체크
    if (position.x < 0) position.x = 0;
    if (position.x > screenWidth - size.x) position.x = screenWidth - size.x;

    // TODO: 무적 시간 처리 로직 추가될 수 있음 (타이머 감소 등)
}

// Player 클래스 Draw 함수 (위 main.cpp 내에 포함되어 있음)
void Player::Draw() {
    DrawRectangleV(position, size, color);
    // TODO: 생명 표시 등을 플레이어 클래스 내부 또는 GameManager에서 처리 (현재는 GameManager에서 처리)
}
```

### 3\. 장애물 및 생명 박스 생성 및 이동

`Obstacle` 및 `LifeItem` 클래스를 정의하고, `GameManager::UpdateGame` 함수 내에서 일정 시간 간격으로 객체를 생성하여 목록에 추가합니다. 각 객체의 `Update` 함수에서 `position.y` 값을 증가시켜 하단으로 이동시키고, `IsOffScreen` 함수를 통해 화면 밖으로 나간 객체를 판단하여 목록에서 제거합니다.

```cpp
// Obstacle 클래스 Update, Draw, IsOffScreen 함수 (위 main.cpp 내에 포함되어 있음)
void Obstacle::Update(float deltaTime) {
    // 상단에서 하단으로 이동
    position.y += speed * deltaTime;
}

void Obstacle::Draw() {
    // 사각형 형태의 장애물 그리기
    DrawRectangleV(position, size, color);
}

bool Obstacle::IsOffScreen(int screenHeight) {
    // 객체가 화면 하단 밖으로 완전히 나갔는지 체크
    return position.y > screenHeight; // 객체의 좌상단 y 좌표가 화면 높이를 넘었는지 체크
    // 또는 return position.y + size.y > screenHeight; // 객체의 하단 y 좌표가 화면 높이를 넘었는지 체크
}

// LifeItem 클래스도 유사하게 구현 (위 main.cpp 내에 포함되어 있음)
void LifeItem::Update(float deltaTime) {
    position.y += speed * deltaTime;
}

void LifeItem::Draw() {
    DrawRectangleV(position, size, color);
}

bool LifeItem::IsOffScreen(int screenHeight) {
    return position.y > screenHeight;
}
```

`GameManager::UpdateGame()` 내에서 `timeSinceLastObstacle` 및 `timeSinceLastLifeItem` 타이머를 사용하여 일정 간격마다 새로운 객체를 생성하고 `obstacles` 및 `lifeItems` 벡터에 추가하는 로직이 포함됩니다. 생성 빈도는 스테이지에 따라 조절됩니다. 화면 밖으로 나간 객체는 `std::remove_if`와 `erase`를 사용하여 목록에서 제거합니다.

### 4\. 충돌 감지, 생명 감소 및 생명 회복

`GameManager::CheckCollisions()` 함수에서 플레이어와 장애물/생명 박스 간의 충돌을 `CheckCollisionRecs()` 함수를 사용하여 감지하고, 충돌 결과에 따라 플레이어의 생명을 증감시키고 충돌한 객체를 목록에서 제거합니다.

```cpp
// GameManager CheckCollisions 함수 (위 main.cpp 내에 포함되어 있음)
void GameManager::CheckCollisions() {
    // 게임 오버 상태에서는 충돌 체크를 하지 않음
    if (gameState != GAME_PLAYING) return;

    // 주인공의 충돌 영역 (사각형)
    Rectangle playerRect = { player.position.x, player.position.y, player.size.x, player.size.y };

    // --- 장애물과의 충돌 체크 (생명 감소) ---
    // 장애물 목록을 순회하며 충돌 체크 및 제거
    obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [&](const Obstacle& obs){
        Rectangle obstacleRect = { obs.position.x, obs.position.y, obs.size.x, obs.size.y };
        bool collided = CheckCollisionRecs(playerRect, obstacleRect);

        if (collided) {
            // TODO: 플레이어 무적 상태가 아닐 때만 생명 감소
            // if (!player.isInvincible) {
                player.lives--; // Player 클래스에 lives 멤버가 있다고 가정
                TraceLog(LOG_INFO, TextFormat("Hit Obstacle! Lives: %i", player.lives));
                // TODO: 플레이어에게 짧은 무적 시간 부여 (무적 타이머 시작 등)
                return true; // 충돌한 장애물 제거
            // }
        }
        return obs.IsOffScreen(screenHeight); // 화면 밖으로 나간 장애물 제거
    }), obstacles.end());


    // --- 생명 회복 아이템과의 충돌 체크 (생명 증가) ---
    // 생명 회복 아이템 목록을 순회하며 충돌 체크 및 제거
    lifeItems.erase(std::remove_if(lifeItems.begin(), lifeItems.end(), [&](const LifeItem& item){
         Rectangle lifeItemRect = { item.position.x, item.position.y, item.size.x, item.size.y };
         bool collided = CheckCollisionRecs(playerRect, lifeItemRect);

        if (collided) {
            // 충돌 발생 시 생명 증가 (최대 생명 제한)
            player.lives++; // 생명 증가
             if (player.lives > player.maxLives) {
                player.lives = player.maxLives; // 최대 생명으로 제한
            }
            TraceLog(LOG_INFO, TextFormat("Got Life Item! Lives: %i", player.lives));
            return true; // 충돌한 아이템 제거
        }
        return item.IsOffScreen(screenHeight); // 화면 밖 아이템 제거
    }), lifeItems.end());

    // 모든 충돌 체크 및 생명 상태 확인 후 게임 오버 상태 전환
    if (player.lives <= 0) {
        gameState = GAME_OVER;
        finalScore = score; // 게임 오버 시 최종 점수 저장
        TraceLog(LOG_INFO, TextFormat("Game Over! Final Score: %i", finalScore));
        // TODO: 게임 오버 상태로 전환 시 필요한 추가 작업 (예: 타이머 중지)
    }
}
```

### 5\. 점수 및 시간 시스템

`GameManager::UpdateGame`에서 `elapsedTime`을 누적하고, `lastScoreTime`과의 차이를 이용해 1초마다 점수를 증가시킵니다. `GameManager::DrawGame`에서 현재 점수와 경과 시간을 화면에 표시합니다.

```cpp
// GameManager UpdateGame 함수 내 점수 및 시간 관련 로직 (위 main.cpp 내에 포함되어 있음)
void GameManager::UpdateGame(float deltaTime) {
    if (gameState == GAME_PLAYING) {
        elapsedTime += deltaTime; // 경과 시간 누적

        // 1초마다 점수 획득
        if (elapsedTime - lastScoreTime >= 1.0f) { // 1초 간격으로 체크
            score++;
            lastScoreTime = elapsedTime; // 마지막 점수 획득 시간 업데이트

            // 스테이지 증가 조건 체크 및 처리
            CheckStageIncrease();
        }
        // ... (다른 업데이트 로직: 플레이어, 장애물, 아이템 업데이트 및 생성)
    }
    // ... (게임 오버 상태 처리)
}

// GameManager DrawGame 함수 내 점수 및 시간 표시 (위 main.cpp 내에 포함되어 있음)
void GameManager::DrawGame() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (gameState == GAME_PLAYING) {
        // ... (플레이어, 장애물, 아이템 그리기)

        // UI 표시
        DrawText(TextFormat("Score: %i", score), 10, 10, 20, BLACK);
        DrawText(TextFormat("Lives: %i", player.lives), 10, 30, 20, BLACK);
        DrawText(TextFormat("Time: %.0f", elapsedTime), 10, 50, 20, BLACK); // 초 단위 정수로 표시
        DrawText(TextFormat("Stage: %i", currentStage), screenWidth - 100, 10, 20, BLACK);

    }
    // ... (게임 오버 화면 그리기)
    EndDrawing();
}
```

### 6\. 스테이지 시스템 및 난이도 조절

`GameManager` 클래스 멤버로 현재 스테이지, 스테이지업에 필요한 점수, 속도/생성 빈도 증가량 등을 정의합니다. `GameManager::CheckStageIncrease()` 함수를 통해 점수가 일정 기준에 도달하면 스테이지를 증가시키고, 장애물 속도 등을 조절합니다. 이 변경된 값은 장애물 생성 로직에 반영됩니다.

```cpp
// GameManager 클래스 멤버 (위 main.cpp 내에 포함되어 있음)
// int currentStage = 1;
// int scorePerStage = 10; // 스테이지가 증가하는 데 필요한 점수
// float baseObstacleSpeed = 200.0f; // 기본 장애물 속도
// float speedIncreasePerStage = 50.0f; // 스테이지당 증가 속도
// float baseSpawnRate = 2.0f; // 기본 생성 간격 (초)
// float spawnRateDecreasePerStage = 0.2f; // 스테이지당 생성 간격 감소량

// GameManager CheckStageIncrease 함수 (UpdateGame에서 점수 획득 시 호출 - 위 main.cpp 내에 포함되어 있음)
void GameManager::CheckStageIncrease() {
    // 다음 스테이지에 필요한 점수 계산
    int requiredScoreForNextStage = currentStage * scorePerStage;

    if (score >= requiredScoreForNextStage) {
        currentStage++;
        TraceLog(LOG_INFO, TextFormat("Stage Increased! Current Stage: %i", currentStage));

        // 스테이지 증가에 따른 속도 증가
        float newObstacleSpeed = baseObstacleSpeed + (currentStage - 1) * speedIncreasePerStage;
        // 현재 존재하는 장애물들의 속도도 업데이트할 수 있습니다.
         for (auto& obs : obstacles) { obs.speed = newObstacleSpeed; }

        // 스테이지 증가에 따른 생성 빈도 증가 (생성 간격 감소)는 UpdateGame에서 timeSinceLastObstacle 로직에 의해 자동 적용됨
        // float newSpawnRate = baseSpawnRate - (currentStage - 1) * spawnRateDecreasePerStage;
        // if (newSpawnRate < 0.5f) newSpawnRate = 0.5f; // 최소 생성 간격 제한 (예: 0.5초)
        // 장애물 생성 로직에서 이 newSpawnRate 값을 사용합니다.
    }
}

// GameManager에서 장애물 생성 로직 일부 (UpdateGame 함수 내에 포함되어 있음)
// timeSinceLastObstacle += deltaTime;
// float currentSpawnRate = baseSpawnRate - (currentStage - 1) * spawnRateDecreasePerStage;
// if (currentSpawnRate < 0.5f) currentSpawnRate = 0.5f;
// if (timeSinceLastObstacle >= currentSpawnRate) {
//     // 새로운 장애물 생성 로직...
//     newObstacle.speed = baseObstacleSpeed + (currentStage - 1) * speedIncreasePerStage; // 현재 스테이지 속도 적용
//     // ...
// }
```

### 7\. 게임 오버 및 재시작/종료

플레이어의 생명이 0 이하가 되면 `GameManager::CheckCollisions()` 함수에서 `gameState`를 `GAME_OVER`로 변경합니다. `GameManager::DrawGame()` 함수에서 게임 오버 상태일 때 게임 오버 메시지와 최종 점수, 재시작/종료 안내를 표시합니다. `GameManager::HandleInput()` 함수에서 게임 오버 상태일 때 Space 키 입력을 감지하여 `GameManager::InitGame()`을 호출하여 게임을 초기화합니다. 모든 상태에서 Esc 키를 누르면 `shouldClose` 플래그를 true로 설정하여 메인 루프를 종료합니다.

```cpp
// GameManager::HandleInput 함수 (전체 게임 상태에 대한 입력 처리 - 위 main.cpp 내에 포함되어 있음)

void GameManager::HandleInput() {
    // Esc 키는 어떤 상태에서든 게임 종료
    if (IsKeyPressed(KEY_ESCAPE)) {
        shouldClose = true; // 메인 루프 종료를 알리는 플래그 (GameManager 멤버)
    }

    if (gameState == GAME_PLAYING) {
        // 플레이 중 상태에서의 입력 (플레이어 이동)
        // 플레이어 클래스의 HandleInput 함수를 호출하거나 여기서 직접 처리
        if (IsKeyDown(KEY_RIGHT)) player.position.x += player.speed * GetFrameTime();
        if (IsKeyDown(KEY_LEFT)) player.position.x -= player.speed * GetFrameTime();

        // 화면 경계 체크 (Update에서 처리하는 것이 일반적이지만 입력과 함께 처리 가능)
        if (player.position.x < 0) player.position.x = 0;
        if (player.position.x > screenWidth - player.size.x) player.position.x = screenWidth - player.size.x;

    } else if (gameState == GAME_OVER) {
        // 게임 오버 상태에서의 입력 (재시작)
        if (IsKeyPressed(KEY_SPACE)) {
            InitGame(screenWidth, screenHeight); // 게임 재시작
        }
    }
}

// GameManager::DrawGame 함수 내 게임 오버 화면 출력 로직 (위 main.cpp 내에 포함되어 있음)
void GameManager::DrawGame() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (gameState == GAME_PLAYING) {
        // ... (플레이 중 UI 및 객체 그리기)
    } else if (gameState == GAME_OVER) {
        const char* gameOverText = "Game Over!";
        DrawText(gameOverText, screenWidth/2 - MeasureText(gameOverText, 40)/2, screenHeight/2 - 40, 40, RED); // 텍스트 위치 조정
        DrawText(TextFormat("Final Score: %i", finalScore), screenWidth/2 - MeasureText(TextFormat("Final Score: %i", finalScore), 30)/2, screenHeight/2 + 0, 30, BLACK); // 텍스트 크기 조정
        DrawText("Press SPACE to Restart", screenWidth/2 - MeasureText("Press SPACE to Restart", 20)/2, screenHeight/2 + 40, 20, DARKGRAY);
        DrawText("Press ESC to Exit", screenWidth/2 - MeasureText("Press ESC to Exit", 20)/2, screenHeight/2 + 60, 20, DARKGRAY);
    }

    EndDrawing();
}

// GameManager::ShouldClose 함수 (메인 루프 종료 조건 - 위 main.cpp 내에 포함되어 있음)
// bool GameManager::ShouldClose() const { return shouldClose || WindowShouldClose(); }
```

-----

## 💬 프로젝트 소감

### 🔹 전재민 (프로젝트 관리 / 기획 및 설계)

> (작성 예정)

### 🔹 김무진 (개발)

> (작성 예정)

### 🔹 김준용 (문서 / 테스트)

> (작성 예정)

-----

```
```
