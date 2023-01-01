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
#include <SDL2/SDL_stdinc.h>

#define WIDTH 1024
#define HEIGHT 720
#define FRAMERATE 60
#undef main

void init();
void load_res();
void start_UI();
void over_UI();
int play_UI();

void print_play();
void print_ground();
void print_cloud();
void print_scores();
void print_cactus();
void print_bird();
void print_time();
void print_dino();
void print_run_dino0();
void dino_jump(int kind);
void print_run_dino1();

void mxg_box(int kind);//游戏结束，暂停的界面

void check_collision();
void restart();


void free_quit();

static SDL_Window *Window = NULL;
static SDL_Renderer *Renderer = NULL;

static SDL_Surface *dino_surface=NULL;//5种恐龙形态
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


static TTF_Font *font_digit = NULL;
static TTF_Font *font_cartoon=NULL;
static SDL_Color font_color0={255,255,100,255};//黄色
static SDL_Color font_color1= {100, 100, 255, 150};//淡紫色
static SDL_Color font_color2= {255, 255, 255, 255};//白色

static time_t play_start_time, play_end_time;//记录游戏时间
static char time_char[12];//打印时间需要的字符串

static int dino_index0=2,dino_index1=5,bird_index=0;//跑动效果
static int jump_speed=7,jump_height=160;
static int speed_dino=3,speed_bird=10,speed_score=3;//越小越快，图片切换速度
static int global_count=0,speed_ground=15,speed_cloud=3,speed_bird_forward=5;//越大越快
static int bird_x=1050,ground_x=0,cloud_x=0,cactus_x[3]={1050,1050,1050};
static int jump_y=415,if_jump=0,if_down=0,if_jump_up=0;
static int if_die = 0,quit=0;//判断是否开始/胜利/有对话框存在
static int score = 0,best_score = 0;//游戏分数与最高分
static FILE *fp=NULL;
static char score_char[10], best_score_char[10];//打印分数需要的字符串


static SDL_Rect dino_dest[2] = {{10, 415, 100, 110},{10, 455, 120, 70}};
static SDL_Rect dino_src[7]={0};
static SDL_Rect over_rect[2]={{290,250,420,125},{470,310,110,43}};

static SDL_Event main_event;
static SDL_Event play_event;
#endif //PROJECT_1__DINO2_H_
