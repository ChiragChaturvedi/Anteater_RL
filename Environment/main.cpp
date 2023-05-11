//
//  main.cpp
//  Anteater
//
//  Created by Chirag Chaturvedi on 5/4/23.
//

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <stdbool.h>
#include "anteater_sw.h"
#include <string.h>
#include <pthread.h>
#include <list>

using namespace std;

int game_on = 1;
int level_on = 1;
int lives = 3;
int reward = 0;
int small_map_larvae = 40;
int no_of_larvae = small_map_larvae;
int large_map_x = 28;
int large_map_y = 24;

int small_map_x = 10;
int small_map_y = 12;
int event = 0;
int done = 0;
int start = 0;
int time_step = 0;
int action_sp[5];
int retract_speed = 2;
//Shared Resource            0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27
int default_map[24][28] = { {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 2, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5}, //0     // malloc for size
                            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6}, //1
                            {5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 5, 5}, //2
                            {5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 5, 5}, //3
                            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6}, //4
                            {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5}, //5
                            {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5}, //6
                            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6}, //7
                            {5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5}, //8
                            {5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5}, //9
                            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6}, //10
                            {5, 5, 5, 0, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5}, //11
                            {5, 5, 5, 0, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5}, //12
                            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6}, //13
                            {5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5}, //14
                            {5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5}, //15
                            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6}, //16
                            {5, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5}, //17
                            {5, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5}, //18
                            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6}, //19
                            {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5}, //20
        };


//Shared Resource            0  1  2  3  4  5  6  7  8  9
int small_map[12][10] = {   {5, 5, 5, 5, 2, 5, 5, 5, 5, 5}, //0     // malloc for size
                            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6}, //1
                            {5, 5, 5, 0, 5, 0, 5, 5, 5, 5}, //2
                            {5, 5, 5, 0, 5, 0, 5, 5, 5, 5}, //3
                            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6}, //4
                            {5, 5, 0, 5, 5, 5, 5, 0, 5, 5}, //5
                            {5, 5, 0, 5, 5, 5, 5, 0, 5, 5}, //6
                            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6}, //7
                            {5, 0, 5, 5, 5, 5, 0, 5, 5, 5}, //8
                            {5, 0, 5, 5, 5, 5, 0, 5, 5, 5}, //9
                            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6}, //10
                            {5, 5, 5, 5, 5, 5, 5, 5, 5, 5}, //11
        };

// 0 -> Empty Space
// 1 -> Tongue
// 2 -> Tip
// 3 -> Ant
// 4 -> Worm
// 5 -> Block
// 6 -> Larvae

//This Function initialises the map of the game
/*
int init_grid(game_map *game_map){

    game_map->map = (int (*)[28]) malloc(sizeof(int[24][28]));
    memcpy(game_map->map, default_map, sizeof(default_map));

    int result = pthread_rwlock_init(&game_map->rwlock, NULL);
    if (result != 0) {
        fprintf(stderr, "Error: pthread_rwlock_init failed with error code %d\n", result);
        return -1;
    }

    return 0;
}
 */
int init_small_grid(game_map *game_map){

    game_map->map = (int (*)[10]) malloc(sizeof(int[12][10]));
    memcpy(game_map->map, small_map, sizeof(small_map));

    int result = pthread_rwlock_init(&game_map->rwlock, NULL);
    if (result != 0) {
        fprintf(stderr, "Error: pthread_rwlock_init failed with error code %d\n", result);
        return -1;
    }

    return 0;
}


