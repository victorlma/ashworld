#include "mdefs.h"
#include "stdlib.h"

typedef enum 
{
    BIO_STONE,
    BIO_WODD,
    BIO_GRASS,
    BIO_WATER,

    BIO_COUNT
} BIOME;

typedef enum
{
    ENT_WALL,
    ENT_PERSON,
    ENT_OBJECT,
    ENT_WEAPON,
    ENT_ARMOR,
    ENT_MAGIC,
    ENT_BOOK,

    ENT_COUNT
} ENTITY;

typedef struct
{
    s8 hp;
    
    s8 agility;
    s8 concentration;
    s8 strength;
    s8 resistance;

    s8 capacity;
    s8 total_capacity;

} PersonStats;

typedef struct
{
    u32 y;
    u32 x;

    PersonStats stats;

} Person;


typedef struct
{
    u8 w;
    u8 h;
    BIOME  biome;

} Room;



u64 roll(u64 d)
{
    return (rand() / (RAND_MAX / d)) + 1;
}

s8 normalize_score(s8 score)
{
    s8 sc;
    if (score <=4) sc = -3;
    else if (score == 6 || score == 5) sc = -2;
    else if (score == 7 || score == 8) sc = -1;
    else if (score >= 9 && score <= 12) sc = 0;
    else if (score >= 13 && score <= 14) sc = 1;
    else if (score >= 15 && score <= 16) sc = 2;
    else if (score >= 17 && score <= 20) sc = 3;
    else if (score >= 21 && score <= 22) sc = 4;
    else if (score >= 23 && score <= 25) sc = 5;
    else if (score > 25) sc = 6;

    return sc;
}

void set_stats_default(PersonStats *stats)
{

    stats->agility = normalize_score(roll(6) + roll(6) + roll(6));
    stats->strength = normalize_score(roll(6) + roll(6) + roll(6));
    stats->concentration = normalize_score(roll(6) + roll(6) + roll(6));
    stats->resistance = normalize_score(roll(6) + roll(6) + roll(6));


    stats->hp = stats->resistance + roll(8);
    stats->capacity = stats->strength + 8;
    stats->total_capacity = stats->capacity;
}
