알겠습니다. 실행화면 섹션에 이미지를 넣을 수 있도록 마크다운 이미지 문법을 사용하여 다시 구성해 드리겠습니다. 실제 이미지 파일은 사용자가 GitHub 등의 저장소에 업로드한 후 해당 링크로 교체해야 합니다.

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

## 🏗️ 시스템 구조

본 게임은 Raylib의 메인 게임 루프를 기반으로 객체 지향 프로그래밍 개념을 활용하여 구현되었습니다. 주요 구성 요소는 다음과 같습니다.

* **GameManager**: 게임의 전체 상태(플레이 중, 게임 오버), 점수, 시간, 스테이지, 객체 목록(플레이어, 장애물, 아이템)을 관리합니다. 게임 초기화, 업데이트, 그리기, 입력 처리, 충돌 감지, 스테이지 관리를 담당하는 핵심 클래스입니다.
* **Player**: 플레이어 객체의 위치, 크기, 색상, 속도, 생명 등의 속성을 가집니다. 이동 및 상태(예: 무적 시간) 관리를 담당합니다.
* **Obstacle**: 장애물 객체의 위치, 크기, 색상, 낙하 속도 등의 속성을 가집니다. 화면 상단에서 생성되어 하단으로 이동하는 로직을 처리합니다.
* **LifeItem**: 생명 회복 아이템 객체의 위치, 크기, 색상, 낙하 속성 등을 가집니다. 장애물과 유사하게 동작하지만, 충돌 시 생명을 회복시키는 역할을 합니다.

```mermaid
classDiagram
    GameManager o-- Player
    GameManager o-- Obstacle
    GameManager o-- LifeItem

    class GameManager {
        +GameState gameState
        +Player player
        +vector~Obstacle~ obstacles
        +vector~LifeItem~ lifeItems
        +int score
        +float elapsedTime
        +int currentStage
        +InitGame()
        +HandleInput()
        +UpdateGame()
        +DrawGame()
        +CheckCollisions()
        +CheckStageIncrease()
        +ShouldClose() bool
    }

    class Player {
        +Vector2 position
        +Vector2 size
        +Color color
        +float speed
        +int lives
        +int maxLives
        +Update(float, int, int)
        +Draw()
    }

    class Obstacle {
        +Vector2 position
        +Vector2 size
        +Color color
        +float speed
        +Update(float)
        +Draw()
        +IsOffScreen(int) bool
    }

     class LifeItem {
        +Vector2 position
        +Vector2 size
        +Color color
        +float speed
        +Update(float)
        +Draw()
        +IsOffScreen(int) bool
    }
````

## 🛠️ 핵심 기능

### 1\. 게임 루프 및 상태 관리

Raylib의 `InitWindow`, `SetTargetFPS`, `WindowShouldClose`, `BeginDrawing`, `EndDrawing` 함수를 사용하여 게임의 기본 루프를 구성하고, `GameManager` 클래스의 `gameState` 변수를 통해 게임 플레이와 게임 오버 상태를 전환하며 각 상태에 맞는 로직을 처리합니다.

```cpp
// 예시: main 함수 및 GameManager 주요 함수 호출 구조
int main() {
    // ... (창 초기화 및 FPS 설정)

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

// GameManager::UpdateGame 함수 일부 (게임 상태에 따른 분기)
void GameManager::UpdateGame(float deltaTime) {
    if (gameState == GAME_PLAYING) {
        // 플레이 중 로직: 객체 업데이트, 생성, 충돌 체크, 점수/시간/스테이지 업데이트
        // ...
    } else if (gameState == GAME_OVER) {
        // 게임 오버 로직: 입력 처리 (재시작)
    }
}

// GameManager::DrawGame 함수 일부 (게임 상태에 따른 분기)
void GameManager::DrawGame() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (gameState == GAME_PLAYING) {
        // 플레이 중 그리기: 플레이어, 장애물, 아이템, UI
        // ...
    } else if (gameState == GAME_OVER) {
        // 게임 오버 화면 그리기: 메시지, 최종 점수, 재시작 안내
        // ...
    }
    EndDrawing();
}
```

### 2\. 플레이어 조작 및 경계 처리

`Player` 클래스는 `position`과 `speed`를 가지고 있으며, `GameManager::HandleInput` 또는 `Player::HandleInput` 함수에서 좌우 방향키 입력을 감지하여 `position.x`를 업데이트합니다. 화면 좌우 경계를 벗어나지 않도록 위치를 제한하는 로직을 포함합니다.

```cpp
// 예시: Player 클래스 Update 및 GameManager::HandleInput 일부
void Player::Update(float deltaTime, int screenWidth, int screenHeight) {
    // 화면 좌우 경계 체크
    if (position.x < 0) position.x = 0;
    if (position.x > screenWidth - size.x) position.x = screenWidth - size.x;
    // TODO: 무적 시간 처리 로직 추가될 수 있음
}