int get_action_space(tongue_tip *tt, game_map *my_game_map, action_space *current_action_space){
    
    current_action_space->retract = false;
    
    if(tt->tt_pos.x_pos < 1)
        current_action_space->left = false;
    
    else if (my_game_map->map[tt->tt_pos.y_pos][tt->tt_pos.x_pos - 1] == 0 || my_game_map->map[tt->tt_pos.y_pos][tt->tt_pos.x_pos - 1] == 6 || my_game_map->map[tt->tt_pos.y_pos][tt->tt_pos.x_pos - 1] == 3)
        current_action_space->left = true;
    
    else if (my_game_map->map[tt->tt_pos.y_pos][tt->tt_pos.x_pos - 1] == 1){
        current_action_space->retract = true;
        current_action_space->left = false;
    }
    
    else
        current_action_space->left = false;
    
    
    if(tt->tt_pos.x_pos >= small_map_x - 1)
        current_action_space->right = false;
    
    else if (my_game_map->map[tt->tt_pos.y_pos][tt->tt_pos.x_pos + 1] == 0 || my_game_map->map[tt->tt_pos.y_pos][tt->tt_pos.x_pos + 1] == 6 || my_game_map->map[tt->tt_pos.y_pos][tt->tt_pos.x_pos + 1] == 3)
        current_action_space->right = true;
    
    else if (my_game_map->map[tt->tt_pos.y_pos][tt->tt_pos.x_pos + 1] == 1){
        current_action_space->retract = true;
        current_action_space->right = false;
    }
    
    else
        current_action_space->right = false;
    
    
    if(tt->tt_pos.y_pos < 1)
        current_action_space->up = false;
    
    else if (my_game_map->map[tt->tt_pos.y_pos - 1][tt->tt_pos.x_pos] == 0 || my_game_map->map[tt->tt_pos.y_pos - 1][tt->tt_pos.x_pos] == 6 || my_game_map->map[tt->tt_pos.y_pos - 1][tt->tt_pos.x_pos] == 3)
        current_action_space->up = true;
    
    else if (my_game_map->map[tt->tt_pos.y_pos - 1][tt->tt_pos.x_pos] == 1){
        current_action_space->retract = true;
        current_action_space->up = false;
    }
    
    else
        current_action_space->up = false;
    
    
    if(tt->tt_pos.y_pos > small_map_y - 2)
        current_action_space->down = false;
    
    else if (my_game_map->map[tt->tt_pos.y_pos + 1][tt->tt_pos.x_pos] == 0 || my_game_map->map[tt->tt_pos.y_pos + 1][tt->tt_pos.x_pos] == 6 || my_game_map->map[tt->tt_pos.y_pos + 1][tt->tt_pos.x_pos] == 3)
        current_action_space->down = true;
    
    else if (my_game_map->map[tt->tt_pos.y_pos + 1][tt->tt_pos.x_pos] == 1){
        current_action_space->retract = true;
        current_action_space->down = false;
    }
    
    else
        current_action_space->down = false;
    
    current_action_space->up ? action_sp[0] = 1 : action_sp[0] = 0;
    current_action_space->right ? action_sp[1] = 1 : action_sp[1] = 0;
    current_action_space->left ? action_sp[2] = 1 : action_sp[2] = 0;
    current_action_space->down ? action_sp[3] = 1 : action_sp[3] = 0;
    current_action_space->retract ? action_sp[4] = 1 : action_sp[4] = 0;
        
    
    //cout << "Got current action space \n";
    return 0;
}

/*
 Actions:
 UP = 0
 Right = 1
 Left = 2
 Down = 3
 Retract = 4
 Do Nothing = 5
 */

// 0 -> Empty Space
// 1 -> Tongue
// 2 -> Tip
// 3 -> Ant
// 4 -> Worm
// 5 -> Block
// 6 -> Larvae
int init_sprite_manager(sprite_manager *my_sprite_manager, list<sprite> *my_sprite_list){
    my_sprite_manager->no_of_sprites = 0;
    my_sprite_list->clear();
    my_sprite_manager->sprites = my_sprite_list;
    
    return 0;
}



int generate_ant(game_map *my_game_map, sprite_manager *my_sprite_manager){
    //int choice[4] = {1,4,7,10}; //,13,16,19};
    srand((unsigned) time(NULL));
    //int randomY = rand() % 4;
    int randomX = rand() % 2;
    sprite ant;
    ant.sprite_position.y_pos = 10;//choice[randomY];
    ant.speed = 3;
    
    if(randomX == 0){
        ant.direction = -1;
        ant.sprite_position.x_pos = small_map_x-1;
    }
    else{
        ant.direction = 1;
        ant.sprite_position.x_pos = 0;
    }
    
    ant.sprite_type = 3;
    ant.sprite_id = my_sprite_manager->no_of_sprites;
    ant.reward = 200;
    
    ant.prev_item = my_game_map->map[ant.sprite_position.y_pos][ant.sprite_position.x_pos];
    if(ant.prev_item == 2){
        reward+=200;
        event = 3;
        return 0;
    }
    my_game_map->map[ant.sprite_position.y_pos][ant.sprite_position.x_pos] = 3;
    my_sprite_manager->sprites->push_back(ant);
    my_sprite_manager->no_of_sprites++;
    //cout << "Generated Ant\n";
    return 0;
}

