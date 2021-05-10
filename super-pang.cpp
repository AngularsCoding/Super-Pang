#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <time.h>
#include <stdlib.h>
#include <graphics.h>
#include <cmath>

#define ScreenX1 0
#define ScreenY1 0
#define ScreenX2 1000
#define ScreenY2 700

using namespace std;

int BallHitPlayer(int j);
void DrawFrame();
void DrawPlayer();
void DrawBullet();
void DrawBall(int j);
int BulletHit(int j);
void GenerateBall(int j);
void SplitBall(int j);
void YouWin();
void YouLose();

static double d2r(double d) {
  return (d / 180.0) * ((double) M_PI);
}

int bullet[2] = {
  0,
  0
};
int playerX = 500;
int playerSize = 50;
int ballSize = 50;
int ballIndex = 1;
int balls[50][5] = {
  {
    0,
    0,
    50,
    250,
    1
  } //x, y, size, pos, direction
};
char option;
int ballsLeft = 0;
int chances = 3;
char txtChances[2] = "3";
int score = 0;
char txtScore[3] = "0";
int ghostTimer = 0;
int gameRunning = 1;
int main() {

  int gd = DETECT, gm, err, tmp;
  initgraph( & gd, & gm, "C:\\tc\\bgi");

  setlinestyle(SOLID_LINE, 15, 0);

  while (gameRunning) {

    if (kbhit()) {
      option = getch();
      switch (option) {
      case 'a':
        playerX -= 10;
        if (playerX < 1) playerX = 0;
        break;
      case 'd':
        playerX += 10;
        if (playerX + 30 > 1000) playerX = 970;
        break;
      case 32:
        bullet[0] = playerX + playerSize / 2;
        bullet[1] = 700;
        break;
      }
    }

    DrawFrame();
    DrawPlayer();
    DrawBullet();
    if (bullet[1] > 0) {
      bullet[1] -= 10;
    }

    ballsLeft = 0;
    for (int j = 0; j < ballIndex; j++) {

      GenerateBall(j);
      DrawBall(j);

      if (BallHitPlayer(j) && ghostTimer == 0) {
        balls[j][4] = -balls[j][4];
        //balls[j][1] = 100;
        chances--;
        txtChances[0] = chances + 48;

        if (chances <= 0) {
          chances = 0;
          txtChances[0] = chances + 48;
          YouLose();
          gameRunning = 0;
          break;
        }
        ghostTimer = 10;
      }

      if (BulletHit(j) == 1) {
        score++;
        if (score > 9) {
          txtScore[0] = (score / 10) + 48;
          txtScore[1] = (score % 10) + 48;
        } else {
          txtScore[0] = '0';
          txtScore[1] = score + 48;
        }
        //delay(1000);
        bullet[0] = bullet[1] = 0;

        SplitBall(j);

        for (int k = 0; k < ballIndex; k++) {
          if (balls[k][2] > 0) {
            ballsLeft = 1;
          }
        }

        if (ballsLeft == 0) {
          YouWin();
          gameRunning = 0;
          break;
        }

        ballIndex++;
      }

      balls[j][3] += balls[j][4];
      if (balls[j][3] - (balls[j][2] / 2) < 1) {
        balls[j][4] = 1;
      }
      if (balls[j][3] + (balls[j][2] / 2) > ScreenX2 / 2) {
        balls[j][4] = -1;
      }
    }
    if (ghostTimer > 0) {
      ghostTimer--;
    }

    delay(5);
    cleardevice();
  }

  closegraph();
  return 0;
}

int BallHitPlayer(int j) {
  if (balls[j][1] + balls[j][2] >= 690) {
    if (balls[j][0] > playerX && balls[j][0] < playerX + playerSize) {
      return 1;
    }
  }
  return 0;
}

void DrawFrame() {
  rectangle(ScreenX1, ScreenY1, ScreenX2, ScreenY2);
  int ScreenW = 1000;

  settextstyle(SANS_SERIF_FONT, 0, 5);
  outtextxy(ScreenW + 40, 20, "Super Pang");

  settextstyle(SANS_SERIF_FONT, 0, 4);
  outtextxy(ScreenW + 130, 100, "Score");

  settextstyle(SANS_SERIF_FONT, 0, 6);
  outtextxy(ScreenW + 150, 150, txtScore);

  settextstyle(SANS_SERIF_FONT, 0, 4);
  outtextxy(ScreenW + 100, 250, "Chances");

  settextstyle(SANS_SERIF_FONT, 0, 6);
  outtextxy(ScreenW + 150, 300, txtChances);

  settextstyle(SANS_SERIF_FONT, 0, 1);
  outtextxy(ScreenW + 20, 500, "Controls");

  settextstyle(SANS_SERIF_FONT, 0, 2);
  outtextxy(ScreenW + 20, 550, "A - Move Left");
  outtextxy(ScreenW + 20, 580, "D - Move Right");
  outtextxy(ScreenW + 20, 610, "Spacebar - Fire");

}

void DrawPlayer() {
  setfillstyle(8, BLUE);
  bar(playerX, 690, playerX + playerSize, 700);
}

void DrawBullet() {
  circle(bullet[0], bullet[1], 5);
}

void DrawBall(int j) {
  setfillstyle(8, YELLOW);
  fillellipse(balls[j][0], balls[j][1], balls[j][2], balls[j][2]);
}

int BulletHit(int j) {
  if (bullet[0] > balls[j][0] - balls[j][2] && bullet[0] < balls[j][0] + balls[j][2]) {
    if (bullet[1] > balls[j][1] - balls[j][2] && bullet[1] < balls[j][1] + balls[j][2]) {
      return 1;
    }
  }
  return 0;
}

void GenerateBall(int j) {
  balls[j][0] = balls[j][3] * 2;
  balls[j][1] = (ScreenY2 - balls[j][2]) - ((balls[j][2] / 10) * abs(100 * sin(d2r(balls[j][3]))));
}

void SplitBall(int j) {
  balls[j][2] = balls[j][2] - 10;

  balls[ballIndex][0] = balls[j][0];
  balls[ballIndex][1] = balls[j][1];
  balls[ballIndex][2] = balls[j][2];
  balls[ballIndex][3] = balls[j][3];
  balls[ballIndex][4] = -balls[j][4];

}

void YouWin() {
  cleardevice();
  settextstyle(EUROPEAN_FONT, 0, 8);
  outtextxy(450, 300, "You Win");

  settextstyle(GOTHIC_FONT, 0, 3);
  outtextxy(470, 400, "Press any key to quit...");

  getch();
}

void YouLose() {
  cleardevice();
  settextstyle(EUROPEAN_FONT, 0, 8);
  outtextxy(450, 300, "You Lose");

  settextstyle(GOTHIC_FONT, 0, 3);
  outtextxy(470, 400, "Press any key to quit...");

  getch();

}
