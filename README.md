# 🎮 Raylib 장애물 피하기 게임

## 📌 프로젝트 개요
- **팀명**: 9팀
- **팀원**: 전재민(팀장), 김준용, 김무진
- **개발 기간**: 2025년 MM월 DD일 ~ 2025년 MM월 DD일 (총 X일)
- **주요 기술**: C++, raylib 라이브러리, 게임 루프, 충돌 감지, 객체 관리, (필요시 추가)

## 👥 팀원 역할 분담
| 이름 | 역할 | 주요 담당 업무 |
|------|-----|---------------|
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

    Note right of GameManager: 게임 전체 상태,\n객체 목록 관리,\n핵심 로직 수행

🛠️ 핵심 기능
1. 게임 초기화 및 기본 루프
raylib 함수를 사용하여 게임 창 생성, 기본 변수 초기화 (주인공 위치, 점수, 장애물 목록 등).
InitGame() 함수에서 게임 시작 시 필요한 설정 수행.
게임 루프(while (!WindowShouldClose())) 안에서 UpdateGame(), DrawGame() 함수 계속 호출.


// 예시: main 함수 일부
#include "raylib.h"
// #include "GameManager.h" // 게임 관리 클래스 헤더 포함 필요

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Raylib Avoid Game");
    // SetTargetFPS(60); // 선택 사항: 프레임 속도 고정

    // GameManager game; // 게임 관리 객체 생성
    // game.InitGame(screenWidth, screenHeight); // 게임 초기화

    // while (!WindowShouldClose()) // 게임 루프
    // {
    //     game.HandleInput(); // 입력 처리
    //     game.UpdateGame(GetFrameTime()); // 게임 상태 업데이트 ( deltaTime 전달)
    //     game.DrawGame(); // 화면 그리기
    // }

    CloseWindow(); // 창 닫기
    return 0;
}


2. 주인공 (막대기) 구현 및 이동
Player 클래스로 주인공 객체 구현. 위치, 크기, 색상 정보 가짐.
키보드 입력(화살표 키 또는 WASD)에 따라 주인공의 x 또는 y 위치 변경.
화면 경계를 넘어가지 않도록 이동 제한 기능 추가.

// 예시: Player 클래스 Update 함수 일부
// void Player::Update(float deltaTime, int screenWidth, int screenHeight) {
//     // 키 입력에 따른 위치 변경
//     if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) position.x += speed * deltaTime;
//     if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) position.x -= speed * deltaTime;
//     if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) position.y += speed * deltaTime;
//     if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) position.y -= speed * deltaTime;

//     // 화면 경계 제한
//     if (position.x < 0) position.x = 0;
//     if (position.x > screenWidth - size.x) position.x = screenWidth - size.x;
//     if (position.y < 0) position.y = 0;
//     if (position.y > screenHeight - size.y) position.y = screenHeight - size.y;
// }

// 예시: Player 클래스 Draw 함수
// void Player::Draw() {
//     DrawRectangleV(position, size, color);
// }


3. 장애물 (원) 구현 및 관리
Obstacle 클래스로 원 형태 장애물 객체 구현. 위치, 반지름, 색상, 속도 정보 가짐.
일정 간격으로 새로운 장애물을 화면 밖에서 생성하고, 화면 가로질러 이동하도록 Update 함수 구현.
화면 밖으로 완전히 나간 장애물은 목록에서 제거하여 메모리 관리.

// 예시: Obstacle 클래스 Update 함수 일부
// void Obstacle::Update(float deltaTime) {
//     position.x -= speed * deltaTime; // 왼쪽으로 이동 (예시)
//     // ... 다른 이동 방향 구현 가능
// }

// 예시: Obstacle 클래스 Draw 함수
// void Obstacle::Draw() {
//     DrawCircleV(position, radius, color);
// }

// 예시: GameManager에서 장애물 생성/관리 (UpdateGame 함수 일부)
// ... 장애물 생성 타이머 로직 ...
// Obstacle newObstacle = { ... }; // 새로운 장애물 생성
// obstacles.push_back(newObstacle);
// ... 장애물 업데이트 루프 ...
// obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(),
//     [](const Obstacle& o){ /* 화면 밖 나갔는지 체크 */ return o.isOffScreen(); }), obstacles.end()); // 화면 밖 장애물 제거


4. 충돌 감지 및 게임 오버
CheckCollisionCircleRec() 함수 등을 사용하여 주인공(사각형)과 각 장애물(원) 간의 충돌 여부 확인.
충돌 발생 시 게임 상태를 '게임 오버'로 변경하고 게임 루프 제어.

// 예시: GameManager CheckCollisions 함수 일부
// void GameManager::CheckCollisions() {
//     Rectangle playerRect = { player.position.x, player.position.y, player.size.x, player.size.y };
//     for (const auto& obstacle : obstacles) {
//         // raylib 함수 사용
//         if (CheckCollisionCircleRec(obstacle.position, obstacle.radius, playerRect)) {
//             gameState = GAME_OVER; // 충돌 발생 시 게임 오버 상태로 전환
//             break; // 충돌했으니 더 이상 검사할 필요 없음
//         }
//     }
// }

