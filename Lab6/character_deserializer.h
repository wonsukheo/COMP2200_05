#ifndef CHARACTER_DESERIALIZER_H
#define CHARACTER_DESERIALIZER_H

enum { NAME_LEN = 50 };
    
typedef struct {
    char name[NAME_LEN]; /* 배열로? 포인터로? */
    unsigned int health;
    unsigned int strength;
    unsigned int defence;
} minion_t;

typedef struct {
    unsigned int fire;
    unsigned int cold;
    unsigned int lightning; /* why unsigned? could it be char instead? */
} elemental_resistance_t;

typedef struct {
    char name[NAME_LEN];
    unsigned int level;
    unsigned int health;
    unsigned int mana;
    unsigned int strength;
    unsigned int dexterity;    /* diff btw unsigned int & size_t? */
    unsigned int intelligence;
    unsigned int armour;
    unsigned int evasion;
    unsigned int leadership;
    size_t minion_count;
    elemental_resistance_t elemental_resistance;
    minion_t minions[3];
} character_v3_t;

int get_character(const char* filename, character_v3_t* out_character);

void deserialize_v1_to_v3(char* character_info, character_v3_t* out_character);

void deserialize_v2_to_v3(char* character_info, character_v3_t* out_character);
#endif /* CHARACTER_DESERIALIZER_H */
