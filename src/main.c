#define _POSIX_C_SOURCE 199309L
#define DEBUG 1
#include "mdefs.h"
#include "ncurses.h"
#include "time.h"
#include "stdlib.h"


typedef enum 
{
    BTN_UP,
    BTN_DOWN,
    BTN_LEFT,
    BTN_RIGHT,
    BTN_CLOSE,

    BTN_COUNT
} BTN;

typedef struct
{
    u32  keymap[BTN_COUNT];
    u32  timeout[BTN_COUNT];
} Controller;



typedef struct
{
    u64 size;
    u64 used;
    void *data;

} MemArena;

#include "game.c"

typedef struct
{
    u32         min_w;
    u32         min_h;
    u32         current_w;
    u32         current_h;

    u32         center_y;
    u32         center_x;

    Controller  pl_keys;
    b32         should_close;
    u32          ch;

    Person      pl;
    u8          plch;

    MemArena    perm_storage;
    MemArena    temp_storage;

} EngineState;

#define push_struct(arena, struct) \
    push_size_(arena, sizeof(struct))

u8 *push_size_(MemArena *arena, u64 size)
{
    DEBUG_ASSERT((arena->used + size) <= arena->size);

    u8 *ret = (((u8 *) arena->data) + arena->used);

    arena->used += size;
    
    return ret;
}

void setup_engine(EngineState *enst) {
    enst->plch = '@';
    enst->pl.x = 0;
    enst->pl.y = 0;

    enst->pl_keys.keymap[BTN_CLOSE] = 27;
    enst->pl_keys.keymap[BTN_UP] = 259;
    enst->pl_keys.keymap[BTN_DOWN] = 258;
    enst->pl_keys.keymap[BTN_LEFT] = 260;
    enst->pl_keys.keymap[BTN_RIGHT] = 261;

    enst->min_w = 40;
    enst->min_h = 20;


    set_stats_default(&enst->pl.stats);

    enst->perm_storage.size = MB(6);
    enst->perm_storage.used = 0;

    enst->temp_storage.size = MB(2);
    enst->temp_storage.used = 0;

    enst->perm_storage.data = malloc(enst->perm_storage.size);
    enst->perm_storage.data = malloc(enst->temp_storage.size);

}


void engine_cleanup(EngineState *enst)
{
    free(enst->perm_storage.data);
    free(enst->temp_storage.data);
}

b32 handle_resize(EngineState *enst)
{
    getmaxyx(stdscr, enst->current_h, enst->current_w);
    enst->center_x = enst->current_w/2;
    enst->center_y = enst->current_h/2;

    return (enst->current_w >= enst->min_w && enst->current_h >= enst->min_h) ? TRUE : FALSE;
}

b32 key_down(u64 key, Controller *ctrl)
{
    return ctrl->timeout[key] > 0 ? TRUE : FALSE;
}

b32 key_up(u64 key, Controller *ctrl)
{
    return ctrl->timeout[key] == 0 ? TRUE : FALSE;
}

b32 key_jp(u64 key, EngineState *enst)
{
    return (enst->ch == enst->pl_keys.keymap[key] &&
            enst->pl_keys.timeout[key] != 0);
}
void process_input(EngineState *enst)
{
    enst->ch = getch();
    while (getch() != EOF) ;

    for (u64 i = 0; i < BTN_COUNT; ++i) {
        enst->ch == enst->pl_keys.keymap[i] ? enst->pl_keys.timeout[i] = 10 : FALSE;

        if (key_down(i, &enst->pl_keys)) --enst->pl_keys.timeout[i];
    }

}

void GameCantDraw(EngineState *enst)
{
    clear();
    mvprintw(enst->center_y, enst->center_x-3, "TooSmall");
    mvprintw(enst->center_y+1, enst->center_x-3, "MinSize");
    mvprintw(enst->center_y+2, enst->center_x-2, "40x20");

    refresh();
}

void GameUpdateRender(EngineState *enst)
{

    if (key_jp(BTN_LEFT, enst)) enst->pl.x -= 1;
    if (key_jp(BTN_UP, enst)) enst->pl.y -= 1;
    if (key_jp(BTN_RIGHT, enst)) enst->pl.x += 1;
    if (key_jp(BTN_DOWN, enst)) enst->pl.y += 1;




    clear();
    box(stdscr,0,0);
    mvprintw(0,enst->center_x-4,"Ash World");
    mvprintw(enst->pl.y,enst->pl.x, "%c", enst->plch);

    for (u64 i=0; i < BTN_COUNT; ++i) {

        mvprintw(2+i, 2, "win h: %d w: %d ", enst->current_h, enst->current_w);

    }

    refresh();
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    EngineState enst = {0};

    setup_engine(&enst);

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, 1);
    nodelay(stdscr, 1);
    set_escdelay(1);

    float target_fps = 60;
    float target_ms = 1.0f/target_fps;

    clock_t  clk = clock();

    while (!enst.should_close) {

        if (key_down(BTN_CLOSE, &enst.pl_keys)) enst.should_close = 1;


        process_input(&enst);


        if (handle_resize(&enst)) {
            GameUpdateRender(&enst);
        }
        else {
            GameCantDraw(&enst);
        }
        



        clk = clock() - clk;
        float ft = (float) clk / CLOCKS_PER_SEC;
        if (ft < target_ms) {
            struct timespec rqt, rqtr = {0};
            rqt.tv_sec = 0;
            rqt.tv_nsec = (target_ms - ft) * 1000000000L;
            nanosleep(&rqt, &rqtr);
        }
        clk = clock();
        
    }

    echo();
    curs_set(1);
    endwin();
    return 0;

}
