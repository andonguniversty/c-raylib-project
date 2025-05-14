



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

이 게임은 Raylib의 메인 루프 구조를 따르며, `GameManager` 클래스 등을 활용하여 게임 상태 및 객체들을 관리할 수 있습니다.

### 1. 게임 초기화 및 메인 루프 구성

Raylib 창을 생성하고 게임의 기본 상태를 초기화합니다. `InitGame()`, `UpdateGame()`, `DrawGame()` 함수를 게임 루프 내에서 호출하여 게임 로직 업데이트 및 화면 그리기를 수행합니다.

```cpp
// 예시: main 함수 일부
int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Raylib Avoid Game");
    SetTargetFPS(60); // 프레임 속도 설정

    GameManager game;
    game.InitGame(screenWidth, screenHeight); // 게임 초기화

    while (!WindowShouldClose() && !game.ShouldClose()) { // Esc 키 종료 조건 추가
        game.HandleInput();          // 입력 처리
        game.UpdateGame(GetFrameTime()); // 게임 로직 업데이트
        game.DrawGame();             // 게임 화면 그리기
    }

    CloseWindow(); // 창 닫기
    return 0;
}
GameManager::InitGame()에서 플레이어, 장애물/아이템 목록, 점수, 시간, 스테이지 등을 초기 상태로 설정합니다 (생명 3개 포함).
GameManager::HandleInput()에서 키보드 입력을 처리합니다 (플레이어 이동, 재시작, 종료).
GameManager::UpdateGame()에서 게임 상태(플레이 중, 게임 오버)에 따라 플레이어/장애물/아이템 위치 업데이트, 충돌 체크, 점수/시간/스테이지 업데이트, 게임 오버 상태 전환 등을 처리합니다.
GameManager::DrawGame()에서 게임 상태에 따라 배경, 플레이어, 장애물/아이템, 점수/생명/시간/스테이지 정보, 게임 오버 메시지 등을 화면에 그립니다.
2. 주인공 이동 구현
Player 클래스를 정의하고, 좌우 방향키 입력을 받아 플레이어의 position.x 값을 업데이트합니다. 화면 좌우 경계를 벗어나지 않도록 제한합니다.

C++

// 예시: Player 클래스 Update 및 Draw 함수

void Player::Update(float deltaTime, int screenWidth, int screenHeight) {
    // 좌우 화살표 키 입력에 따른 수평 위치 변경
    if (IsKeyDown(KEY_RIGHT)) position.x += speed * deltaTime;
    if (IsKeyDown(KEY_LEFT))  position.x -= speed * deltaTime;

    // 화면 좌우 경계 체크
    if (position.x < 0) position.x = 0;
    if (position.x > screenWidth - size.x) position.x = screenWidth - size.x;

    // TODO: 무적 시간 처리 로직 추가될 수 있음
}

void Player::Draw() {
    DrawRectangleV(position, size, color);
    // TODO: 생명 표시 등을 플레이어 클래스 내부 또는 GameManager에서 처리
}
3. 장애물 및 생명 박스 생성 및 이동
Obstacle 및 LifeItem (혹은 공통 부모 클래스) 클래스를 정의합니다. 일정 시간 간격으로 화면 상단 임의의 위치에 객체를 생성하고, Update 함수에서 position.y 값을 증가시켜 하단으로 이동시킵니다. 화면 하단 밖으로 나간 객체는 목록에서 제거합니다.

C++

// 예시: Obstacle 클래스 Update, Draw, IsOffScreen 함수
// Obstacle 클래스는 Vector2 position, Vector2 size, Color color, float speed; 등의 멤버를 가집니다.

void Obstacle::Update(float deltaTime) {
    // 상단에서 하단으로 이동
    position.y += speed * deltaTime;
}

void Obstacle::Draw() {
    // 사각형 형태의 장애물 그리기
    DrawRectangleV(position, size, color);
}

// 화면 높이 정보를 받아야 체크 가능
bool Obstacle::IsOffScreen(int screenHeight) {
    // 객체가 화면 하단 밖으로 완전히 나갔는지 체크
    return position.y > screenHeight; // 객체의 좌상단 y 좌표가 화면 높이를 넘었는지 체크
    // 또는 position.y + size.y > screenHeight; // 객체의 하단 y 좌표가 화면 높이를 넘었는지 체크
}

// LifeItem 클래스도 유사하게 구현

4. 충돌 감지, 생명 감소 및 생명 회복
CheckCollisionRecs() 함수를 사용하여 플레이어와 장애물/생명 박스 간의 충돌을 감지합니다.

장애물 충돌: 충돌 시 플레이어의 생명을 1 감소시키고, 충돌한 장애물은 목록에서 제거합니다. 연속 충돌 방지를 위해 플레이어에게 짧은 무적 시간을 부여하는 로직을 추가할 수 있습니다.
생명 박스 충돌: 충돌 시 플레이어의 생명을 1 증가시키고 (최대 생명 제한), 충돌한 생명 박스는 목록에서 제거합니다.
<!-- end list -->

C++

// 예시: GameManager CheckCollisions 함수 (UpdateGame 함수 내에서 호출)

void GameManager::CheckCollisions() {
    // 게임 오버 상태에서는 충돌 체크를 하지 않음
    if (gameState != GAME_PLAYING) return;

    // 주인공의 충돌 영역 (사각형)
    Rectangle playerRect = { player.position.x, player.position.y, player.size.x, player.size.y };

    // --- 장애물과의 충돌 체크 (생명 감소) ---
    // 장애물 목록을 순회하며 충돌 체크
    // obstacles는 Obstacle 객체의 벡터라고 가정
    // 충돌했거나 화면 밖으로 나간 장애물을 임시로 저장할 인덱스 목록
    std::vector<size_t> obstaclesToRemove;

    for (size_t i = 0; i < obstacles.size(); ++i) {
        Rectangle obstacleRect = { obstacles[i].position.x, obstacles[i].position.y, obstacles[i].size.x, obstacles[i].size.y };

        // 주인공과 장애물 간의 충돌 체크
        if (CheckCollisionRecs(playerRect, obstacleRect)) {
            // TODO: 플레이어 무적 상태가 아닐 때만 생명 감소
            // if (!player.isInvincible) {
                player.lives--; // Player 클래스에 lives 멤버가 있다고 가정
                // TODO: 플레이어에게 짧은 무적 시간 부여 (무적 타이머 시작 등)
                obstaclesToRemove.push_back(i); // 충돌한 장애물 제거 예정
            // }
        } else if (obstacles[i].IsOffScreen(screenHeight)) { // 화면 밖으로 나갔는지 체크 (점수와 관련 없음 - 시간 기반 점수)
             obstaclesToRemove.push_back(i); // 화면 밖 장애물 제거 예정
        }
    }

    // 뒤에서부터 제거하여 인덱스 오류 방지
    for (int i = obstaclesToRemove.size() - 1; i >= 0; --i) {
        obstacles.erase(obstacles.begin() + obstaclesToRemove[i]);
    }


    // --- 생명 회복 아이템과의 충돌 체크 (생명 증가) ---
    // 생명 회복 아이템 목록을 순회하며 충돌 체크
    // lifeItems는 LifeItem 객체의 벡터라고 가정
    std::vector<size_t> lifeItemsToRemove;

    for (size_t i = 0; i < lifeItems.size(); ++i) {
        Rectangle lifeItemRect = { lifeItems[i].position.x, lifeItems[i].position.y, lifeItems[i].size.x, lifeItems[i].size.y };

        // 주인공과 생명 회복 아이템 간의 충돌 체크
        if (CheckCollisionRecs(playerRect, lifeItemRect)) {
            // 충돌 발생 시 생명 증가 (최대 생명 제한)
            player.lives++; // 생명 증가
            // Player 클래스에 maxLives 멤버가 있다고 가정
            if (player.lives > player.maxLives) {
                player.lives = player.maxLives; // 최대 생명으로 제한
            }
            lifeItemsToRemove.push_back(i); // 충돌한 아이템 제거 예정
        } else if (lifeItems[i].IsOffScreen(screenHeight)) { // 화면 밖으로 나갔는지 체크
             lifeItemsToRemove.push_back(i); // 화면 밖 아이템 제거 예정
        }
    }

    // 뒤에서부터 제거하여 인덱스 오류 방지
    for (int i = lifeItemsToRemove.size() - 1; i >= 0; --i) {
        lifeItems.erase(lifeItems.begin() + lifeItemsToRemove[i]);
    }

    // 모든 충돌 체크 및 생명 상태 확인 후 게임 오버 상태 전환
    if (player.lives <= 0) {
        gameState = GAME_OVER;
        finalScore = score; // 게임 오버 시 최종 점수 저장
        // TODO: 게임 오버 상태로 전환 시 필요한 추가 작업 (예: 타이머 중지)
    }
}

5. 점수 및 시간 시스템
게임 시작 후 경과 시간을 측정하고, 일정 시간(예: 1초)마다 점수를 1점씩 증가시킵니다. 게임 오버 시 최종 점수를 표시합니다.

C++

// 예시: GameManager UpdateGame 함수 내 점수 및 시간 관련 로직

void GameManager::UpdateGame(float deltaTime) {
    if (gameState == GAME_PLAYING) {
        elapsedTime += deltaTime; // 경과 시간 누적

        // 1초마다 점수 획득
        if (elapsedTime - lastScoreTime >= 1.0f) { // 1초 간격으로 체크
            score++;
            lastScoreTime = elapsedTime; // 마지막 점수 획득 시간 업데이트

            // TODO: 스테이지 증가 조건 체크 및 처리
            CheckStageIncrease();
        }

        // ... (다른 업데이트 로직)
    } else if (gameState == GAME_OVER) {
        // 게임 오버 상태에서의 입력 처리 (재시작, 종료)
        if (IsKeyPressed(KEY_SPACE)) {
            InitGame(GetScreenWidth(), GetScreenHeight()); // 게임 재시작
        }
        // Esc 종료는 메인 루프에서 처리
    }
}

// 예시: GameManager DrawGame 함수 내 점수 및 시간 표시
void GameManager::DrawGame() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (gameState == GAME_PLAYING) {
        // ... (플레이어, 장애물, 아이템 그리기)

        // UI 표시
        DrawText(TextFormat("Score: %i", score), 10, 10, 20, BLACK);
        DrawText(TextFormat("Lives: %i", player.lives), 10, 30, 20, BLACK);
        DrawText(TextFormat("Time: %.0f", elapsedTime), 10, 50, 20, BLACK); // 초 단위 정수로 표시
        DrawText(TextFormat("Stage: %i", currentStage), GetScreenWidth() - 100, 10, 20, BLACK);

    } else if (gameState == GAME_OVER) {
        const char* gameOverText = "Game Over!";
        DrawText(gameOverText, GetScreenWidth()/2 - MeasureText(gameOverText, 40)/2, GetScreenHeight()/2 - 40, 40, RED); // 텍스트 위치 조정
        DrawText(TextFormat("Final Score: %i", finalScore), GetScreenWidth()/2 - MeasureText(TextFormat("Final Score: %i", finalScore), 30)/2, GetScreenHeight()/2 + 0, 30, BLACK); // 텍스트 크기 조정
        DrawText("Press SPACE to Restart", GetScreenWidth()/2 - MeasureText("Press SPACE to Restart", 20)/2, GetScreenHeight()/2 + 40, 20, DARKGRAY);
        DrawText("Press ESC to Exit", GetScreenWidth()/2 - MeasureText("Press ESC to Exit", 20)/2, GetScreenHeight()/2 + 60, 20, DARKGRAY);
    }

    EndDrawing();
}
6. 스테이지 시스템 및 난이도 조절
점수가 일정 기준(예: 10점 단위)에 도달할 때마다 스테이지를 증가시킵니다. 스테이지가 증가하면 장애물의 낙하 속도와 생성 빈도를 증가시켜 게임 난이도를 높입니다.

C++

// 예시: GameManager 클래스 멤버
int currentStage = 1;
int scorePerStage = 10; // 스테이지가 증가하는 데 필요한 점수
float baseObstacleSpeed = 200.0f; // 기본 장애물 속도
float speedIncreasePerStage = 50.0f; // 스테이지당 증가 속도
float baseSpawnRate = 2.0f; // 기본 생성 간격 (초)
float spawnRateDecreasePerStage = 0.2f; // 스테이지당 생성 간격 감소량

// 예시: GameManager CheckStageIncrease 함수 (UpdateGame에서 점수 획득 시 호출)
void GameManager::CheckStageIncrease() {
    // 다음 스테이지에 필요한 점수 계산
    int requiredScoreForNextStage = currentStage * scorePerStage;

    if (score >= requiredScoreForNextStage) {
        currentStage++;
        TraceLog(LOG_INFO, TextFormat("Stage Increased! Current Stage: %i", currentStage));

        // 스테이지 증가에 따른 속도 증가
        float newObstacleSpeed = baseObstacleSpeed + (currentStage - 1) * speedIncreasePerStage;
        // 현재 존재하는 장애물들의 속도도 업데이트할 수 있습니다.
        // for (auto& obs : obstacles) { obs.speed = newObstacleSpeed; }
        // 또는 단순히 새로 생성되는 장애물에만 적용

        // 스테이지 증가에 따른 생성 빈도 증가 (생성 간격 감소)
        float newSpawnRate = baseSpawnRate - (currentStage - 1) * spawnRateDecreasePerStage;
        if (newSpawnRate < 0.5f) newSpawnRate = 0.5f; // 최소 생성 간격 제한 (예: 0.5초)
        // 장애물 생성 로직에서 이 newSpawnRate 값을 사용합니다.
    }
}

// 예시: GameManager에서 장애물 생성 로직 일부
// float timeSinceLastObstacle = 0.0f; // GameManager 멤버

// void GameManager::UpdateGame(float deltaTime) {
//     if (gameState == GAME_PLAYING) {
//         // ...
//         timeSinceLastObstacle += deltaTime;
//         // 현재 스테이지에 맞는 생성 간격 계산
//         float currentSpawnRate = baseSpawnRate - (currentStage - 1) * spawnRateDecreasePerStage;
//         if (currentSpawnRate < 0.5f) currentSpawnRate = 0.5f; // 최소 제한

//         if (timeSinceLastObstacle >= currentSpawnRate) {
//             // 새로운 장애물 생성 로직
//             Obstacle newObstacle;
//             newObstacle.speed = baseObstacleSpeed + (currentStage - 1) * speedIncreasePerStage; // 현재 스테이지 속도 적용
//             // ... (위치, 크기, 색상 설정)
//             obstacles.push_back(newObstacle);
//             timeSinceLastObstacle = 0.0f; // 타이머 리셋
//         }
//         // ...
//     }
// }
7. 게임 오버 및 재시작/종료
플레이어의 생명이 0 이하가 되면 gameState를 GAME_OVER로 변경합니다. 게임 오버 상태에서는 "Game Over" 메시지와 최종 점수를 표시하고, Space 키를 누르면 InitGame()을 다시 호출하여 게임을 초기 상태로 되돌립니다. Esc 키를 누르면 게임 루프를 종료합니다.

C++

// 예시: GameManager::HandleInput 함수 (전체 게임 상태에 대한 입력 처리)

void GameManager::HandleInput() {
    // Esc 키는 어떤 상태에서든 게임 종료
    if (IsKeyPressed(KEY_ESCAPE)) {
        shouldClose = true; // 메인 루프 종료를 알리는 플래그 (GameManager 멤버)
    }

    if (gameState == GAME_PLAYING) {
        // 플레이 중 상태에서의 입력 (플레이어 이동)
        // 플레이어 클래스의 HandleInput 함수를 호출하거나 여기서 직접 처리
        player.HandleInput(); // Player 클래스에 HandleInput 함수 추가 고려
    } else if (gameState == GAME_OVER) {
        // 게임 오버 상태에서의 입력 (재시작)
        if (IsKeyPressed(KEY_SPACE)) {
            InitGame(GetScreenWidth(), GetScreenHeight()); // 게임 재시작
        }
    }
}

// GameManager 클래스에 bool shouldClose 멤버와 ShouldClose() 함수 추가
// bool GameManager::ShouldClose() const { return shouldClose; }

// Player 클래스 HandleInput 예시
// void Player::HandleInput() {
//     if (IsKeyDown(KEY_RIGHT)) position.x += speed * GetFrameTime(); // GetFrameTime 사용
//     if (IsKeyDown(KEY_LEFT))  position.x -= speed * GetFrameTime(); // GetFrameTime 사용
//     // 화면 경계 체크는 Update에서 하는 것이 일반적
// }
```
네, 제공해주신 게임 기능 요구사항과 README 구조, 코드 스니펫을 바탕으로 README를 수정해 드리겠습니다.

