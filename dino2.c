#include "dino2.h"

int main() {
    init();
    printf("before main_event\n");//测试
    while (SDL_WaitEvent(&main_event)) {
        printf("1");
        start_UI();
        SDL_RenderClear(Renderer);
        switch (main_event.type) {
            case SDL_QUIT:
                free_quit();
                return 0;
            case SDL_KEYDOWN:
                switch (main_event.key.keysym.sym) {
                    case SDLK_ESCAPE:free_quit();
                        return 0;
                    case SDLK_RETURN:
                        printf("before play_UI\n");//测试用，可删
                        if (play_UI()) {
                            return 0;
                        }
                        printf("after play_UI\n");//测试用，可删
                        break;
                    default:break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:printf("(%d,%d)\n", main_event.button.x, main_event.button.y);
                break;
            default:break;
        }
    }
    return 0;
}

void init() {
    SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);//忽视鼠标带来的事件处理与内存占用
    SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    Window =SDL_CreateWindow("dino", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);//创建窗口
    Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);//创建渲染器
    font_digit = TTF_OpenFont("fonts/digit.ttf", 400);//初始化字体
    font_cartoon = TTF_OpenFont("fonts/cartoon.ttf", 400);
}

void start_UI() {
    SDL_Surface *start_surface[2] = {NULL};
    SDL_Texture *start_texture[2] = {NULL};
    start_surface[0] = TTF_RenderUTF8_Blended(font_cartoon, "Are You Ready ?", font_color0);
    start_surface[1] = TTF_RenderUTF8_Blended(font_cartoon, "Please Press ''ENTER'' to start", font_color1);
    dino_surface[4] = IMG_Load("images/dino_4.png");
    SDL_Surface *ground_surface = IMG_Load("images/ground.png");
    SDL_Texture *ground_texture = SDL_CreateTextureFromSurface(Renderer, ground_surface);
    dino_texture[4] = SDL_CreateTextureFromSurface(Renderer, dino_surface[4]);
    SDL_Rect start_rect[3] = {{300, 100, 500, 150},
                              {200, 300, 700, 70},
                              {10, 400, 105, 140}};
    SDL_Rect ground_rect = {0, 500, 2000, 40};
    for (int i = 0; i < 2; i++) {
        start_texture[i] = SDL_CreateTextureFromSurface(Renderer, start_surface[i]);
        SDL_RenderCopy(Renderer, start_texture[i], NULL, &start_rect[i]);
    }
    SDL_RenderCopy(Renderer, ground_texture, NULL, &ground_rect);
    SDL_RenderCopy(Renderer, dino_texture[4], NULL, &start_rect[2]);

    SDL_RenderPresent(Renderer);
    SDL_FreeSurface(ground_surface);
    SDL_DestroyTexture(ground_texture);

    SDL_FreeSurface(dino_surface[4]);
    SDL_DestroyTexture(dino_texture[4]);

    for (int i = 0; i < 2; i++) {
        SDL_FreeSurface(start_surface[i]);
    }
    for (int i = 0; i < 2; i++) {
        SDL_DestroyTexture(start_texture[i]);
    }
}

