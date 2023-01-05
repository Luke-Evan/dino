#include "dino2.h"

int main() {
    init();
    load_res();
    //printf("before main_event\n");//测试
    while (SDL_WaitEvent(&main_event)) {
        start_UI();
        switch (main_event.type) {
            case SDL_QUIT:
                free_quit();
                return 0;
            case SDL_KEYDOWN:
                switch (main_event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        free_quit();
                        return 0;
                    case SDLK_RETURN:
                        //printf("before play_UI\n");//测试用，可删
                        if (play_UI()){
                            //printf("after play_UI\n");//测试用，可删
                            return 0;
                        }
                    default:break;
                }
                break;
//            case SDL_MOUSEBUTTONDOWN:
//                printf("(%d,%d)\n", main_event.button.x, main_event.button.y);
//                break;
            default:break;
        }
    }
    return 0;
}
void init() {
    SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);//忽视鼠标带来的事件处理与内存占用
    SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Init(SDL_INIT_AUDIO);
    TTF_Init();
    Window =SDL_CreateWindow("dino", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);//创建窗口
    Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);//创建渲染器
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
}
void load_res(){
    //字体
    font_digit = TTF_OpenFont("fonts/digit.ttf", 400);
    font_cartoon = TTF_OpenFont("fonts/cartoon.ttf", 400);
    //图片
    start_surface[0] = TTF_RenderUTF8_Blended(font_cartoon, "Are You Ready ?", font_color0);
    start_surface[1] = TTF_RenderUTF8_Blended(font_cartoon, "Please Press ''ENTER'' to start", font_color1);
    ground_surface = IMG_Load("images/ground.png");
    cloud_surface = IMG_Load("images/cloud.png");//云
    dino_surface= IMG_Load("images/res.png");
    bird_surface[0] = IMG_Load("images/bird_0.png");
    bird_surface[1] = IMG_Load("images/bird_1.png");
    time_surface0 = TTF_RenderUTF8_Blended(font_cartoon, "TIME :", font_color2);//time是白色
    HI_surface = TTF_RenderUTF8_Blended(font_cartoon, "HI :", font_color2);//hi是白色
    cactus_surface[0] = IMG_Load("images/cac_0.png");
    cactus_surface[1] = IMG_Load("images/cac_1.png");
    cactus_surface[2] = IMG_Load("images/cac_2.png");
    over_surface[0]= IMG_Load("images/game_over.png");
    over_surface[1]= IMG_Load("images/restart.png");
    invincible= IMG_Load("images/heart.png");
    for (int i = 0; i < 5; i++) {
        dino_src[i]=(SDL_Rect){848+i*44,2,44,47};
    }
    for (int i = 5; i < 7; i++) {
        dino_src[i]=(SDL_Rect){1112+(i-5)*59,19,59,30};
    }
    //音频
    bgm= Mix_LoadMUS("sounds/Subway_Surfers.ogg");
    game_over= Mix_LoadWAV("sounds/game_over.wav");
    jump= Mix_LoadWAV("sounds/jump.wav");
}
void start_UI() {
    SDL_Texture *start_texture[2] = {NULL};
    SDL_Texture *ground_texture = SDL_CreateTextureFromSurface(Renderer, ground_surface);
    SDL_Texture *dino_texture = SDL_CreateTextureFromSurface(Renderer, dino_surface);
    SDL_Rect start_rect[2] = {{300, 100, 500, 150},
                              {200, 300, 700, 70},
                              };
    SDL_Rect ground_rect = {0, 500, 2000, 40};

    for (int i = 0; i < 2; i++) {
        start_texture[i] = SDL_CreateTextureFromSurface(Renderer, start_surface[i]);
        SDL_RenderCopy(Renderer, start_texture[i], NULL, &start_rect[i]);
        SDL_DestroyTexture(start_texture[i]);
    }
    SDL_RenderCopy(Renderer, ground_texture, NULL, &ground_rect);
    SDL_DestroyTexture(ground_texture);
    SDL_RenderCopyF(Renderer, dino_texture, &dino_src[1], &dino_dest[0]);
    SDL_DestroyTexture(dino_texture);

    SDL_RenderPresent(Renderer);

}
int play_UI() {
    play_start_time= time(NULL);
    //printf("play_start_time: %lld\n",play_start_time);//测试
    Mix_PlayMusic(bgm,-1);
    //printf("in play_UI and before play_event\n");//测试
    while (1) {
        long begin = (long) SDL_GetTicks();
        srand(global_count);
        global_count++;
        SDL_RenderClear(Renderer);
        check_die();
        check_music();
        while (SDL_PollEvent(&play_event)) {
            switch (play_event.type) {
                case SDL_QUIT:
                    free_quit();
                    return 1;
                case SDL_KEYDOWN:
                    if (play_event.key.keysym.sym==SDLK_ESCAPE) {
                            free_quit();
                            return 1;
                    }else if (play_event.key.keysym.sym==SDLK_UP){
                        if (if_jump==0){
                            if_jump=1;
                            Mix_PlayChannel(-1,jump,0);
                        }
                        break;
                    }else if (play_event.key.keysym.sym==SDLK_DOWN){
                        if_down=1;
                        break;
                    }else if (play_event.key.keysym.sym==SDLK_SPACE){
                            if (if_pause==0){
                                if (if_pause_trigger==0){
                                    if_pause_trigger=1;
                                }
                            } else{
                                if (if_pause_end==0){
                                    if_pause_end=1;
                                    music_for_pause=1;
                                }
                            }
                            break;
                    }else { break;}
                case SDL_KEYUP:
                    if(play_event.key.keysym.sym==SDLK_DOWN) {
                        if_down=0;
                        break;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    //printf("(%d,%d)\n", play_event.button.x, play_event.button.y);
                    if (if_die==1){
                        if (play_event.button.x>=over_rect[1].x &&play_event.button.x<=(over_rect[1].x+over_rect[1].w) &&play_event.button.y>=over_rect[1].y &&play_event.button.y<=(over_rect[1].y+over_rect[1].h)){
                            restart();
                        }
                    }
                    break;
                default:break;
            }
            break;
        }

        print_play();

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
void over_UI(){
    SDL_Texture *over_texture[2]={NULL};
    over_texture[0]= SDL_CreateTextureFromSurface(Renderer,over_surface[0]);
    over_texture[1]= SDL_CreateTextureFromSurface(Renderer,over_surface[1]);
    for (int i = 0; i < 2; i++) {
        SDL_RenderCopy(Renderer,over_texture[i],NULL,&over_rect[i]);
        SDL_DestroyTexture(over_texture[i]);
    }
}
void restart(){
    if_die=0;
    invin_start_time=0,if_invining=0,if_invin_able=0,old_num=0;

    if_jump=0;
    if_down=0;
    jump_y=415;
    jump_speed=init_jump;

    play_start_time= time(NULL);
    score=0;

    bird_x=init_bird;
    cactus_x=init_cac;
    speed_ground=init_ground;

    if( Mix_PausedMusic() == 1 )
    {
        Mix_ResumeMusic();
    }
}

void check_music(){
    if (if_die==1||if_pause==1){
        if (Mix_PlayingMusic()!=0&&Mix_PausedMusic()!=1){//有音乐在播放且未被暂停
            Mix_PauseMusic();
        }
    }
    if (music_for_pause==1){
        if (Mix_PlayingMusic()!=0&&Mix_PausedMusic()==1){//有音乐在播放且已经被暂停
            Mix_ResumeMusic();
            music_for_pause=0;
        }
    }

}

void print_play() {
    update_speed();//无敌在里面
    print_time();
    print_ground();
    print_cloud();
    print_scores();
    print_cactus();
    print_bird();
    print_dino();
    if (if_die==1){
        over_UI();
    }
    print_invin();
    SDL_RenderPresent(Renderer);
}
void update_speed(){
    speed_ground=init_ground+(score/interval)*2;
    if (if_invin_able==0){
        if (score/invin_interval_score>old_num){
            old_num=score/invin_interval_score;
            if_invin_able=1;
        }
    }
}
void print_invin(){
    if (if_invining==1||if_invin_able==1) {
        SDL_Texture *invin_texture = SDL_CreateTextureFromSurface(Renderer, invincible);
        SDL_Rect invin_rect = {70, 580, 60, 60};
        SDL_RenderCopy(Renderer, invin_texture, NULL, &invin_rect);
        SDL_DestroyTexture(invin_texture);
    }
}
void print_ground() {
    SDL_Texture *ground_texture = SDL_CreateTextureFromSurface(Renderer, ground_surface);
    if (ground_x <= -1024) {
        ground_x = 0;
    }
    SDL_Rect ground_rect = {ground_x, 500, 2048, 40};
    SDL_RenderCopy(Renderer, ground_texture, NULL, &ground_rect);
    SDL_DestroyTexture(ground_texture);
    if (if_die==0&&if_pause==0){
        ground_x-=speed_ground;
    }
}
void print_cloud() {
    if (cloud_x <= -1000) {
        cloud_x = 0;
    }
    SDL_Texture *cloud_texture = SDL_CreateTextureFromSurface(Renderer, cloud_surface);
    SDL_SetTextureAlphaMod(cloud_texture,200);
    SDL_Rect cloud_rect = {cloud_x, 80, 2000, 250};
    SDL_RenderCopy(Renderer, cloud_texture, NULL, &cloud_rect);
    SDL_DestroyTexture(cloud_texture);
    if (if_die==0&&if_pause==0) {
        cloud_x -= speed_cloud;
    }
}
void print_time() {
    SDL_Texture *time_texture0 = SDL_CreateTextureFromSurface(Renderer, time_surface0);
    SDL_Rect time_rect0 = {650, 120, 100, 50};
    SDL_RenderCopy(Renderer, time_texture0, NULL, &time_rect0);
    SDL_DestroyTexture(time_texture0);
    if (if_die==0) {
        if (if_pause_trigger == 1) {
            if_pause = 1;
            pause_start_time = time(NULL);
            //printf("pause_start_time:%lld\n",pause_start_time);//测试
            if_pause_trigger=0;
        }
        if (if_pause == 1) {
            if (if_pause_end==1){
                play_start_time += time(NULL)-pause_start_time;
                if_pause=0;
                if_pause_end=0;
                //printf("new play start time:%lld\n",play_start_time);//测试
            }
        }else{
            play_end_time = time(NULL);
            int DurSecond = (int) difftime(play_end_time, play_start_time);
            sprintf(time_char, "%.2d:%.2d:%.2d", DurSecond / 3600, DurSecond / 60 % 60, DurSecond % 60);
        }
    }
    time_surface1 = TTF_RenderUTF8_Blended(font_digit, time_char, font_color1);
    SDL_Texture *time_texture1 = SDL_CreateTextureFromSurface(Renderer, time_surface1);
    SDL_Rect time_rect1 = {770, 120 , 140, 50};
    SDL_RenderCopy(Renderer, time_texture1, NULL, &time_rect1);
    SDL_FreeSurface(time_surface1);
    SDL_DestroyTexture(time_texture1);
}
void print_scores() {
    SDL_Texture *HI_texture = SDL_CreateTextureFromSurface(Renderer, HI_surface);
    SDL_Rect HI_rect = {650, 50, 100, 50};
    SDL_RenderCopy(Renderer, HI_texture, NULL, &HI_rect);
    SDL_DestroyTexture(HI_texture);

    fp = fopen("best_score.txt", "r");
    fscanf(fp, "%05d", &best_score);
    fclose(fp);
    if (if_die==0&&if_pause==0){
        if (global_count % speed_score == 0) {
            score++;
        }
    }
    if (score >= best_score) {
        best_score = score;
        fp = fopen("best_score.txt", "w");
        fprintf(fp, "%05d", best_score);
        fclose(fp);
    }

    sprintf(score_char, "%05d", score);
    now_score_surface = TTF_RenderUTF8_Blended(font_digit, score_char, font_color2);//目前分数是白色
    SDL_Texture *now_score_texture = SDL_CreateTextureFromSurface(Renderer, now_score_surface);
    SDL_Rect now_score_rect = {900, 50, 100, 50};
    SDL_RenderCopy(Renderer, now_score_texture, NULL, &now_score_rect);
    SDL_FreeSurface(now_score_surface);
    SDL_DestroyTexture(now_score_texture);

    sprintf(best_score_char, "%05d", best_score);
    best_score_surface = TTF_RenderUTF8_Blended(font_digit, best_score_char, font_color0);//最高分是黄色
    SDL_Texture *best_score_texture = SDL_CreateTextureFromSurface(Renderer, best_score_surface);
    SDL_Rect best_score_rect = {770, 50, 100, 50};
    SDL_RenderCopy(Renderer, best_score_texture, NULL, &best_score_rect);
    SDL_FreeSurface(best_score_surface);
    SDL_DestroyTexture(best_score_texture);
}
void print_cactus() {
    cactus_rect=(SDL_Rect){cactus_x, 450, 65, 70};
    SDL_Texture *cactus_texture = SDL_CreateTextureFromSurface(Renderer, cactus_surface[cac_index]);
    SDL_RenderCopy(Renderer, cactus_texture, NULL, &cactus_rect);
    SDL_DestroyTexture(cactus_texture);
    if (cactus_x<-80){
        cactus_x=init_cac;
        cac_index=rand()%3;
    }
    if (if_die==0&&if_pause==0){
        cactus_x-=speed_ground;
    }
}
void print_dino(){
    if (if_jump==0){
        if (if_down==0){//直立跑
            print_run_dino0();
        }else if (if_down==1){//趴下跑
            print_run_dino1();
        }
    }else if(if_jump==1){//跳跃
        if (jump_y>415){
            if_jump=0;
            jump_speed=init_jump;
            jump_y=415;
        }
        dino_jump();
    }else{
        //printf("print_dino error\n");
    }
}
void print_run_dino0() {
    SDL_Texture *dino_texture= SDL_CreateTextureFromSurface(Renderer,dino_surface);
    if (if_die==0&&if_pause==0){
        if (dino_index0 > 3) {
            dino_index0 = 2;
        }
        SDL_RenderCopyF(Renderer, dino_texture, &dino_src[dino_index0], &dino_dest[0]);
        if(global_count%dino_change==0){
            dino_index0++;
        }
    } else{
        SDL_RenderCopyF(Renderer, dino_texture, &dino_src[4], &dino_dest[0]);
    }
    SDL_DestroyTexture(dino_texture);
}
void dino_jump(){
    const Uint8 *key_states = SDL_GetKeyboardState( NULL );
    SDL_Texture *dino_texture = SDL_CreateTextureFromSurface(Renderer, dino_surface);
    dino_rect= (SDL_FRect){DINO_X, jump_y, 100, 110};
    if (if_die==0&&if_pause==0){
        if (jump_speed<0&&key_states[SDL_SCANCODE_DOWN]){
            jump_y-=jump_speed*3;
        }
        jump_speed-=GRAVITY;
        jump_y-=jump_speed;
        SDL_RenderCopyF(Renderer,dino_texture,&dino_src[0],&dino_rect);
    }else{
        SDL_RenderCopyF(Renderer,dino_texture,&dino_src[4],&dino_rect);
    }
    SDL_DestroyTexture(dino_texture);
}
void print_run_dino1(){
    SDL_Texture *dino_texture= SDL_CreateTextureFromSurface(Renderer, dino_surface);
    if (if_die==0&&if_pause==0){
        if (dino_index1 > 6) {
            dino_index1 = 5;
        }
        SDL_RenderCopyF(Renderer, dino_texture, &dino_src[dino_index1], &dino_dest[1]);
        if(global_count%dino_change==0){
            dino_index1++;
        }
    }else{
        SDL_RenderCopyF(Renderer, dino_texture, &dino_src[4], &dino_dest[0]);
    }
    SDL_DestroyTexture(dino_texture);
}
void print_bird() {
    SDL_Texture *bird_texture[2] = {NULL};
    bird_texture[0] = SDL_CreateTextureFromSurface(Renderer, bird_surface[0]);
    bird_texture[1] = SDL_CreateTextureFromSurface(Renderer, bird_surface[1]);
    bird_rect = (SDL_Rect){bird_x, 405, 80, 40};
    if (bird_x<=-80){
        bird_x=cactus_x+bird_rand[rand()%3];
    }
    if (if_die==0&&if_pause==0){
        if (global_count % bird_change == 0) {
            bird_index++;
        }
        if (bird_index > 1) {
            bird_index = 0;
        }
        bird_x-=speed_ground;
    }
    SDL_RenderCopy(Renderer, bird_texture[bird_index], NULL, &bird_rect);
    for (int i = 0; i < 2; i++) {
        SDL_DestroyTexture(bird_texture[i]);
    }
}

void check_die(){
    int a=0;
    if (if_jump==1){//跳时死
        a+=check_collision(dino_rect,cactus_rect);
        a+=check_collision(dino_rect,bird_rect);
    } else if (if_down==1){//趴下时死
        a+=check_collision(dino_dest[1],cactus_rect);
        a+=check_collision(dino_dest[1],bird_rect);
    }else{//直立时死
        a+=check_collision(dino_dest[0],cactus_rect);
        a+=check_collision(dino_dest[0],bird_rect);
    }
    if (a>0){
        if(if_invin_able==1){
            if_invin_able=0;
            if_invining=1;
            invin_start_time=(int)play_end_time;
        } else if (if_invining==0){
            if (if_die==0){
                if_die=1;
                Mix_PlayChannel(-1,game_over,0);
            }
        }
    }
    if (if_invining==1&&(time(NULL)==invin_start_time+invin_last_time)){
        if_invining=0;
    }
}
int check_collision(SDL_FRect A,SDL_Rect B){
    //矩形的各边
    float leftA, rightA,topA,bottomA;
    int leftB, rightB,  topB,  bottomB;
    //计算矩形A的各边
    leftA = A.x;
    rightA = A.x + A.w;
    topA = A.y;
    bottomA = A.y + A.h;
    if (A.h==dino_rect.h&&A.y==dino_rect.y){//跳跃中考虑脚的前后空地
        leftA=A.x+30;
        rightA=A.x+70;
    }else if (A.h==dino_dest[0].h&&A.y==dino_dest[0].y){//跳跃后起立考虑脑袋后方空地
        leftA=A.x+55;
    }
    //计算矩形B的各边
    leftB = B.x+10;
    rightB = B.x + B.w-10;
    topB = B.y+10;
    bottomB = B.y + B.h-10;
    //如果矩形A的任意一条边都在矩形B外
    if( bottomA < (float )topB ||topA > (float )bottomB ||rightA <(float ) leftB||leftA > (float )rightB){
        return 0;
    }
    return 1;
}

void free_quit() {
    SDL_FreeSurface(ground_surface);
    SDL_FreeSurface(cloud_surface);
    SDL_FreeSurface(time_surface0);
    SDL_FreeSurface(HI_surface);
    SDL_FreeSurface(dino_surface);
    for (int i = 0; i < 2; i++) {
        SDL_FreeSurface(start_surface[i]);
    }
    for (int i = 0; i < 3; i++) {
        SDL_FreeSurface(cactus_surface[i]);
    }
    for (int i = 0; i < 2; i++) {
        SDL_FreeSurface(bird_surface[i]);
    }
    SDL_FreeSurface(invincible);
    TTF_CloseFont(font_digit);
    TTF_CloseFont(font_cartoon);
    Mix_HaltMusic();
    Mix_FreeMusic(bgm);
    Mix_FreeChunk(game_over);
    Mix_FreeChunk(jump);
    Mix_Quit();
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
    SDL_Quit();

}