int update_sprites(game_map *my_game_map, sprite_manager *my_sprite_manager){
    list<sprite> *my_sprites = my_sprite_manager->sprites;
    if(my_sprite_manager->no_of_sprites == 0){
        return  0;
    }
    if(my_sprites->empty()){
        return 0;
    }
    for (auto it = my_sprites->begin(); it != my_sprites->end(); it++) {
        if(my_game_map->map[it->sprite_position.y_pos][it->sprite_position.x_pos] == 2){
            my_game_map->map[it->sprite_position.y_pos][it->sprite_position.x_pos] = 2;
            reward = reward + it->reward;
            event = 3;
            my_sprites->erase(it);
            my_sprite_manager->no_of_sprites--;
            if(my_sprite_manager->no_of_sprites == 0)
                break;
            
            continue;
        }
        my_game_map->map[it->sprite_position.y_pos][it->sprite_position.x_pos] = it->prev_item;
        //cout << "Ant Position Before Update X: " << it->sprite_position.x_pos << "\n";
        if(time_step % it->speed == 0)
            it->sprite_position.x_pos = it->sprite_position.x_pos + it->direction;
        
        if(it->sprite_position.x_pos == -1 || it->sprite_position.x_pos == small_map_x){
            my_sprites->erase(it);
            my_sprite_manager->no_of_sprites--;
            if(my_sprite_manager->no_of_sprites == 0)
                break;
            
            continue;
        }
        it->prev_item = my_game_map->map[it->sprite_position.y_pos][it->sprite_position.x_pos];
        //cout << "Ant Position X: " << it->sprite_position.x_pos << ", Y: " << it->sprite_position.y_pos << "\n" << ", Direction: " << it->direction << ", Sprite ID: " << it->sprite_id << ", Previous "<< it->prev_item << "\n";
        if(it->prev_item == 1){
            reward = reward - 100;
            my_sprites->erase(it);
            my_sprite_manager->no_of_sprites--;
            if(reward < 0){
                reward = 0;
            }
            lives--;
            if(my_sprite_manager->no_of_sprites == 0)
                break;
            
            continue;
        }
        else if (it->prev_item == 2){
            my_game_map->map[it->sprite_position.y_pos][it->sprite_position.x_pos] = 2;
            reward = reward + it->reward;
            event = 3;
            my_sprites->erase(it);
            my_sprite_manager->no_of_sprites--;
            if(my_sprite_manager->no_of_sprites == 0)
                break;
            
            continue;
        }
        else{
            my_game_map->map[it->sprite_position.y_pos][it->sprite_position.x_pos] = it->sprite_type;
        }
        
        }
    
    return 0;
}

int print_position(position *my_pp){
    //cout << "Tongue X: " << my_pp->x_pos << ", Y: " << my_pp->y_pos << "\n";
    return 0;
}