int play_UI() {
    play_start_time= time(NULL);
    printf("in play_UI and before play_event\n");
    while (1) {
        printf("2");
        long begin = (long) SDL_GetTicks();
        SDL_RenderClear(Renderer);
        global_count++;
        print_play();
        while (SDL_PollEvent(&play_event)) {
            switch (play_event.type) {
                case SDL_QUIT:
                    free_quit();
                    return 1;
                case SDL_KEYDOWN:
                    switch (play_event.key.keysym.sym) {
                        case SDLK_w:
                        case SDLK_UP:
                            dino_jump();
                            break;
                        case SDLK_s:
                        case SDLK_DOWN:print_run_dino1();
                            break;
                        default:break;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:printf("(%d,%d)\n", play_event.button.x, play_event.button.y);
                    break;
                default:break;
            }
            break;
        }
        //控制帧频
        long current = (long) SDL_GetTicks();
        long cost = current - begin;
        long frame = 1000 / FRAMERATE;
        long delay = frame - cost;
        if (delay > 0) {
            SDL_Delay(delay);
        }
    }
}

void print_play() {
    print_ground();
    print_cloud();
    print_time();
    print_scores();
    print_cactus();
    if (if_jump==0&&if_jump_down==0){
        print_run_dino0();
    }
    print_bird();
    SDL_RenderPresent(Renderer);
}
void print_ground() {
    SDL_Surface *ground_surface = IMG_Load("images/ground.png");//地面
    SDL_Texture *ground_texture = SDL_CreateTextureFromSurface(Renderer, ground_surface);
    if (ground_x <= -1024) {
        ground_x = 0;
    }
    ground_texture = SDL_CreateTextureFromSurface(Renderer, ground_surface);
    SDL_Rect ground_rect = {ground_x, 500, 2048, 40};
    SDL_RenderCopy(Renderer, ground_texture, NULL, &ground_rect);
    SDL_FreeSurface(ground_surface);
    SDL_DestroyTexture(ground_texture);
    ground_x-=speed_ground;
}
void print_cloud() {
    if (cloud_x <= -1000) {
        cloud_x = 0;
    }
    SDL_Surface *  cloud_surface = IMG_Load("images/cloud.png");//云
    SDL_Texture *cloud_texture = SDL_CreateTextureFromSurface(Renderer, cloud_surface);
    SDL_SetTextureAlphaMod(cloud_texture,200);
    SDL_Rect cloud_rect = {cloud_x, 80, 2000, 250};
    SDL_RenderCopy(Renderer, cloud_texture, NULL, &cloud_rect);
    SDL_DestroyTexture(cloud_texture);
    cloud_x-=speed_cloud;
}
void print_time() {
    SDL_Surface *time_surface0 = TTF_RenderUTF8_Blended(font_cartoon, "TIME :", font_color2);//time是白色
    SDL_Texture *time_texture0 = SDL_CreateTextureFromSurface(Renderer, time_surface0);
    SDL_Rect time_rect0 = {650, 120, 100, 50};
    SDL_RenderCopy(Renderer, time_texture0, NULL, &time_rect0);
    SDL_FreeSurface(time_surface0);
    SDL_DestroyTexture(time_texture0);

    play_end_time = time(NULL);
    int DurSecond = (int) difftime(play_end_time, play_start_time);
    sprintf(time_char, "%.2d:%.2d:%.2d", DurSecond / 3600, DurSecond / 60 % 60, DurSecond % 60);
    SDL_Surface *time_surface1 = TTF_RenderUTF8_Blended(font_digit, time_char, font_color1);
    SDL_Texture *time_texture1 = SDL_CreateTextureFromSurface(Renderer, time_surface1);
    SDL_Rect time_rect1 = {770, 120 , 140, 50};
    SDL_RenderCopy(Renderer, time_texture1, NULL, &time_rect1);
    SDL_FreeSurface(time_surface1);
    SDL_DestroyTexture(time_texture1);
}
void print_scores() {
    SDL_Surface *hi_surface = TTF_RenderUTF8_Blended(font_cartoon, "HI :", font_color2);//hi是白色
    SDL_Texture *hi_texture = SDL_CreateTextureFromSurface(Renderer, hi_surface);
    SDL_Rect hi_rect = {650, 50, 100, 50};
    SDL_RenderCopy(Renderer, hi_texture, NULL, &hi_rect);
    SDL_FreeSurface(hi_surface);
    SDL_DestroyTexture(hi_texture);

    fp = fopen("best_score.txt", "r");
    fscanf(fp, "%05d", &best_score);
    fclose(fp);

    if (global_count % speed_score == 0) {
        score++;
    }
    if (score >= best_score) {
        best_score = score;
        fp = fopen("best_score.txt", "w");
        fprintf(fp, "%05d", best_score);
        fclose(fp);
    }

    sprintf(score_char, "%05d", score);
    SDL_Surface *now_score_surface = TTF_RenderUTF8_Blended(font_digit, score_char, font_color2);//目前分数是白色
    SDL_Texture *now_score_texture = SDL_CreateTextureFromSurface(Renderer, now_score_surface);
    SDL_Rect now_score_rect = {900, 50, 100, 50};
    SDL_RenderCopy(Renderer, now_score_texture, NULL, &now_score_rect);
    SDL_FreeSurface(now_score_surface);
    SDL_DestroyTexture(now_score_texture);

    sprintf(best_score_char, "%05d", best_score);
    SDL_Surface *best_score_surface = TTF_RenderUTF8_Blended(font_digit, best_score_char, font_color0);//最高分是黄色
    SDL_Texture *best_score_texture = SDL_CreateTextureFromSurface(Renderer, best_score_surface);
    SDL_Rect best_score_rect = {770, 50, 100, 50};
    SDL_RenderCopy(Renderer, best_score_texture, NULL, &best_score_rect);
    SDL_FreeSurface(best_score_surface);
    SDL_DestroyTexture(best_score_texture);
}
void print_cactus() {
    //障碍物随机出现
    int a;
    for (int i = 0; i <3 ; i++) {
        if (cactus_x[i] <= -80) {
            srand((unsigned)time(NULL));//重新设置随机数种子
            a = rand() % 2000 + 1050;
            cactus_x[i] = a;
        }
    }
    SDL_Surface *cactus_surface[3] = {NULL};//仙人掌
    SDL_Texture *cactus_texture[3] = {NULL};
    cactus_surface[0] = IMG_Load("images/cac_0.png");
    cactus_surface[1] = IMG_Load("images/cac_1.png");
    cactus_surface[2] = IMG_Load("images/cac_2.png");
    for (int i = 0; i < 3; i++) {
        cactus_texture[i] = SDL_CreateTextureFromSurface(Renderer, cactus_surface[i]);
    }
    SDL_Rect cactus_rect[3] = {{cactus_x[0], 450, 65, 70},
                               {cactus_x[1], 450, 65, 70},
                               {cactus_x[2], 440, 96, 80}
    };

    SDL_RenderCopy(Renderer, cactus_texture[0], NULL, &cactus_rect[0]);
    cactus_x[0]-=speed_ground;

    //其余障碍物随后开始
    if(global_count>=150){
        SDL_RenderCopy(Renderer,cactus_texture[1],NULL,&cactus_rect[1]);
        cactus_x[1]-=speed_ground;
    }

    if(global_count>=300){
        SDL_RenderCopy(Renderer,cactus_texture[2],NULL,&cactus_rect[2]);
        cactus_x[2]-=speed_ground;
    }


    for (int i = 0; i < 3; i++) {
        SDL_FreeSurface(cactus_surface[i]);
    }
    for (int i = 0; i < 3; i++) {
        SDL_DestroyTexture(cactus_texture[i]);
    }
}
void print_run_dino0() {
    if (dino_index0 > 1) {
        dino_index0 = 0;
    }
    dino_surface[0] = IMG_Load("images/dino_0.png");
    dino_surface[1] = IMG_Load("images/dino_1.png");
    dino_texture[0] = SDL_CreateTextureFromSurface(Renderer, dino_surface[0]);
    dino_texture[1] = SDL_CreateTextureFromSurface(Renderer, dino_surface[1]);
    SDL_Rect dino_rect = {10, 415, 100, 110};
    SDL_RenderCopy(Renderer, dino_texture[dino_index0], NULL, &dino_rect);
    if(global_count%speed_dino==0){
        dino_index0++;
    }

    for (int i = 0; i < 2; i++) {
        SDL_FreeSurface(dino_surface[i]);
    }
    for (int i = 0; i < 2; i++) {
        SDL_DestroyTexture(dino_texture[i]);
    }

}
void print_bird() {
    SDL_Surface *bird_surface[2] = {NULL};
    SDL_Texture *bird_texture[2] = {NULL};
    if (bird_index > 1) {
        bird_index = 0;
    }

    bird_surface[0] = IMG_Load("images/bird_0.png");
    bird_surface[1] = IMG_Load("images/bird_1.png");
    bird_texture[0] = SDL_CreateTextureFromSurface(Renderer, bird_surface[0]);
    bird_texture[1] = SDL_CreateTextureFromSurface(Renderer, bird_surface[1]);
    SDL_Rect bird_rect = {bird_x, 220, 100, 50};
    SDL_RenderCopy(Renderer, bird_texture[bird_index], NULL, &bird_rect);

    bird_x-=speed_bird_forward;
    if (global_count % speed_bird == 0) {
        bird_index++;
    }

    for (int i = 0; i < 2; i++) {
        SDL_FreeSurface(bird_surface[i]);
    }
    for (int i = 0; i < 2; i++) {
        SDL_DestroyTexture(bird_texture[i]);
    }
}
void dino_jump(){
    long begin = (long) SDL_GetTicks();
    dino_surface[4]= IMG_Load("images/dino_4.png");
    dino_texture[4] = SDL_CreateTextureFromSurface(Renderer, dino_surface[4]);
    SDL_Rect dino_rect = {10, jump_y, 100, 110};

    while (jump_y>=300){
        jump_y-=5;
        SDL_RenderCopy(Renderer,dino_texture[4],NULL,&dino_rect);
        SDL_RenderPresent(Renderer);
        //控制帧频
        long current = (long) SDL_GetTicks();
        long cost = current - begin;
        long frame = 1000 / FRAMERATE;
        long delay = frame - cost;
        if (delay > 0) {
            SDL_Delay(delay);
        }
    }
    while (jump_y<=415){
        jump_y+=5;
        SDL_RenderCopy(Renderer,dino_texture[4],NULL,&dino_rect);
        SDL_RenderPresent(Renderer);
        //控制帧频
        long current = (long) SDL_GetTicks();
        long cost = current - begin;
        long frame = 1000 / FRAMERATE;
        long delay = frame - cost;
        if (delay > 0) {
            SDL_Delay(delay);
        }
    }

    SDL_FreeSurface(dino_surface[4]);
    SDL_DestroyTexture(dino_texture[4]);
}
void print_run_dino1(){
    if (dino_index1 > 1) {
        dino_index1 = 0;
    }
    dino_surface[2] = IMG_Load("images/dino_2.png");
    dino_surface[3] = IMG_Load("images/dino_3.png");
    dino_texture[2] = SDL_CreateTextureFromSurface(Renderer, dino_surface[2]);
    dino_texture[3] = SDL_CreateTextureFromSurface(Renderer, dino_surface[3]);
    SDL_Rect dino_rect = {10, 470, 70, 50};
    SDL_RenderCopy(Renderer, dino_texture[dino_index1], NULL, &dino_rect);
    if(global_count%speed_dino==0){
        dino_index1++;
    }

    for (int i = 2; i < 4; i++) {
        SDL_FreeSurface(dino_surface[i]);
    }
    for (int i = 2; i < 4; i++) {
        SDL_DestroyTexture(dino_texture[i]);
    }
}
void free_quit() {

    for (int i = 0; i < 5; i++) {
        SDL_FreeSurface(dino_surface[i]);
    }

    for (int i = 0; i < 5; i++) {
        SDL_DestroyTexture(dino_texture[i]);
    }

    TTF_CloseFont(font_digit);
    TTF_CloseFont(font_cartoon);
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);

    SDL_Quit();

}




