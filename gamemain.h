#ifndef GAMEMAIN_H
#define GAMEMAIN_H
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <math.h>
#include "common.h"
#include "resources.h"
extern SDL_Texture *loadTexture(const char *filePath);
extern SDL_Texture *loadRenderText(const char *text, SDL_Color color);
struct level {
    int level_num;
    int level_goal;
    int total_res;
    struct resources *reses;
};
level *getLevel(int lvl)
{
    if(lvl == 1) {
        level *currentLvl = (level *)malloc(sizeof(level));
        currentLvl->level_num = lvl;
        currentLvl->level_goal = 500;
        currentLvl->total_res = 6;
        resources *res = (resources *)malloc(sizeof(resources) * 6);

        res[0].id = ID_DIAMOND;
        res[0].position.x = 600; res[0].position.y = 300;

        res[1].id = ID_BOMB;
        res[1].position.x = 100; res[1].position.y = 200;

        res[2].id = ID_GOLD100;
        res[2].position.x = 400; res[2].position.y = 350;

        res[3].id = ID_GOLD500;
        res[3].position.x = 200; res[3].position.y = 400;

        res[4].id = ID_ROCK10;
        res[4].position.x = 700; res[4].position.y = 100;

        res[5].id = ID_GOLD250;
        res[5].position.x = 300; res[5].position.y = 200;

        currentLvl->reses = res;
        return currentLvl;
    } else if(lvl == 2) {
        level *currentLvl = (level *)malloc(sizeof(level));
        currentLvl->level_num = lvl;
        currentLvl->level_goal = 700;
        currentLvl->total_res = 8;

        resources *res = (resources *)malloc(sizeof(resources) * 8);

        res[0].id = ID_BOMB;
        res[0].position.x = 600; res[0].position.y = 300;

        res[1].id = ID_BOMB;
        res[1].position.x = 100; res[1].position.y = 200;

        res[2].id = ID_GOLD100;
        res[2].position.x = 400; res[2].position.y = 350;

        res[3].id = ID_GOLD250;
        res[3].position.x = 200; res[3].position.y = 400;

        res[4].id = ID_GOLD500;
        res[4].position.x = 700; res[4].position.y = 100;

        res[5].id = ID_ROCK10;
        res[5].position.x = 300; res[5].position.y = 200;

        res[6].id = ID_ROCK10;
        res[6].position.x = 500; res[6].position.y = 100;

        res[7].id = ID_GOLD250;
        res[7].position.x = 200; res[7].position.y = 250;

        currentLvl->reses = res;
        return currentLvl;
    } else if(lvl == 3) {
        level *currentLvl = (level *)malloc(sizeof(level));
        currentLvl->level_num = lvl;
        currentLvl->level_goal = 1000;//
        currentLvl->total_res = 10;

        resources *res = (resources *)malloc(sizeof(resources) * 10);

        res[0].id = ID_DIAMOND;
        res[0].position.x = 600; res[0].position.y = 300;

        res[1].id = ID_BOMB;
        res[1].position.x = 100; res[1].position.y = 200;

        res[2].id = ID_BOMB;
        res[2].position.x = 400; res[2].position.y = 350;

        res[3].id = ID_GOLD100;
        res[3].position.x = 200; res[3].position.y = 400;

        res[4].id = ID_GOLD100;
        res[4].position.x = 700; res[4].position.y = 100;

        res[5].id = ID_ROCK20;
        res[5].position.x = 300; res[5].position.y = 200;

        res[6].id = ID_DIAMOND;
        res[6].position.x = 500; res[6].position.y = 100;

        res[7].id = ID_ROCK10;
        res[7].position.x = 200; res[7].position.y = 250;

        res[8].id = ID_DIAMOND;
        res[8].position.x = 400; res[8].position.y = 200;

        res[9].id = ID_DIAMOND;
        res[9].position.x = 700; res[9].position.y = 250;

        currentLvl->reses = res;
        return currentLvl;
    }

    return NULL;
}