int step(int action, action_space *current_action_space, game_map *my_game_map, tongue_tip *my_tongue_tip){

    switch (action) {
        case 0: {//UP
            
            if(!current_action_space->up){
                //cout << "wrong action\n";
                event = 5;
                break;
            }
            my_tongue_tip->trace->push_front(my_tongue_tip->tt_pos);
            my_game_map->map[my_tongue_tip->tt_pos.y_pos][my_tongue_tip->tt_pos.x_pos] = 1;
            my_tongue_tip->tt_pos.y_pos--;
            while(my_game_map->map[my_tongue_tip->tt_pos.y_pos][my_tongue_tip->tt_pos.x_pos] == 0){
                if(my_tongue_tip->tt_pos.y_pos == 1 || my_tongue_tip->tt_pos.y_pos == 4 || my_tongue_tip->tt_pos.y_pos == 7 || my_tongue_tip->tt_pos.y_pos == 10){
                    break;
                }
                my_tongue_tip->trace->push_front(my_tongue_tip->tt_pos);
                my_game_map->map[my_tongue_tip->tt_pos.y_pos][my_tongue_tip->tt_pos.x_pos] = 1;
                my_tongue_tip->tt_pos.y_pos--;
                
            }
            if(my_game_map->map[my_tongue_tip->tt_pos.y_pos][my_tongue_tip->tt_pos.x_pos] == 6)
                reward = reward + 100;
            my_game_map->map[my_tongue_tip->tt_pos.y_pos][my_tongue_tip->tt_pos.x_pos] = 2;
            
            
            break;
        }
        case 1:{ //Right
            if(!current_action_space->right){
                //cout << "wrong action\n";
                event = 5;
                break;
            }
            
            my_tongue_tip->trace->push_front(my_tongue_tip->tt_pos);
            my_game_map->map[my_tongue_tip->tt_pos.y_pos][my_tongue_tip->tt_pos.x_pos] = 1;
            my_tongue_tip->tt_pos.x_pos++;
            if(my_game_map->map[my_tongue_tip->tt_pos.y_pos][my_tongue_tip->tt_pos.x_pos] == 6)
                reward = reward + 100;
            my_game_map->map[my_tongue_tip->tt_pos.y_pos][my_tongue_tip->tt_pos.x_pos] = 2;
            break;
        case 2: //Left
            if(!current_action_space->left){
                //cout << "wrong action\n";
                event = 5;
                break;
            }
            my_tongue_tip->trace->push_front(my_tongue_tip->tt_pos);
            my_game_map->map[my_tongue_tip->tt_pos.y_pos][my_tongue_tip->tt_pos.x_pos] = 1;
            my_tongue_tip->tt_pos.x_pos--;
            if(my_game_map->map[my_tongue_tip->tt_pos.y_pos][my_tongue_tip->tt_pos.x_pos] == 6)
                reward = reward + 100;
            my_game_map->map[my_tongue_tip->tt_pos.y_pos][my_tongue_tip->tt_pos.x_pos] = 2;
            break;
        }
        case 3: {//Down
            if(!current_action_space->down){
                //cout << "wrong action\n";
                event = 5;
                break;
            }
            my_tongue_tip->trace->push_front(my_tongue_tip->tt_pos);
            my_game_map->map[my_tongue_tip->tt_pos.y_pos][my_tongue_tip->tt_pos.x_pos] = 1;
            my_tongue_tip->tt_pos.y_pos++;
            while(my_game_map->map[my_tongue_tip->tt_pos.y_pos][my_tongue_tip->tt_pos.x_pos] == 0){
                
                if(my_tongue_tip->tt_pos.y_pos == 1 || my_tongue_tip->tt_pos.y_pos == 4 || my_tongue_tip->tt_pos.y_pos == 7 || my_tongue_tip->tt_pos.y_pos == 10){
                    break;
                }
                my_tongue_tip->trace->push_front(my_tongue_tip->tt_pos);
                my_game_map->map[my_tongue_tip->tt_pos.y_pos][my_tongue_tip->tt_pos.x_pos] = 1;
                my_tongue_tip->tt_pos.y_pos++;
            }
            
            if(my_game_map->map[my_tongue_tip->tt_pos.y_pos][my_tongue_tip->tt_pos.x_pos] == 6)
                reward = reward + 100;
            my_game_map->map[my_tongue_tip->tt_pos.y_pos][my_tongue_tip->tt_pos.x_pos] = 2;
            break;
        }
        case 4:{ //Retract
            if(!current_action_space->retract){
                //cout << "wrong action\n";
                event = 5;
                break;
            }
            for(int rspeed = 0; rspeed < retract_speed; rspeed++){
                auto it = my_tongue_tip->trace->begin();
                my_game_map->map[my_tongue_tip->tt_pos.y_pos][my_tongue_tip->tt_pos.x_pos] = 0;
                if(my_game_map->map[my_tongue_tip->tt_pos.y_pos][my_tongue_tip->tt_pos.x_pos + 1] == 1 && my_tongue_tip->tt_pos.y_pos == it->y_pos && my_tongue_tip->tt_pos.x_pos + 1 == it->x_pos){
                    my_tongue_tip->tt_pos.x_pos++;
                    my_tongue_tip->trace->erase(it);
                }
                else if(my_game_map->map[my_tongue_tip->tt_pos.y_pos][my_tongue_tip->tt_pos.x_pos - 1] == 1 && my_tongue_tip->tt_pos.y_pos == it->y_pos && my_tongue_tip->tt_pos.x_pos - 1 == it->x_pos){
                    my_tongue_tip->tt_pos.x_pos--;
                    my_tongue_tip->trace->erase(it);
                }
                else if(my_game_map->map[my_tongue_tip->tt_pos.y_pos + 1][my_tongue_tip->tt_pos.x_pos] == 1 && my_tongue_tip->tt_pos.y_pos + 1 == it->y_pos && my_tongue_tip->tt_pos.x_pos == it->x_pos){
                    if(rspeed >= 1){
                        continue;
                    }
                    for(int i = 1; i <= 3; i++){
                        my_game_map->map[my_tongue_tip->tt_pos.y_pos][my_tongue_tip->tt_pos.x_pos] = 0;
                        my_tongue_tip->trace->erase(it++);
                        my_tongue_tip->tt_pos.y_pos++;
                        my_game_map->map[my_tongue_tip->tt_pos.y_pos][my_tongue_tip->tt_pos.x_pos] = 2;
                        if(my_tongue_tip->tt_pos.y_pos == 1 || my_tongue_tip->tt_pos.y_pos == 4 || my_tongue_tip->tt_pos.y_pos == 7 || my_tongue_tip->tt_pos.y_pos == 10){
                            break;
                        }
                        
                    }
                }
                else if(my_game_map->map[my_tongue_tip->tt_pos.y_pos - 1][my_tongue_tip->tt_pos.x_pos] == 1 && my_tongue_tip->tt_pos.y_pos - 1 == it->y_pos && my_tongue_tip->tt_pos.x_pos == it->x_pos){
                    if(rspeed >= 1){
                        continue;
                    }
                    for(int i = 1; i <= 3; i++){
                        my_game_map->map[my_tongue_tip->tt_pos.y_pos][my_tongue_tip->tt_pos.x_pos] = 0;
                        my_tongue_tip->trace->erase(it++);
                        my_tongue_tip->tt_pos.y_pos--;
                        my_game_map->map[my_tongue_tip->tt_pos.y_pos][my_tongue_tip->tt_pos.x_pos] = 2;
                        if(my_tongue_tip->tt_pos.y_pos == 0 || my_tongue_tip->tt_pos.y_pos == 1 || my_tongue_tip->tt_pos.y_pos == 4 || my_tongue_tip->tt_pos.y_pos == 7 || my_tongue_tip->tt_pos.y_pos == 10){
                            break;
                        }
                    }
                }
                my_game_map->map[my_tongue_tip->tt_pos.y_pos][my_tongue_tip->tt_pos.x_pos] = 2;
            }
            my_game_map->map[my_tongue_tip->tt_pos.y_pos][my_tongue_tip->tt_pos.x_pos] = 2;
            break;
            }
        
        case 5: //Do nothing
            //cout << "Did Nothing";
            break;
            
            
        default:
            //cout << "Wrong Action\n";
            event = 5;
            break;
    }
    
    
    return 0;
}

