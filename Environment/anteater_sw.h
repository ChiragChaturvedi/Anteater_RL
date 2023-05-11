//
//  anteater_sw.h
//  Anteater
//
//  Created by Chirag Chaturvedi on 5/4/23.
//
#include <list>
#include <pthread.h>
using namespace std;

#ifndef anteater_sw_h
#define anteater_sw_h
typedef struct{
    int (*map)[10];
    pthread_rwlock_t rwlock;
} game_map;

typedef struct{
    bool up;
    bool down;
    bool right;
    bool left;
    bool retract;
} action_space;

typedef struct{
    int x_pos;
    int y_pos;
} position;

typedef struct{
    position tt_pos;
    list<position> *trace;
} tongue_tip;

typedef struct{
    int sprite_id;
    int sprite_type;
    int prev_item;
    position sprite_position;
    int direction;
    int reward;
    int speed;
} sprite;

typedef struct{
    list<sprite> *sprites;
    pthread_rwlock_t rwlock;
    int no_of_sprites;
} sprite_manager;

typedef struct{
    int (*map)[10];
    int no_of_sprites;
    tongue_tip my_tt;
    int lives;
    int reward;
    int no_of_larvae;
} state_space;

#endif /* anteater_sw_h */
