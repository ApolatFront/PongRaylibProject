/*
Pong Game Features:
Ball Moving
Ball recognizing borders and collisions
Game over at 6 points
Moveable Paddles
Player vs Player
Player vs CPU

!!Missing or not working features!!:
When ball hits the Paddle from above or below, the ball goes through the paddle (not working)
Add scene for choosing game mode
Add Field Size
Add Difficulty 
Leaderboard (MAYBE)
Better UI
Use assets


*/

#include <iostream>
#include <raylib.h>

using namespace std;

typedef enum Screen { TITLE, GAME1,GAME2, ENDING } Screen;
Screen current_screen = TITLE;

class Paddle {
protected:
    void LimitMovement() {
        if (y <= 30) {
            y = 30;
        }
        if (y + height >= GetScreenHeight()) {
            y = GetScreenHeight() - height;
        }
    }

public:
    float x, y;
    float width, height;
    int speed;

    void Draw() {
        DrawRectangle(x, y, width, height, WHITE);
    }

    void Update(bool isSecondPlayer) {


        if (isSecondPlayer == false) {
            if (IsKeyDown(KEY_W)) {
                y -= speed;
            }

            if (IsKeyDown(KEY_S) || y >= GetScreenHeight()) {
                y += speed;
            }
        }
        else if (isSecondPlayer == true) {
            if (IsKeyDown(KEY_UP)) {
                y -= speed;
            }

            if (IsKeyDown(KEY_DOWN) || y >= GetScreenHeight()) {
                y += speed;
            }
        }
        
        LimitMovement();
        
    }
};

class CpuPaddle : public Paddle {
public:
    void Update(int ball_y) {
        if (y + height / 2 > ball_y) {
            y -= speed;
        }
        if (y + height / 2 <= ball_y) {
            y += speed;
        }

        LimitMovement();
    }
};

class Ball {
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw() {

        DrawCircle(x, y, radius, WHITE);
    }

    void Update(Paddle paddle, Paddle paddle2) {
        x += speed_x;
        y += speed_y;

            

        if (y - radius <= 30 || y + radius >= GetScreenHeight()) {
            speed_y *= -1;
        }

        
    }

    void ResetBall() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = { -1 , 1 };
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }

};

const int screen_width = 1280;
const int screen_height = 800;

Ball ball;
Paddle paddle;
Paddle paddle2;
CpuPaddle cpuPaddle;
int scorePlayer1;
int scorePlayer2;