//{1,4,7,10,13,16,19} Important states for the state space
int get_state_space(game_map *my_game_map, sprite_manager *my_sprite_manager, state_space *current_state_space, tongue_tip *my_tongue_tip){
    
   // int important_states[4] ={1,4,7,10};//,13,16,19};
    for(int i = 0; i<small_map_y; i++){ //prev 7
        for(int j = 0; j<small_map_x; j++){
            current_state_space->map[i][j] = my_game_map->map[i][j];
            if(start == 1)
                cout << current_state_space->map[i][j] << " ";
        }
        if(start == 1)
            cout << "\n";
    }
    current_state_space->no_of_sprites = my_sprite_manager->no_of_sprites;
    current_state_space->my_tt.tt_pos.x_pos = my_tongue_tip->tt_pos.x_pos;
    current_state_space->my_tt.tt_pos.y_pos = my_tongue_tip->tt_pos.y_pos;
    current_state_space->reward =reward;
    current_state_space->lives = lives;
    current_state_space->no_of_larvae = no_of_larvae;
    
    //cout << "got current state space \n";
    return 0;
}

int init_state_space(state_space *s) {
    s->map = (int (*)[10]) malloc(sizeof(int[12][10]));
    s->no_of_sprites = 0;
    s->my_tt.tt_pos.x_pos = 4; //12
    s->my_tt.tt_pos.y_pos = 0;
    s->lives = 3;
    s->reward = 0;
    s->no_of_larvae = 0;
    
    return 0;
}