void destroyLevel(level *lvl)
{
	free(lvl->reses);
	free(lvl);
	lvl = NULL;
}
TTF_Font *gameFont = NULL;
SDL_Color fontColor = { 255, 255, 0 };
int userGrade = 0;
bool rectImpact(SDL_Rect a, SDL_Rect b) {
    int leftA = a.x;
    int rightA = a.x + a.w;
    int topA = a.y;
    int bottomA = a.y + a.h;
    int leftB = b.x;
    int rightB = b.x + b.w;
    int topB = b.y;
    int bottomB = b.y + b.h;
    if (bottomA  < topB || topA > bottomB || rightA < leftB || leftA > rightB) {
        return false;
    } else {
        return true;
    }
}
void displayLevelImage(int levelNumber) {
    char imageFilePath[50];
    snprintf(imageFilePath, sizeof(imageFilePath), "res/img/target%d.png", levelNumber);
    SDL_Texture *levelImage = loadTexture(imageFilePath);
    SDL_RenderClear(winRenderer);
    SDL_RenderCopy(winRenderer, levelImage, NULL, NULL);
    SDL_RenderPresent(winRenderer);

    SDL_Delay(2000);
    SDL_DestroyTexture(levelImage);
}
int gameMain(level *level)
{
	SDL_Texture *gameBg, *resTexture[level->total_res],  *timeTexture = NULL, *levelTexture, *targetTexture, *gradeTexture, *hook, *line;
	SDL_Rect resRect[level->total_res], timeRect, levelRect, targetRect, gradeRect, hookRect, lineRect;
	SDL_Point startpoint, hook_point, line_point;
	resources resProp;
	int startTime, levelTime, hookTimer, lineTimer;
	int catchedRes = -1;
	bool running = true, hook_fired = false, hook_go_right = true, hook_go_back = false;
	levelTime = SDL_GetTicks();
	char levelStr[4] = { 0 };
	char targetStr[10] = { 0 };
	double hook_angle = 20, lineLen = 0;
	targetStr[0] = '$';

	gameBg = loadTexture(imgFile[ID_BACKGROUND]);
    SDL_snprintf(levelStr, sizeof(levelStr), "%d", level->level_num);
    levelTexture = loadRenderText(levelStr, fontColor);
	hook = loadTexture(imgFile[ID_HOOK]);
	line = loadTexture(imgFile[ID_LINE]);
    SDL_snprintf(&targetStr[1], sizeof(targetStr) - 1, "%d", level->level_goal);
	targetTexture = loadRenderText(targetStr, fontColor);
	for(int i = 0; i < level->total_res; i++) {
		resTexture[i] = loadTexture(imgFile[level->reses[i].id]);
		resRect[i].x = level->reses[i].position.x;
		resRect[i].y = level->reses[i].position.y;
		SDL_QueryTexture(resTexture[i], NULL, NULL, &resRect[i].w, &resRect[i].h);
	}

	startpoint.x = 500;
	startpoint.y = 100;
	timeRect.x = 800; timeRect.y = 15;
	timeRect.w = 26; timeRect.h = 40;
	levelRect.x = 800; levelRect.y = 60;
	levelRect.w = 8 ;
	levelRect.h = 45;
	targetRect.x = 150; targetRect.y = 60;
	targetRect.w = 50 ;
	targetRect.h = 45;
	gradeRect.x = 180; gradeRect.y = 20;
	gradeRect.h = 45;
	hookTimer = 0;
	lineTimer = 0;
	lineLen = 0;
	catchedRes = -1;

	SDL_QueryTexture(hook, NULL, NULL, &hookRect.w, &hookRect.h);
	hookRect.x = startpoint.x - hookRect.w;
	hookRect.y = startpoint.y -21;
	hook_point.x = hookRect.w / 2;
	hook_point.y = 0;
	lineRect.x = startpoint.x - hookRect.w / 2;
	lineRect.y = startpoint.y -21;
	lineRect.h = 0;
	lineRect.w = 3;
	line_point.x = 2;
	line_point.y = 0;

	while(running) {
		char timeStr[3] = { 0 };
		char gradeStr[10] = { 0 };
		startTime = SDL_GetTicks();
		if((startTime - levelTime) / 1000 > 60)
			running = false;
		while(SDL_PollEvent(&keyEvent)) {
			if(keyEvent.type == SDL_QUIT)
				running = false;
			if(keyEvent.type == SDL_KEYDOWN)
				if(keyEvent.key.keysym.sym == SDLK_DOWN && !hook_fired)
					hook_fired = true;
		}

		if(!hook_fired) {
			if(SDL_GetTicks() - hookTimer > 17) {
				hookTimer = SDL_GetTicks();
				if(hook_go_right) {
					hook_angle += 1.5;
					if(hook_angle >= 165)
						hook_go_right = false;
				} else {
					hook_angle -= 1.5;
					if(hook_angle <= 15)
						hook_go_right = true;
				}
			}
		} else {
			if(SDL_GetTicks() - lineTimer > 20) {
				lineTimer = SDL_GetTicks();
				if(!hook_go_back) {
					lineLen += 5;
				}
				else if(resProp.id == -1)
					lineLen -= 6;
				else {
					lineLen -= 6 /resProp.weight;
					if(hook_angle <= 90) {
						resRect[catchedRes].x = hookRect.x - resRect[catchedRes].w / 2;
						resRect[catchedRes].y = hookRect.y - resRect[catchedRes].h / 2 + 35;
					} else {
						resRect[catchedRes].x = hookRect.x + resRect[catchedRes].w / 2 - 30;
						resRect[catchedRes].y = hookRect.y - resRect[catchedRes].h / 2 + 35;
					}
				}
				if(lineLen < 0)
					lineLen = 0;
			}
    if (lineLen <= 1) {
    hook_fired = false;
    hook_go_back = false;
    if (catchedRes != -1) {
        userGrade += resProp.score;
        if (userGrade >= level->level_goal) {
          return 2;
        }
        resProp.setId(-1);
					SDL_DestroyTexture(resTexture[catchedRes]);
					resTexture[catchedRes] = NULL;
        catchedRes = -1;
    }
}
}

			if(lineLen >= 500)
				hook_go_back = true;
			if(hook_angle <= 90) {
				hookRect.x = startpoint.x - hookRect.w - abs(cos(hook_angle / 180.0 * M_PI) * lineLen);
				hookRect.y = startpoint.y + abs(sin(hook_angle / 180.0 * M_PI) * lineLen) - 20;
			} else {
				hookRect.x = startpoint.x - hookRect.w + abs(cos(hook_angle / 180.0 * M_PI) * lineLen);
				hookRect.y = startpoint.y  + abs(sin(hook_angle / 180.0 * M_PI) * lineLen) - 20;
			}
		gradeStr[0] = '$';
		SDL_itoa(userGrade, &gradeStr[1], 10);
		gradeTexture = loadRenderText(gradeStr, fontColor);
		gradeRect.w = 30;
		lineRect.h = lineLen;

		if(timeTexture)
			SDL_DestroyTexture(timeTexture);
		timeTexture = loadRenderText(SDL_itoa(60 - ((startTime - levelTime) / 1000), timeStr, 10), fontColor);

		SDL_RenderCopy(winRenderer, gameBg, NULL, NULL);
		SDL_RenderCopy(winRenderer, levelTexture, NULL, &levelRect);
		SDL_RenderCopy(winRenderer, timeTexture, NULL, &timeRect);
		SDL_RenderCopy(winRenderer, targetTexture, NULL, &targetRect);
		SDL_RenderCopy(winRenderer, gradeTexture, NULL, &gradeRect);
		SDL_RenderCopyEx(winRenderer, hook, NULL, &hookRect, 90 - hook_angle, &hook_point, SDL_FLIP_NONE);
		SDL_RenderCopyEx(winRenderer, line, NULL, &lineRect, 90 - hook_angle, &line_point, SDL_FLIP_NONE);

		for(int i = 0; i < level->total_res; i++) {
			if(resTexture[i])
				SDL_RenderCopy(winRenderer, resTexture[i], NULL, &resRect[i]);
		}
		SDL_RenderPresent(winRenderer);

		if(catchedRes == -1)
for(int i = 0; i < level->total_res; i++) {
    if(resTexture[i] == NULL) {
        continue;
    }

    if(rectImpact(hookRect, resRect[i])) {
        if (level->reses[i].id == ID_BOMB) {
            SDL_DestroyTexture(resTexture[i]);
            hook_go_back = true;
            userGrade=userGrade-500;
            resTexture[i] = NULL;
            printf("Caught a bomb! It disappeared!\n");
        } else {
            catchedRes = i;
            resProp.setId(level->reses[i].id);
            hook_go_back = true;
            printf("Caught!\n");
        }
        break;
    }
}

	}

	for(int i = 0; i < level->total_res; i++) {
		if(resTexture[i])
			SDL_DestroyTexture(resTexture[i]);
	}
	SDL_DestroyTexture(gameBg);
	SDL_DestroyTexture(timeTexture);
	SDL_DestroyTexture(levelTexture);
	return userGrade;
}

void gameOver(bool win)
{
	if(win)
		printf("You Win %d\n", userGrade);
	else
		printf("You Lose %d\n", userGrade);
}

int startGame() {
    int lvlNum = 1;
    bool win = false;
    level *lvl = NULL;
    gameFont = TTF_OpenFont("res/default.ttf", 40);

    while (1) {
        displayLevelImage(lvlNum); // Hiển thị ảnh trước khi vào mỗi cấp độ
        userGrade = 0;
        lvl = getLevel(lvlNum);
        if (!lvl) {
            win = true;
            gameOver(win);
            break;
        }
        int result = gameMain(lvl);
        if (result == 2)
        {
            destroyLevel(lvl);
            lvlNum++;
            continue;
        }
        else
        {
            gameOver(win);
            break;
        }
        destroyLevel(lvl);
    }
    return 1;
}




#endif
