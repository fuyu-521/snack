#include <SDL.h>
#include<SDL_mixer.h>
#include <iostream>
#include<SDL_ttf.h>
#include<SDL_image.h>
#define WIDTH 1500
#define LONGTH 800

using namespace std;

struct Body
{
    SDL_Texture* body;
    SDL_Rect rectBody;
    Body* next;
};

int main(int argc, char* argv[]) {


    bool right = true;
    bool left = false;
    bool down = false;
    bool up = false;
    bool first = true;


    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window1 = SDL_CreateWindow("My Snack", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, LONGTH, SDL_WINDOW_SHOWN);


    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_Chunk* soundBack = Mix_LoadWAV_RW(SDL_RWFromFile("12.wav", "rb"), 1); 
    Mix_Chunk* soundBack1 = Mix_LoadWAV_RW(SDL_RWFromFile("123.wav", "rb"), 1);
    Mix_PlayChannel(-1, soundBack, -1);


    SDL_Surface* snack= SDL_LoadBMP("13.bmp");
    SDL_Surface* black = SDL_LoadBMP("14.bmp");
    SDL_Surface* sBody = SDL_LoadBMP("19.bmp");
    SDL_Surface* gameo = SDL_LoadBMP("15.bmp");
    SDL_Surface* sFruit = SDL_LoadBMP("as.bmp");


    SDL_Renderer* rend1 = SDL_CreateRenderer(window1, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* tSnack = SDL_CreateTextureFromSurface(rend1, snack);
    SDL_Texture* blackt = SDL_CreateTextureFromSurface(rend1, black);
    SDL_Texture* gameot = SDL_CreateTextureFromSurface(rend1, gameo);
    SDL_Texture* tFruit = SDL_CreateTextureFromSurface(rend1, sFruit);
    SDL_SetTextureBlendMode(tFruit, SDL_BLENDMODE_BLEND);
 

    //得分字体渲染变量
    char score[] = "0000";
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("font.ttf", 64);
    SDL_Color color = { 255,255,255 };
    SDL_Surface* sColor = TTF_RenderUTF8_Solid(font, "score:", color);
    SDL_Texture* tColor = SDL_CreateTextureFromSurface(rend1, sColor);


    Body* head = new Body();
    head->body = tSnack;

    SDL_Event event1;
    SDL_Rect rectBlack;
    SDL_Rect rectOver;
    SDL_Rect rectFruit;
    SDL_Rect rectScore;
    SDL_Rect rectNum;

    head->rectBody.x = 100;
    head->rectBody.y = 100;    
    head->rectBody.w = snack->w;
    head->rectBody.h = snack->h;

    rectBlack.x = 0;
    rectBlack.y = 0;
    rectBlack.w = WIDTH;
    rectBlack.h = LONGTH;

    rectOver.x = WIDTH / 2 - gameo->w / 2;
    rectOver.y = LONGTH / 2 - gameo->h / 2;
    rectOver.w = gameo->w;
    rectOver.h = gameo->h;

    rectFruit.w = sFruit->w;
    rectFruit.h = sFruit->h;
    rectFruit.x = (rand() % 1300) / 100 * 100;
    rectFruit.y = (rand() % 600) / 100 * 100;

    rectScore.x = 0;
    rectScore.y = 0;
    rectScore.w = sColor->w;
    rectScore.h = sColor->h;


    int alpha = 255;
    bool dark = true;

    bool fruit = false;

    bool quit = false;
    while (!quit) {
        //蛇是否咬到自己
        bool bite = false;
        Body* head5 = head;
        while (head5->next)
        {
            head5 = head5->next;
            if (head5->rectBody.x == head->rectBody.x && head5->rectBody.y == head->rectBody.y)
            {
                bite = true;
            }
        }
        if (bite)
        {
            SDL_RenderCopy(rend1, gameot, nullptr, &rectOver);
            SDL_RenderPresent(rend1);
            break;
        }

        //是否吃亚瑟,增长
        if (head->rectBody.x == rectFruit.x && head->rectBody.y == rectFruit.y)
        {
            fruit = false;
            Body* head3 = head;
            head = new Body();
            *head = { tSnack,head3->rectBody,head3 };
            head3->body = SDL_CreateTextureFromSurface(rend1, sBody);
            Mix_PlayChannel(-1, soundBack1, 0);
        }

        SDL_Delay(300);

        //方向调节
        while (SDL_PollEvent(&event1)) 
        {
            if ( SDL_QUIT ==event1.type) {
                quit = true;
            }
            else if (event1.type == SDL_KEYDOWN) {
                if (event1.key.keysym.sym == SDLK_RIGHT) {
                    if (left != true) {
                        right = true;
                        up = false;
                        down = false;
                    }
                }
                else if (event1.key.keysym.sym == SDLK_LEFT) {
                    if (right != true) {
                        left = true;
                        up = false;
                        down = false;
                    }
                }
                else if (event1.key.keysym.sym == SDLK_UP) {
                    if (down != true) {
                        left = false;
                        up = true;
                        right = false;
                    }
                }
                else if (event1.key.keysym.sym == SDLK_DOWN) {
                    if (up != true) {
                        left = false;
                        right = false;
                        down = true;
                    }
                }
            }
        }

        //蛇移动
        if (right)
        {
            int i = 0;
            Body* head4 = head;
            if (head4->next)
            {
                for (; head4->next->next; i++)
                    head4 = head4->next;
                head4->next->rectBody = head4->rectBody;
                for (; i > 0; i--)
                {
                    head4 = head;
                    for (int j = 1; j < i; j++)
                    {
                        head4 = head4->next;
                    }
                    head4->next->rectBody = head4->rectBody;
                }
            } 
            if (head->rectBody.x + snack->w >= WIDTH) {
                SDL_RenderCopy(rend1, gameot, nullptr, &rectOver);
                SDL_RenderPresent(rend1);
                break;
            }
            head->rectBody.x += 100;
        }
        else if (left) 
        {
            int i = 0;
            Body* head4 = head;
            if (head4->next)
            {
                for (; head4->next->next; i++)
                    head4 = head4->next;
                head4->next->rectBody = head4->rectBody;
                for (; i > 0; i--)
                {
                    head4 = head;
                    for (int j = 1; j < i; j++)
                    {
                        head4 = head4->next;
                    }
                    head4->next->rectBody = head4->rectBody;
                }
            }
            if (head->rectBody.x <= 0) {
                SDL_RenderCopy(rend1, gameot, nullptr, &rectOver);
                SDL_RenderPresent(rend1);
                break;
            }
            head->rectBody.x -= 100;
        }
        else if (up) 
        {
            int i = 0;
            Body* head4 = head;
            if (head4->next)
            {
                for (; head4->next->next; i++)
                    head4 = head4->next;
                head4->next->rectBody = head4->rectBody;
                for (; i >0; i--)
                {
                    head4 = head;
                    for (int j = 1; j < i; j++)
                    {
                        head4 = head4->next;
                    }
                    head4->next->rectBody = head4->rectBody;
                }
            }
            if (head->rectBody.y <= 0) {
                SDL_RenderCopy(rend1, gameot, nullptr, &rectOver);
                SDL_RenderPresent(rend1);
                break;
            }
            head->rectBody.y -= 100;
        }
        else if (down)
        {
            int i = 0;
            Body* head4 = head;
            if (head4->next)
            {
                for (; head4->next->next; i++)
                    head4 = head4->next;
                head4->next->rectBody = head4->rectBody;
                for (; i > 0; i--)
                {
                    head4 = head;
                    for (int j = 1; j < i; j++)
                    {
                        head4 = head4->next;
                    }
                    head4->next->rectBody = head4->rectBody;
                }
            }
            if (head->rectBody.y + snack->h >= LONGTH) 
            {
                SDL_RenderCopy(rend1, gameot, nullptr, &rectOver);
                SDL_RenderPresent(rend1);
                break;
            }
            head->rectBody.y += 100;
        }
        
        SDL_RenderCopy(rend1, blackt, nullptr, &rectBlack);

        //生成亚瑟
        if (!fruit)
        {
            fruit = true;
            rectFruit.x = (rand() % 1300)/100*100;
            rectFruit.y = (rand() % 600)/100*100;
            if (score[2] == '9')
            {
                score[1]++;
                score[2] = '0';
            }
            else if(first)
            {
                first=false;
            }
            else
            {
                score[2]++;
            }
        }

        SDL_Surface* sNum = TTF_RenderUTF8_Solid(font, score, color);
        SDL_Texture* tNum = SDL_CreateTextureFromSurface(rend1, sNum);
        rectNum.x = sColor->w;
        rectNum.y = 0;
        rectNum.w = sNum->w;
        rectNum.h = sNum->h;

        SDL_SetTextureAlphaMod(tFruit, alpha);
        SDL_RenderCopy(rend1, tFruit, nullptr, &rectFruit);
        if (dark)
        {
            alpha-=17*5;
            if (alpha == 0)
            {
                dark = false;
            }
        }
        else
        {
            alpha+=17*5;
            if (alpha == 255)
            {
                dark = true;
            }
        }
        Body* head2 = head;
        
        while (head2->next)
        {
            SDL_RenderCopy(rend1, head2->body , nullptr, &head2->rectBody);
            head2 = head2->next;
        }
        SDL_RenderCopy(rend1, head2->body, nullptr, &head2->rectBody);
        SDL_RenderCopy(rend1, tColor, nullptr, &rectScore);
        SDL_RenderCopy(rend1, tNum, nullptr, &rectNum);
        SDL_RenderPresent(rend1);
    }


    Mix_CloseAudio();


    while (quit == false) {
        while (SDL_PollEvent(&event1)) {
            if (event1.type == SDL_QUIT) {
                quit = true;
            }
        }
    }

    SDL_DestroyWindow(window1);
    SDL_Quit();
    return 0;
}