void GameManager::HandleInput() {
    // ... (Esc 키 처리)
    if (gameState == GAME_PLAYING) {
        // 플레이어 이동 입력 처리
        if (IsKeyDown(KEY_RIGHT)) player.position.x += player.speed * GetFrameTime();
        if (IsKeyDown(KEY_LEFT)) player.position.x -= player.speed * GetFrameTime();
        // 경계 체크는 Player::Update에서 처리하거나 여기서 함께 처리
    }
    // ... (게임 오버 상태 입력 처리)
}
```

### 3\. 장애물 및 아이템 생성/관리

`GameManager::UpdateGame` 함수 내에서 타이머(`timeSinceLastObstacle`, `timeSinceLastLifeItem`)를 사용하여 일정 간격마다 화면 상단에 `Obstacle` 또는 `LifeItem` 객체를 생성하고 해당 목록(`obstacles`, `lifeItems`)에 추가합니다. 각 객체의 `Update` 함수를 호출하여 하단으로 이동시키고, `IsOffScreen` 함수를 통해 화면 밖으로 나간 객체는 목록에서 제거합니다.

```cpp
// 예시: GameManager UpdateGame 함수 내 객체 생성 및 업데이트 로직 일부
void GameManager::UpdateGame(float deltaTime) {
    if (gameState == GAME_PLAYING) {
        // ... (점수/시간 업데이트)

        // 장애물 생성
        timeSinceLastObstacle += deltaTime;
        float currentSpawnRate = baseSpawnRate - (currentStage - 1) * spawnRateDecreasePerStage; // 스테이지별 빈도 조절
        if (currentSpawnRate < 0.5f) currentSpawnRate = 0.5f; // 최소 제한

        if (timeSinceLastObstacle >= currentSpawnRate) {
            Obstacle newObstacle;
            // ... (위치, 크기, 속도 설정 - 스테이지 속도 반영)
            obstacles.push_back(newObstacle);
            timeSinceLastObstacle = 0.0f;
        }

        // 생명 아이템 생성 (유사한 로직)
        timeSinceLastLifeItem += deltaTime;
         if (timeSinceLastLifeItem >= lifeItemSpawnRate) { // 고정된 생명 아이템 생성 간격 예시
             LifeItem newLifeItem;
             // ... (위치, 크기, 속도 설정)
             lifeItems.push_back(newLifeItem);
             timeSinceLastLifeItem = 0.0f;
         }


        // 장애물 업데이트 및 화면 밖 제거
        for (auto& obs : obstacles) { obs.Update(deltaTime); }
        obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [&](const Obstacle& o){
            return o.IsOffScreen(screenHeight);
        }), obstacles.end());

        // 생명 아이템 업데이트 및 화면 밖 제거
        for (auto& item : lifeItems) { item.Update(deltaTime); }
        lifeItems.erase(std::remove_if(lifeItems.begin(), lifeItems.end(), [&](const LifeItem& i){
            return i.IsOffScreen(screenHeight);
        }), lifeItems.end());

        // ... (충돌 체크)
    }
    // ...
}
```

### 4\. 충돌 감지 및 생명 시스템

`GameManager::CheckCollisions` 함수에서 Raylib의 `CheckCollisionRecs`를 사용하여 플레이어와 장애물 또는 아이템의 충돌을 감지합니다. 장애물과 충돌 시 플레이어의 생명을 감소시키고 (필요시 무적 시간 고려), 아이템과 충돌 시 생명을 증가시킵니다 (최대 생명 제한). 충돌한 객체는 목록에서 제거합니다. 플레이어 생명이 0이 되면 게임 오버 상태로 전환합니다.

```cpp
// 예시: GameManager CheckCollisions 함수 일부
void GameManager::CheckCollisions() {
    if (gameState != GAME_PLAYING) return;
    Rectangle playerRect = { player.position.x, player.position.y, player.size.x, player.size.y };

    // --- 장애물 충돌 체크 ---
    obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [&](const Obstacle& obs){
        Rectangle obstacleRect = { obs.position.x, obs.position.y, obs.size.x, obs.size.y };
        if (CheckCollisionRecs(playerRect, obstacleRect)) {
            // TODO: 무적 시간 체크 후 생명 감소
            player.lives--;
            TraceLog(LOG_INFO, TextFormat("Hit Obstacle! Lives: %i", player.lives));
            return true; // 장애물 제거
        }
        return obs.IsOffScreen(screenHeight);
    }), obstacles.end());

    // --- 생명 아이템 충돌 체크 ---
    lifeItems.erase(std::remove_if(lifeItems.begin(), lifeItems.end(), [&](const LifeItem& item){
         Rectangle lifeItemRect = { item.position.x, item.position.y, item.size.x, item.size.y };
         if (CheckCollisionRecs(playerRect, lifeItemRect)) {
             player.lives++;
             if (player.lives > player.maxLives) player.lives = player.maxLives; // 최대 생명 제한
             TraceLog(LOG_INFO, TextFormat("Got Life Item! Lives: %i", player.lives));
             return true; // 아이템 제거
         }
        return item.IsOffScreen(screenHeight);
    }), lifeItems.end());

    // 게임 오버 조건 체크
    if (player.lives <= 0) {
        gameState = GAME_OVER;
        finalScore = score;
        TraceLog(LOG_INFO, TextFormat("Game Over! Final Score: %i", finalScore));
    }
}
```

### 5\. 점수, 시간, 스테이지 시스템 및 UI 표시

`GameManager::UpdateGame`에서 `elapsedTime`과 `score`를 업데이트하고, `CheckStageIncrease` 함수를 통해 점수에 따라 `currentStage`를 증가시킵니다. `GameManager::DrawGame` 함수에서 `DrawText`를 사용하여 현재 점수, 남은 생명, 경과 시간, 현재 스테이지 정보를 화면에 표시합니다.

```cpp
// 예시: GameManager UpdateGame 함수 내 점수/시간/스테이지 로직 일부
void GameManager::UpdateGame(float deltaTime) {
    if (gameState == GAME_PLAYING) {
        elapsedTime += deltaTime;
        if (elapsedTime - lastScoreTime >= 1.0f) {
            score++;
            lastScoreTime = elapsedTime;
            CheckStageIncrease(); // 점수 획득 시 스테이지 체크
        }
        // ...
    }
}