void init_action_space(action_space *action) {
    action->up = false;
    action->down = false;
    action->right = false;
    action->left = false;
    action->retract = false;
}


void clear_map(game_map *my_game_map){
    for(int i = 0; i < small_map_y; i++){
        for(int j = 1; j < small_map_x; j++){
            if(my_game_map->map[i][j] == 1 || my_game_map->map[i][j] == 2 || my_game_map->map[i][j] == 3 || my_game_map->map[i][j] == 4)
                my_game_map->map[i][j] = 0;
        }
    }
    my_game_map->map[0][4] = 2;
}



int main()
{
    game_map my_game_map;
    sprite_manager my_sprite_manager;
    tongue_tip my_tongue_tip;
    action_space current_action_space;
    state_space current_state_space;
    int action;
    int chance;
    int randChance;
    int Ulives;
    int Oreward;
    int no_of_ants;
    list<sprite> sprites;
    list<position> trace;

    init_sprite_manager(&my_sprite_manager, &sprites);
    init_small_grid(&my_game_map);
    init_state_space(&current_state_space);
    init_action_space(&current_action_space);
    my_tongue_tip.tt_pos.x_pos = 4; //12
    my_tongue_tip.tt_pos.y_pos = 0;
    my_tongue_tip.trace = &trace;

    
    chance = 20;
    no_of_ants = 0;



    while (1) {
        if (game_on && level_on)
        {
            time_step++;
            Ulives = lives;
            Oreward = reward;
            get_action_space(&my_tongue_tip, &my_game_map, &current_action_space);
            get_state_space(&my_game_map, &my_sprite_manager, &current_state_space, &my_tongue_tip);
            if(start == 1)
                cout << event << "\n";
            if(start == 1)
                cout << done << "\n";
            
            if(start == 1){
                for(int i = 0; i < 5; i++){
                    cout << action_sp[i] << " ";
                }
                cout << "\n";
            }
            event = 0;
            //cout << "Please Enter the Next Action: \n";
            cin >> action;
            if(action == -1){
                init_sprite_manager(&my_sprite_manager, &sprites);
                free(my_game_map.map);
                init_small_grid(&my_game_map);
                my_tongue_tip.tt_pos.x_pos = 4; //12s
                my_tongue_tip.tt_pos.y_pos = 0;
                my_tongue_tip.trace->clear();
                free(current_state_space.map);
                init_state_space(&current_state_space);
                init_action_space(&current_action_space);
                init_sprite_manager(&my_sprite_manager, &sprites);
                chance = 20;
                no_of_ants = 0;
                reward = 0;
                event = 0;
                done = 0;
                start = 1;
                lives = 3;
                no_of_larvae = small_map_larvae;
                continue;
            }
            step(action, &current_action_space, &my_game_map, &my_tongue_tip);
            //print_position(&my_tongue_tip.tt_pos);
            update_sprites(&my_game_map, &my_sprite_manager);
            if(lives < Ulives){
                init_sprite_manager(&my_sprite_manager, &sprites);
                no_of_ants = 0;
                clear_map(&my_game_map);
                my_tongue_tip.trace->clear();
                my_tongue_tip.tt_pos.x_pos = 4; //12s
                my_tongue_tip.tt_pos.y_pos = 0;
                event = 7;
                if(lives == 0){
                    done = 1;
                    event = 2;
                    continue;
                }
            }
            if(Oreward + 100 == reward){
                no_of_larvae--;
                event = 6;
                if(no_of_larvae == 0){
                    //cout << "You won the game !!";
                    done = 1;
                    event = 1;
                    continue;
                }
            }
        
            srand((unsigned) time(NULL));
            randChance = rand() % 100;
            if(randChance <= chance && no_of_ants < 1){
                generate_ant(&my_game_map, &my_sprite_manager);
                no_of_ants++;
            }
            no_of_ants = my_sprite_manager.no_of_sprites;
            //cout << event << "\n";
            if(start == 0){
                start = 1;
            }
            
        }
    }
}

