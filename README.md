물론입니다! 아래는 위의 내용을 `README.md` 형식에 맞게 정리한 Markdown 파일입니다. 이 파일을 그대로 복사해 `.md` 확장자로 저장하시면 GitHub 등에서 바로 문서로 활용하실 수 있습니다.

> 👉 프로젝트 문서 자동 변환과 생성은 [gptonline.ai](https://gptonline.ai/ko/)에서 더 편리하게 이용해보세요!

---

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

```mermaid
classDiagram
    GameManager --|> Player
    GameManager --|> Obstacle
    GameManager : -score
    GameManager : -obstacleSpeed
    GameManager : -obstacleSize
    GameManager : -gameState
    GameManager : +player
    GameManager : +obstacles
    GameManager : +InitGame()
    GameManager : +UpdateGame()
    GameManager : +DrawGame()
    GameManager : +CheckCollisions()
    GameManager : +UpdateScore()
    GameManager : +ChangeObstacleProperties()
    GameManager : +HandleInput()

    class Player {
        +Vector2 position
        +Vector2 size
        +Color color
        +float speed
        +Draw()
        +Update(float deltaTime)
    }

    class Obstacle {
        +Vector2 position
        +float radius
        +Color color
        +float speed
        +Draw()
        +Update(float deltaTime)
        +IsOffScreen() bool
    }
````

## 🛠️ 핵심 기능 요약

1. **게임 초기화 및 루프 구성**

   * raylib로 게임 창 생성 및 기본 구조 작성
   * `InitGame()`, `UpdateGame()`, `DrawGame()` 구조로 게임 루프 구성
  // 예시: main 함수 일부
```
int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Raylib Avoid Game");

    GameManager game;
    game.InitGame(screenWidth, screenHeight); // 게임 초기화

    while (!WindowShouldClose()) {
        game.HandleInput();                // 입력 처리
        game.UpdateGame(GetFrameTime());   // 게임 로직 업데이트
        game.DrawGame();                   // 게임 화면 그리기
    }

    CloseWindow(); // 창 닫기
    return 0;
}
```
InitGame()에서 주인공과 장애물 초기화

UpdateGame()에서 게임 상태 업데이트 및 충돌 체크

DrawGame()에서 화면에 모든 객체 그리기


2. **주인공 이동 구현**

   * Player 클래스 활용
   * 화살표 또는 WASD 키로 이동 제어, 화면 경계 내 제한
  ```
     void Player::Update(float deltaTime, int screenWidth, int screenHeight) {
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) position.x += speed * deltaTime;
    if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) position.x -= speed * deltaTime;
    if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) position.y += speed * deltaTime;
    if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) position.y -= speed * deltaTime;

    // 화면 경계 체크
    if (position.x < 0) position.x = 0;
    if (position.x > screenWidth - size.x) position.x = screenWidth - size.x;
    if (position.y < 0) position.y = 0;
    if (position.y > screenHeight - size.y) position.y = screenHeight - size.y;
}

void Player::Draw() {
    DrawRectangleV(position, size, color);
}
```
    키 입력에 따른 이동
    
    화면 밖으로 나가지 않도록 경계 제한 적용



3. **장애물 생성 및 이동**

   * Obstacle 클래스 활용
   * 자동 생성 및 이동, 화면 밖으로 나가면 제거
 
  ```
void Obstacle::Update(float deltaTime) {
    position.x -= speed * deltaTime; // 왼쪽으로 이동
}

void Obstacle::Draw() {
    DrawCircleV(position, radius, color);
}

bool Obstacle::IsOffScreen() {
    return position.x + radius < 0; // 화면 왼쪽 밖으로 나갔는지
}
```

4. **충돌 감지 및 게임 오버**

   * `CheckCollisionCircleRec()` 함수 사용
   * 충돌 시 게임 상태를 GAME\_OVER로 전환
  
```
void GameManager::CheckCollisions() {
    Rectangle playerRect = { player.position.x, player.position.y, player.size.x, player.size.y };

    for (const auto& obstacle : obstacles) {
        if (CheckCollisionCircleRec(obstacle.position, obstacle.radius, playerRect)) {
            gameState = GAME_OVER;
            break;
        }
    }
}
```

5. **점수 시스템**

   * 장애물이 화면 밖으로 나가면 점수 10점 증가
```
obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(),
    [&](const Obstacle& o){
        if (o.position.x + o.radius < 0) {
            score += 10;
            return true;
        }
        return false;
    }), obstacles.end());
```
6. **속도/크기 변화**

   * 점수나 시간 경과에 따라 장애물 속도/크기 점진적 증가


```
void GameManager::UpdateGame(float deltaTime) {
    currentObstacleSpeed += speedIncreaseRate * deltaTime;
    currentObstacleSize += sizeIncreaseRate * deltaTime;

    Obstacle newObstacle;
    newObstacle.speed = currentObstacleSpeed;
    newObstacle.radius = GetRandomValue(minSize, maxSize); // 또는 currentObstacleSize 사용
    // ...
}
```

7. **게임 오버 UI**

   * 게임 종료 시 “Game Over”, 최종 점수 출력
  
```
void GameManager::DrawGame() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (gameState == GAME_PLAYING) {
        player.Draw();
        for (auto& o : obstacles) o.Draw();
        DrawText(TextFormat("Score: %i", score), 10, 10, 20, BLACK);
    } else if (gameState == GAME_OVER) {
        const char* gameOverText = "Game Over!";
        DrawText(gameOverText, GetScreenWidth()/2 - MeasureText(gameOverText, 40)/2, GetScreenHeight()/2 - 20, 40, RED);
        DrawText(TextFormat("Final Score: %i", score), GetScreenWidth()/2 - MeasureText(TextFormat("Final Score: %i", score), 20)/2, GetScreenHeight()/2 + 20, 20, BLACK);
    }

    EndDrawing();
}

```

## 📅 개발 일정

| 일차  | 주요 작업                                 | 참여            |
| --- | ------------------------------------- | ------------- |
| 1일차 | 주제 선정, 일정 관리, 기획 및 설계     | 김무진,김준용,전재민
| 2일차 | 
| 3일차 | 
| 4일차 |

## ✅ 성과 및 개선점

### 성공 요인

* (작성 예정)

### 어려웠던 점

* (작성 예정)

### 보완할 점 및 개선 방향

* (작성 예정)

## 📸 실행 화면

1. **게임 초기 화면** (게임 시작화면 없이 바로 게임구현)

   

2. **주인공 이동 구현** 

 

3. **장애물 생성 및 이동**(장애물 화면)



4. **점수 시스템** (score 점수 화면)



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