int main() {

    

    //GameOver and Texttimer variables
    bool isGameOver = false;
    bool speedIncreased = false;
    bool displaySpeedMessage = false;
    int messageTimer = 0; 
    const int MESSAGE_DURATION = 180;
    bool cpuGame = false;

    cout << "Game is starting..." << endl;
    
    

    //Ball
    ball.radius = 20;
    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 5;
    ball.speed_y = 5;

    //Paddles
    paddle.width = 25;
    paddle.height = 150;
    paddle.x = 10;
    paddle.y = screen_height / 2 - paddle.height / 2;
    paddle.speed = 5;

    
    paddle2.width = 25;
    paddle2.height = 150;
    paddle2.x = screen_width - paddle2.width - 10;
    paddle2.y = screen_height / 2 - paddle.height / 2;
    paddle2.speed = 5;

    cpuPaddle.width = 25;
    cpuPaddle.height = 150;
    cpuPaddle.x = screen_width - paddle2.width - 10;
    cpuPaddle.y = screen_height / 2 - paddle.height / 2;
    cpuPaddle.speed = 5;

    InitWindow(screen_width, screen_height, "Pong Game (Tutorial)");
    SetTargetFPS(120);
    while (WindowShouldClose() == false) {
        BeginDrawing();
        ClearBackground(BLACK);

        switch (current_screen) {
        case TITLE: {
            DrawText("PONG GAME", screen_width / 2 - 300, 50, 100, WHITE);
            DrawText("By Ahmed Polat", screen_width / 2 - 300, 150, 30, WHITE);
            DrawText("1. Player vs Player", screen_width / 2 - 200, 350, 40, WHITE);
            DrawText("2. Player vs CPU", screen_width / 2 - 200, 450, 40, WHITE);
            DrawText("3. Exit", screen_width / 2 - 200, 550, 40, WHITE);
            if (IsKeyPressed(KEY_ONE)) {
                current_screen = GAME1;
                scorePlayer1 = 0;
                scorePlayer2 = 0;
                paddle.x = 10;
                paddle.y = screen_height / 2 - paddle.height / 2;

                paddle2.x = screen_width - paddle2.width - 10;
                paddle2.y = screen_height / 2 - paddle.height / 2;
                speedIncreased = false;
                cpuGame = false;
            }
            else if (IsKeyPressed(KEY_TWO)) {
                current_screen = GAME2;
                scorePlayer1 = 0;
                scorePlayer2 = 0;

                paddle.x = 10;
                paddle.y = screen_height / 2 - paddle.height / 2;

                cpuPaddle.x = screen_width - paddle2.width - 10;
                cpuPaddle.y = screen_height / 2 - paddle.height / 2;
                speedIncreased = false;
                cpuGame = true;
            }
            else if (IsKeyPressed(KEY_THREE)) {
                exit(0);
            }
        }break;
        case GAME1: {
            
            if (WindowShouldClose() == false) {


                ball.Update(paddle, paddle2);
                paddle.Update(false);
                paddle2.Update(true);


                if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ paddle.x, paddle.y , paddle.width, paddle.height })) {
                    ball.speed_x *= -1;
                }



                if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ paddle2.x, paddle2.y, paddle2.width, paddle2.height })) {
                    ball.speed_x *= -1;
                }


                ball.Draw();
                paddle.Draw();
                paddle2.Draw();
                DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
                int dot_spacing = 15;
                for (int x = 0; x < screen_width; x += 2 * dot_spacing) {
                    DrawLine(x, 25, x + dot_spacing, 25, WHITE);
                }




                if (ball.x >= GetScreenWidth()) {
                    scorePlayer1++;
                    ball.ResetBall();


                }

                if (ball.x <= 0) {
                    scorePlayer2++;
                    ball.ResetBall();

                }

                if ((scorePlayer1 >= 3 || scorePlayer2 >= 3) && !speedIncreased) {
                    ball.speed_x *= 1.2;
                    ball.speed_y *= 1.2;
                    paddle.speed *= 1.3;
                    paddle2.speed *= 1.3;
                    speedIncreased = true;


                    displaySpeedMessage = true;
                    messageTimer = MESSAGE_DURATION;
                }

                if (messageTimer > 0) {
                    messageTimer--;
                }
                else {
                    displaySpeedMessage = false;
                }

                if (displaySpeedMessage) {
                    DrawText("Speed was increased!", screen_width / 2 - 190, 50, 40, BLUE);
                }

                if (scorePlayer1 == 0 && scorePlayer2 == 0) {
                    speedIncreased = false;
                }

                DrawText(TextFormat("Score: %02i", scorePlayer1), 0, 0, 20, GREEN);
                DrawText(TextFormat("Score: %02i", scorePlayer2), screen_width - 100, 00, 20, GREEN);

                if (scorePlayer1 == 6 && scorePlayer2 < 6) {
                    current_screen = ENDING;

                }

                if (scorePlayer1 < 6 && scorePlayer2 == 6) {
                    current_screen = ENDING;

                }
            }
            

        }break;
        case GAME2: {
            if (WindowShouldClose() == false) {


                ball.Update(paddle, cpuPaddle);
                paddle.Update(false);
                cpuPaddle.Update(ball.y);


                if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ paddle.x, paddle.y , paddle.width, paddle.height })) {
                    ball.speed_x *= -1;
                }



                if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ cpuPaddle.x, cpuPaddle.y, cpuPaddle.width, cpuPaddle.height })) {
                    ball.speed_x *= -1;
                }


                ball.Draw();
                paddle.Draw();
                cpuPaddle.Draw();
                DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
                int dot_spacing = 15;
                for (int x = 0; x < screen_width; x += 2 * dot_spacing) {
                    DrawLine(x, 25, x + dot_spacing, 25, WHITE);
                }




                if (ball.x >= GetScreenWidth()) {
                    scorePlayer1++;
                    ball.ResetBall();


                }

                if (ball.x <= 0) {
                    scorePlayer2++;
                    ball.ResetBall();

                }

                if ((scorePlayer1 >= 3 || scorePlayer2 >= 3) && !speedIncreased) {
                    ball.speed_x *= 1.2;
                    ball.speed_y *= 1.2;
                    paddle.speed *= 1.3;
                    paddle2.speed *= 1.3;
                    speedIncreased = true;


                    displaySpeedMessage = true;
                    messageTimer = MESSAGE_DURATION;
                }

                if (messageTimer > 0) {
                    messageTimer--;
                }
                else {
                    displaySpeedMessage = false;
                }

                if (displaySpeedMessage) {
                    DrawText("Speed was increased!", screen_width / 2 - 190, 50, 40, BLUE);
                }

                if (scorePlayer1 == 0 && scorePlayer2 == 0) {
                    speedIncreased = false;
                }

                DrawText(TextFormat("Score: %02i", scorePlayer1), 0, 0, 20, GREEN);
                DrawText(TextFormat("Score: %02i", scorePlayer2), screen_width - 100, 00, 20, GREEN);

                if (scorePlayer1 == 6 && scorePlayer2 < 6) {
                    current_screen = ENDING;

                }

                if (scorePlayer1 < 6 && scorePlayer2 == 6) {
                    current_screen = ENDING;

                }
            }
        }break;
        case ENDING: {
            if (scorePlayer1 == 6 && scorePlayer2 < 6) {
                DrawText("Player 1 has won!", screen_width / 2 - 190, 50, 40, BLUE);

            }

            if (scorePlayer1 < 6 && scorePlayer2 == 6) {
                DrawText("Player 2 has won!", screen_width / 2 - 190, 50, 40, BLUE);

            }
            DrawText("GAME OVER", screen_width / 2 - 250, screen_height / 2 - 150, 80, WHITE);
            DrawText("1. Back to menu", screen_width / 2 - 150, screen_height / 2 + 80, 40, WHITE);
            DrawText("2. Replay", screen_width / 2 - 150, screen_height / 2 + 160, 40, WHITE);
            DrawText("3. Exit", screen_width / 2 - 150, screen_height / 2 + 240, 40, WHITE);

            if (IsKeyPressed(KEY_ONE)) {
                current_screen = TITLE;
            }
            else if (IsKeyPressed(KEY_TWO)) {
                if (cpuGame == false) {
                    current_screen = GAME1;
                    paddle.x = 10;
                    paddle.y = screen_height / 2 - paddle.height / 2;


                    paddle2.x = screen_width - paddle2.width - 10;
                    paddle2.y = screen_height / 2 - paddle.height / 2;
                    speedIncreased = false;
                    cpuGame = false;

                }
                else if (cpuGame == true) {
                    current_screen = GAME2;
                    paddle.x = 10;
                    paddle.y = screen_height / 2 - paddle.height / 2;

                    cpuPaddle.x = screen_width - paddle2.width - 10;
                    cpuPaddle.y = screen_height / 2 - paddle.height / 2;
                    speedIncreased = false;
                    cpuGame = true;

                }
                scorePlayer1 = 0;
                scorePlayer2 = 0;
            }
            else if (IsKeyPressed(KEY_THREE)) {
                exit(0);
            }
            
        }break;
        }

        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}