// 예시: GameManager CheckStageIncrease 함수
void GameManager::CheckStageIncrease() {
    int requiredScoreForNextStage = currentStage * scorePerStage;
    if (score >= requiredScoreForNextStage) {
        currentStage++;
        TraceLog(LOG_INFO, TextFormat("Stage Increased! Current Stage: %i", currentStage));
        // TODO: 스테이지 증가에 따른 장애물 속도, 생성 빈도 조절 로직 적용
    }
}

// 예시: GameManager DrawGame 함수 내 UI 그리기 일부
void GameManager::DrawGame() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    if (gameState == GAME_PLAYING) {
        // ... (객체 그리기)
        // UI 표시
        DrawText(TextFormat("Score: %i", score), 10, 10, 20, BLACK);
        DrawText(TextFormat("Lives: %i", player.lives), 10, 30, 20, BLACK);
        DrawText(TextFormat("Time: %.0f", elapsedTime), 10, 50, 20, BLACK);
        DrawText(TextFormat("Stage: %i", currentStage), screenWidth - 100, 10, 20, BLACK);
    }
    // ...
    EndDrawing();
}
```

### 6\. 게임 오버 및 재시작/종료

`GameManager::CheckCollisions`에서 생명이 0이 되면 `gameState`를 `GAME_OVER`로 설정합니다. `GameManager::DrawGame`에서 게임 오버 상태일 때 최종 점수와 재시작/종료 안내 메시지를 중앙에 표시합니다. `GameManager::HandleInput`에서 게임 오버 상태 중 Space 키 입력이 있으면 `GameManager::InitGame`을 호출하여 게임을 초기 상태로 되돌립니다. Esc 키 입력은 `shouldClose` 플래그를 통해 게임 루프를 종료시킵니다.

```cpp
// 예시: GameManager CheckCollisions 함수 일부 (게임 오버 상태 전환)
void GameManager::CheckCollisions() {
    // ... (충돌 처리 후)
    if (player.lives <= 0) {
        gameState = GAME_OVER;
        finalScore = score;
        TraceLog(LOG_INFO, TextFormat("Game Over! Final Score: %i", finalScore));
    }
}