5. 점수 시스템 구현 (10점/장애물)
장애물이 주인공을 지나쳐 화면 밖으로 나갈 때마다 점수 10점 증가.
장애물 제거 로직과 통합하여 구현하는 방식 고려.

// 예시: GameManager UpdateGame 함수 일부 (장애물 제거와 함께 점수 증가)
// ... 장애물 업데이트 루프 ...
// obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(),
//     [&](const Obstacle& o){
//         // 장애물이 화면 왼쪽 끝을 넘어갔는지 확인 (오른쪽에서 온다고 가정)
//         if (o.position.x + o.radius < 0) {
//             score += 10; // 점수 증가
//             return true; // 제거 대상
//         }
//         return false; // 제거 대상 아님
//     }), obstacles.end());


6. 장애물 속도 및 크기 변화 기능
게임이 진행될수록 장애물의 이동 속도 또는 생성될 때 크기를 점진적으로 증가시키거나 무작위로 조절.
점수, 시간 등 게임 진행 상태에 따라 속도/크기 변화 로직 구현.

// 예시: GameManager 내 속성 변경 로직 (UpdateGame 또는 별도 함수)
// void GameManager::UpdateGame(...) {
//     // ... 게임 진행 시간 또는 점수에 따라 속도/크기 증가 로직 ...
//     currentObstacleSpeed += speedIncreaseRate * deltaTime; // 시간에 따라 속도 증가
//     currentObstacleSize += sizeIncreaseRate * deltaTime; // 시간에 따라 크기 증가

//     // ... 장애물 생성 시 속도/크기 적용 ...
//     // newObstacle.speed = currentObstacleSpeed;
//     // newObstacle.radius = GetRandomValue(minSize, maxSize); // 또는 currentObstacleSize 사용
// }

7. 게임 오버 화면 표시
게임 상태가 '게임 오버'일 때, 화면에 'Game Over!' 메시지와 최종 점수 표시.
필요시 재시작 또는 종료 선택 기능 추가 고려.

// 예시: GameManager DrawGame 함수 일부
// void GameManager::DrawGame() {
//     BeginDrawing();
//     ClearBackground(RAYWHITE); // 배경 지우기

//     // 게임 플레이 중일 때 그릴 것들
//     if (gameState == GAME_PLAYING) {
//         // ... 주인공 그리기 (player.Draw()) ...
//         // ... 장애물들 그리기 (for loop call obstacle.Draw()) ...
//         DrawText(TextFormat("Score: %i", score), 10, 10, 20, BLACK); // 점수 표시
//     }
//     // 게임 오버일 때 그릴 것들
//     else if (gameState == GAME_OVER) {
//         const char* gameOverText = "Game Over!";
//         int textWidth = MeasureText(gameOverText, 40);
//         DrawText(gameOverText, GetScreenWidth()/2 - textWidth/2, GetScreenHeight()/2 - 20, 40, RED);
//         DrawText(TextFormat("Final Score: %i", score), GetScreenWidth()/2 - MeasureText(TextFormat("Final Score: %i", score), 20)/2, GetScreenHeight()/2 + 20, 20, BLACK);
//         // ... 재시작/종료 안내 문구 등 ...
//     }

//     EndDrawing(); // 그리기 종료
// }

📅 개발 일정
일차	주요 작업	참여
1일차	raylib 환경 설정, 기본 게임 창/루프 구현. 주인공(막대기) 클래스 설계 및 기본 그리기 코드 작성.	전재민(기획/설계), 김무진(구현), 김준용(환경설정/문서 기초)
2일차	주인공 이동 로직 구현 (입력 처리 포함). 장애물(원) 클래스 설계 및 기본 그리기/이동 코드 작성.	김무진(구현), 전재민(기획/설계 피드백)
3일차	장애물 생성 로직 구현 (자동 생성 및 제거). 주인공-장애물 충돌 감지 구현 및 게임 오버 상태 전환. 점수 시스템 (10점/장애물) 구현.	김무진(구현), 전재민(로직 검토)
4일차	장애물 속도/크기 변화 기능 구현. 게임 시작/오버 화면 표시 로직 구현. 전체 통합 테스트 및 버그 수정. README 문서 작성 및 최종 검토.	김무진(통합/수정), 김준용(문서/테스트), 전재민(최종 검토/관리)


💡 성과 및 개선점
✅ 성공 요인

🔧 어려웠던 점

📌 보완할 점 및 개선 방향

📄실행화면


프로젝트 소감
🔹전재민 (프로젝트 관리 / 기획 및 설계)


🔹김무진 (개발)


🔹김준용 (문서 / 테스트)
