#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "../inc/string_handler.h"
#include "../inc/main.h"

uint16_t change_arr_str_to_int(char arr_char[], uint8_t arr_int[])
{
	char str_tok[3] = {0};
    uint8_t j = 0;
    int data = 0;
    uint16_t z = 0;
    
    for(uint32_t i = 0; i < strlen(arr_char); i++)
    {
        if(arr_char[i] == ' ' || arr_char[i] == '\0' || arr_char[i] == '\n')
        {
            str_tok[j] = '\0';
            sscanf(str_tok, "%x",&data);
            arr_int[z] = data;
            memset(str_tok, 0, sizeof(str_tok));
            j = 0;
            z++;
        }
        else
        {
            str_tok[j] = arr_char[i];
            j++;
        }
    }
    return z;
}