게임의 핵심 기능과 구현 방식을 명확하게 설명하고, 누락된 기능 (시간 표시, 시간 기반 점수 획득, 스테이지 시스템, 재시작/종료 입력 처리)을 포함하여 README를 업데이트하겠습니다.

Markdown

# 피하기 게임 (Avoid Game) with Raylib

이 프로젝트는 C++과 Raylib 라이브러리를 사용하여 개발된 간단한 피하기 게임입니다. 플레이어는 화면 하단의 파란색 바로 장애물(빨간색 박스)을 피하고 생명 회복 아이템(초록색 박스)을 획득하며 최고 점수를 목표로 합니다.

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

이 게임은 Raylib의 메인 루프 구조를 따르며, `GameManager` 클래스 등을 활용하여 게임 상태 및 객체들을 관리할 수 있습니다.

### 1. 게임 초기화 및 메인 루프 구성

Raylib 창을 생성하고 게임의 기본 상태를 초기화합니다. `InitGame()`, `UpdateGame()`, `DrawGame()` 함수를 게임 루프 내에서 호출하여 게임 로직 업데이트 및 화면 그리기를 수행합니다.

```cpp
// 예시: main 함수 일부
int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Raylib Avoid Game");
    SetTargetFPS(60); // 프레임 속도 설정

    GameManager game;
    game.InitGame(screenWidth, screenHeight); // 게임 초기화

    while (!WindowShouldClose() && !game.ShouldClose()) { // Esc 키 종료 조건 추가
        game.HandleInput();          // 입력 처리
        game.UpdateGame(GetFrameTime()); // 게임 로직 업데이트
        game.DrawGame();             // 게임 화면 그리기
    }

    CloseWindow(); // 창 닫기
    return 0;
}
GameManager::InitGame()에서 플레이어, 장애물/아이템 목록, 점수, 시간, 스테이지 등을 초기 상태로 설정합니다 (생명 3개 포함).
GameManager::HandleInput()에서 키보드 입력을 처리합니다 (플레이어 이동, 재시작, 종료).
GameManager::UpdateGame()에서 게임 상태(플레이 중, 게임 오버)에 따라 플레이어/장애물/아이템 위치 업데이트, 충돌 체크, 점수/시간/스테이지 업데이트, 게임 오버 상태 전환 등을 처리합니다.
GameManager::DrawGame()에서 게임 상태에 따라 배경, 플레이어, 장애물/아이템, 점수/생명/시간/스테이지 정보, 게임 오버 메시지 등을 화면에 그립니다.
2. 주인공 이동 구현
Player 클래스를 정의하고, 좌우 방향키 입력을 받아 플레이어의 position.x 값을 업데이트합니다. 화면 좌우 경계를 벗어나지 않도록 제한합니다.

C++

// 예시: Player 클래스 Update 및 Draw 함수
void Player::Update(float deltaTime, int screenWidth, int screenHeight) {
    // 좌우 화살표 키 입력에 따른 수평 위치 변경
    if (IsKeyDown(KEY_RIGHT)) position.x += speed * deltaTime;
    if (IsKeyDown(KEY_LEFT))  position.x -= speed * deltaTime;

    // 화면 좌우 경계 체크
    if (position.x < 0) position.x = 0;
    if (position.x > screenWidth - size.x) position.x = screenWidth - size.x;

    // TODO: 무적 시간 처리 로직 추가될 수 있음
}

void Player::Draw() {
    DrawRectangleV(position, size, color);
    // TODO: 생명 표시 등을 플레이어 클래스 내부 또는 GameManager에서 처리
}
3. 장애물 및 생명 박스 생성 및 이동
Obstacle 및 LifeItem (혹은 공통 부모 클래스) 클래스를 정의합니다. 일정 시간 간격으로 화면 상단 임의의 위치에 객체를 생성하고, Update 함수에서 position.y 값을 증가시켜 하단으로 이동시킵니다. 화면 하단 밖으로 나간 객체는 목록에서 제거합니다.

C++

// 예시: Obstacle 클래스 Update, Draw, IsOffScreen 함수
// Obstacle 클래스는 Vector2 position, Vector2 size, Color color, float speed; 등의 멤버를 가집니다.

void Obstacle::Update(float deltaTime) {
    // 상단에서 하단으로 이동
    position.y += speed * deltaTime;
}

void Obstacle::Draw() {
    // 사각형 형태의 장애물 그리기
    DrawRectangleV(position, size, color);
}

// 화면 높이 정보를 받아야 체크 가능
bool Obstacle::IsOffScreen(int screenHeight) {
    // 객체가 화면 하단 밖으로 완전히 나갔는지 체크
    return position.y > screenHeight; // 객체의 좌상단 y 좌표가 화면 높이를 넘었는지 체크
    // 또는 position.y + size.y > screenHeight; // 객체의 하단 y 좌표가 화면 높이를 넘었는지 체크
}

// LifeItem 클래스도 유사하게 구현
GameManager::UpdateGame() 내에서 일정 간격으로 새로운 Obstacle 또는 LifeItem 객체를 생성하여 관리 목록에 추가합니다.
생성 빈도는 현재 스테이지에 따라 조절됩니다.
4. 충돌 감지, 생명 감소 및 생명 회복
CheckCollisionRecs() 함수를 사용하여 플레이어와 장애물/생명 박스 간의 충돌을 감지합니다.

장애물 충돌: 충돌 시 플레이어의 생명을 1 감소시키고, 충돌한 장애물은 목록에서 제거합니다. 연속 충돌 방지를 위해 플레이어에게 짧은 무적 시간을 부여하는 로직을 추가할 수 있습니다.
생명 박스 충돌: 충돌 시 플레이어의 생명을 1 증가시키고 (최대 생명 제한), 충돌한 생명 박스는 목록에서 제거합니다.
<!-- end list -->

C++

// 예시: GameManager CheckCollisions 함수 (UpdateGame 함수 내에서 호출)

void GameManager::CheckCollisions() {
    // 게임 오버 상태에서는 충돌 체크를 하지 않음
    if (gameState != GAME_PLAYING) return;

    // 주인공의 충돌 영역 (사각형)
    Rectangle playerRect = { player.position.x, player.position.y, player.size.x, player.size.y };

    // --- 장애물과의 충돌 체크 (생명 감소) ---
    // 장애물 목록을 순회하며 충돌 체크
    // obstacles는 Obstacle 객체의 벡터라고 가정
    // 충돌했거나 화면 밖으로 나간 장애물을 임시로 저장할 인덱스 목명 시스템** (score 점수 화면)



5. **속도/크기 변화**(점수 변화에 따른 장애물 크기 변화)



6. **게임 오버 UI**(장애물에 닿았을시 게임종료)


---

## 💬 프로젝트 소감

### 🔹 전재민 (프로젝트 관리 / 기획 및 설계)

> (작성 예정)

### 🔹 김무진 (개발)

> (작성 예정)

### 🔹 김준용 (문서 / 테스트)

> (작성 예정)

---



```

--- 

```
