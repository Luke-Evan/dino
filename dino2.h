//
// Created by Estrella on 2022/12/27.
//

#ifndef PROJECT_1__DINO2_H_
#define PROJECT_1__DINO2_H_


#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_stdinc.h"
#include "SDL2/SDL_mixer.h"

#define WIDTH 1024
#define HEIGHT 720
#define FRAMERATE 60

#undef main

void init();
void load_res();
void start_UI();
void over_UI();
int play_UI();

void print_play();//打印画布
void print_ground();
void print_cloud();
void print_scores();
void print_cactus();
void print_bird();
void print_time();
void print_dino();
void print_run_dino0();
void dino_jump();
void print_run_dino1();
void update_speed();
void print_invin();

void check_die();
int check_collision(SDL_FRect A,SDL_Rect B);
void restart();
void free_quit();
void check_music();

static SDL_Window *Window = NULL;
static SDL_Renderer *Renderer = NULL;
static SDL_Event main_event;
static SDL_Event play_event;
//surface
static SDL_Surface *dino_surface=NULL;
static SDL_Surface *start_surface[2] = {NULL};
static SDL_Surface *bird_surface[2] = {NULL};
static SDL_Surface *ground_surface = NULL;
static SDL_Surface *cloud_surface =NULL;
static SDL_Surface *over_surface[2]={NULL};
static SDL_Surface *time_surface0 =NULL;//time是白色
static SDL_Surface *time_surface1 =NULL;
static SDL_Surface *HI_surface =NULL;
static SDL_Surface *now_score_surface =NULL;//目前分数是白色//hi是白色
static SDL_Surface *best_score_surface = NULL;//最高分是黄色
static SDL_Surface *cactus_surface[3] = {NULL};//仙人掌
static SDL_Surface *invincible=NULL;
static TTF_Font *font_digit = NULL;//字体管理
static TTF_Font *font_cartoon=NULL;
static SDL_Color font_color0={255,255,100,255};//黄色
static SDL_Color font_color1= {100, 100, 255, 150};//淡紫色
static SDL_Color font_color2= {255, 255, 255, 255};//白色
//小恐龙相关
static int global_count=0;
static int dino_index0=2,dino_index1=5,dino_change=5;
static float GRAVITY =(float )1.1;//跳跃管理
static float jump_y=415,init_jump=20,jump_speed=20;
static int if_jump=0,if_down=0;
static SDL_FRect dino_rect={0};//跳跃的框
static SDL_FRect dino_dest[2] = {{10, 415, 100, 110},{10, 455, 120, 70}};//0是直立，1是趴下
static SDL_Rect dino_src[7]={0};
//无敌相关
static int invin_start_time=0,if_invining=0,if_invin_able=0;
static int old_num=0,invin_interval_score=500,invin_last_time=2;
//鸟相关
static int bird_index=0,bird_change=10,bird_x=1575,init_bird=1575,bird_rand[3]={6000,2985,4000};//越小越快，图片切换速度
static SDL_Rect bird_rect ={0};
//地面,云，仙人掌相关
static int speed_ground=15,init_ground=15,speed_cloud=3;//越大越快
static int ground_x=0,cloud_x=0,cactus_x=1050;
static int init_cac=1050,cac_index=0;
static SDL_Rect cactus_rect ={0};
//时间，分数相关
static time_t play_start_time, play_end_time,pause_start_time,if_pause_trigger=0,if_pause_end=0;//时间管理
static char time_char[12];
static int score = 0,best_score = 0,speed_score=3 ;//分数管理
static FILE *fp=NULL;
static char score_char[10], best_score_char[10];
static int interval=300;//加速区间
//死亡相关
static int if_die = 0,if_pause=0;//判断是否死亡
static SDL_Rect over_rect[2]={{290,250,420,85},{474,380,76,57}};


static Mix_Music *bgm=NULL;
static Mix_Chunk *game_over=NULL;
static Mix_Chunk *jump=NULL;
#endif //PROJECT_1__DINO2_H_
