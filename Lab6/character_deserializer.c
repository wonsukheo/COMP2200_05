#include <stdio.h>
#include <string.h>

#include "character_deserializer.h"

enum {
    CHARACTER_INFO_LEN = 512,
    CHAR_INFO_COUNT = 64,
    VERSION_CHECK_LEN = 6 /* read first 6 char of file to determine version */
};

int get_character(const char* filename, character_v3_t* out_character)
{
    char character_info_str[CHARACTER_INFO_LEN];
    char* character_info_str_ptr = character_info_str;
    size_t version;
    size_t i = 0;

    FILE* stream = fopen(filename, "r"); /* error check */
    fgets(character_info_str, CHARACTER_INFO_LEN, stream);
    fclose(stream);

    while (i++ < VERSION_CHECK_LEN) {
        if (*character_info_str_ptr == ':') {
            version = 1;
            deserialize_v1_to_v3(character_info_str, out_character);
            break;
        } else if (*character_info_str_ptr == ',') {
            version = 2;
            deserialize_v2_to_v3(character_info_str, out_character);
            break;
        } else if (*character_info_str_ptr == ' ') {
            version = 3;
            /*deserialize_v3_to_v3();*/
            break;
        }
        character_info_str_ptr++;
    } 
    return version;
}

void deserialize_v1_to_v3(char* character_info, character_v3_t* out_character)
{
    char* char_info_token[CHAR_INFO_COUNT];
    char** char_info_token_ptr = char_info_token;
    char delims[] = ":,";
    *char_info_token_ptr = strtok(character_info, delims);
    
    while (*char_info_token_ptr != NULL) {
       *++char_info_token_ptr = strtok(NULL, delims);
    }
    
    char_info_token_ptr = char_info_token;

    while (*char_info_token_ptr != NULL) {
        if (strcmp(*char_info_token_ptr, "id") == 0) {
            sscanf("player_", "%s", &(out_character->name));
            sscanf(*(char_info_token_ptr + 1), "%s", &(out_character->name[7]));
        } else if (strcmp(*char_info_token_ptr, "lvl") == 0) {
            sscanf(*(char_info_token_ptr + 1), "%d", &(out_character->level));
        } else if (strcmp(*char_info_token_ptr, "str") == 0) {
            sscanf(*(char_info_token_ptr + 1), "%d", &(out_character->strength));
        } else if (strcmp(*char_info_token_ptr, "dex") == 0) {
            sscanf(*(char_info_token_ptr + 1), "%d", &(out_character->dexterity));
        } else if (strcmp(*char_info_token_ptr, "intel") == 0) {
            sscanf(*(char_info_token_ptr + 1), "%d", &(out_character->intelligence));
        } else if (strcmp(*char_info_token_ptr, "def") == 0) {
            sscanf(*(char_info_token_ptr + 1), "%d", &(out_character->armour));
        } else if (strcmp(*char_info_token_ptr, "hp") == 0) {
            sscanf(*(char_info_token_ptr + 1), "%d", &(out_character->health));
        } else if (strcmp(*char_info_token_ptr, "mp") == 0) {
            sscanf(*(char_info_token_ptr + 1), "%d", &(out_character->mana));
        }
        char_info_token_ptr += 2;
    }
    
    out_character->evasion = out_character->dexterity / 2;
    out_character->elemental_resistance.fire = out_character->armour / 12;
    out_character->elemental_resistance.cold = out_character->elemental_resistance.fire;
    out_character->elemental_resistance.lightning = out_character->elemental_resistance.fire;
    out_character->leadership = out_character->level / 10;
    out_character->minion_count = 0;
}

void deserialize_v2_to_v3(char* character_info, character_v3_t* out_character)
{
    char* char_info_token[CHAR_INFO_COUNT];
    char** char_info_token_ptr = char_info_token;
    char delims[] = ",\n";
    *char_info_token_ptr = strtok(character_info, delims);
    
    while (*char_info_token_ptr != NULL) {
       *++char_info_token_ptr = strtok(NULL, delims);
    }
    
    char_info_token_ptr = char_info_token;

    while (*char_info_token_ptr != NULL) {
        if (strcmp(*char_info_token_ptr, "name") == 0) {
            sscanf(*(char_info_token_ptr + 10), "%.50s", &(out_character->name));
        } else if (strcmp(*char_info_token_ptr, "level") == 0) {
            sscanf(*(char_info_token_ptr + 10), "%d", &(out_character->level));
        } else if (strcmp(*char_info_token_ptr, "strength") == 0) {
            sscanf(*(char_info_token_ptr + 10), "%d", &(out_character->strength));
        } else if (strcmp(*char_info_token_ptr, "dexterity") == 0) {
            sscanf(*(char_info_token_ptr + 10), "%d", &(out_character->dexterity));
        } else if (strcmp(*char_info_token_ptr, "intelligence") == 0) {
            sscanf(*(char_info_token_ptr + 10), "%d", &(out_character->intelligence));
        } else if (strcmp(*char_info_token_ptr, "armour") == 0) {
            sscanf(*(char_info_token_ptr + 10), "%d", &(out_character->armour));
        } else if (strcmp(*char_info_token_ptr, "health") == 0) {
            sscanf(*(char_info_token_ptr + 10), "%d", &(out_character->health));
        } else if (strcmp(*char_info_token_ptr, "mana") == 0) {
            sscanf(*(char_info_token_ptr + 10), "%d", &(out_character->mana));
        } else if (strcmp(*char_info_token_ptr, "evasion") == 0) {
            sscanf(*(char_info_token_ptr + 10), "%d", &(out_character->evasion));
        }
        char_info_token_ptr++;
    }
    

    out_character->elemental_resistance.fire = out_character->armour / 12;
    out_character->elemental_resistance.cold = out_character->elemental_resistance.fire;
    out_character->elemental_resistance.lightning = out_character->elemental_resistance.fire;
    out_character->leadership = out_character->level / 10;
    out_character->minion_count = 0;
}