// 예시: GameManager DrawGame 함수 내 게임 오버 화면 그리기
void GameManager::DrawGame() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    if (gameState == GAME_PLAYING) {
        // ...
    } else if (gameState == GAME_OVER) {
        const char* gameOverText = "Game Over!";
        DrawText(gameOverText, screenWidth/2 - MeasureText(gameOverText, 40)/2, screenHeight/2 - 40, 40, RED);
        DrawText(TextFormat("Final Score: %i", finalScore), screenWidth/2 - MeasureText(TextFormat("Final Score: %i", finalScore), 30)/2, screenHeight/2 + 0, 30, BLACK);
        DrawText("Press SPACE to Restart", screenWidth/2 - MeasureText("Press SPACE to Restart", 20)/2, screenHeight/2 + 40, 20, DARKGRAY);
        DrawText("Press ESC to Exit", screenWidth/2 - MeasureText("Press ESC to Exit", 20)/2, screenHeight/2 + 60, 20, DARKGRAY);
    }
    EndDrawing();
}

// 예시: GameManager HandleInput 함수 내 재시작/종료 처리
void GameManager::HandleInput() {
    if (IsKeyPressed(KEY_ESCAPE)) {
        shouldClose = true;
    }
    if (gameState == GAME_OVER) {
        if (IsKeyPressed(KEY_SPACE)) {
            InitGame(screenWidth, screenHeight); // 게임 재시작
        }
    }
    // ...
}
```

## 📅 개발 일정

| 일차 | 날짜 | 주요 작업 | 참여 |
|------|------|----------|------|
| 1일차 | 05.14 | 주제 선정,일정 산정,역할 분배,계획 및 설계 | 전재민, 김무진, 김준용 |
| 2일차 | 05.15 | 
| 3일차 | 05.16 | 
| 4일차 | 05.17 |
| 5일차 | 05.18 | 

## 💡 성과 및 개선점

### ✅ 성공 요인

  - Raylib 라이브러리 학습 및 활용 능력 배양
      * 게임 개발에 필요한 기본적인 그래픽, 입력, 게임 루프 관리를 경험
  - 객체 지향 설계를 통한 코드 모듈화
      * GameManager, Player, Obstacle 등 역할을 분담하여 코드의 가독성 및 관리 용이성 향상
  - 기본적인 게임 메커니즘 구현 경험
      * 이동, 충돌, 점수, 생명, 난이도 조절 등 핵심 게임 기능을 직접 구현
  - Git을 활용한 팀 협업
      * 변경 이력 관리 및 팀원 간 효율적인 코드 공유/병합 진행

### 🔧 어려웠던 점

  - 게임 루프 내 시간 관리 및 객체 상태 동기화
      * `GetFrameTime()` 활용 및 deltaTime 기반 업데이트 로직 설계의 필요성 인지
  - 충돌 감지 및 객체 제거 로직 구록 및 생명 블록 생성 및 이동(장애물 블록,생명 블록 화면)

점수 및 생명 시스템 (score,생명  화면)

속도/크기 변화(점수 변화에 따른 장애물 크기 변화)

게임 오버 UI(장애물에 닿았을시 게임종료)



## 프로젝트 소감

### 🔹전재민(팀장)

> **Raylib을 처음 사용해보면서 게임 개발의 기본적인 파이프라인을 경험할 수 있었습니다. 객체 관리와 게임 상태 관리를 GameManager 클래스로 집중시킨 설계 방식이 팀원들과의 협업 및 코드 통합에 도움이 되었습니다.**

### 🔹김무진

> **C++과 Raylib을 활용하여 실제 게임의 움직임과 상호작용을 구현하는 과정이 흥미로웠습니다. 특히 장애물의 속도와 생성 빈도를 조절하며 난이도를 변화시키는 로직을 구현하면서 게임 밸런스에 대한 이해도를 높일 수 있었습니다.**

### 🔹김준용

> **게임 기능을 테스트하고 발생한 버그를 추적하는 역할을 맡으면서 코드의 작은 변경이 전체 게임 동작에 미치는 영향을 파악하는 중요성을 알게 되었습니다. README 문서를 작성하며 프로젝트의 기능과 구조를 명확하게 정리하는 연습을 할 수 있었습니다.**

-----

```
```
