/*
 * buzzer.c
 *
 *  Created on: 2024Äê10ÔÂ27ÈÕ
 *      Author: peril
 */

#include "buzzer.h"
void initBuzzer(){
    gpio_init(BUZZER_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);
}
void startBuzzer(){
    gpio_set_level(BUZZER_PIN, GPIO_HIGH);
}
void stopBuzzer(){
    gpio_set_level(BUZZER_PIN, GPIO_LOW);
}
void shortBuzz(){
    startBuzzer();
    system_delay_ms(SHORT_BUZZ);
    stopBuzzer();
    system_delay_ms(WAIT_SPACING);
}
void longBuzz(){
    startBuzzer();
    system_delay_ms(LONG_BUZZ);
    stopBuzzer();
    system_delay_ms(WAIT_SPACING);
}
void buzzLetter(char c){
    switch(c){
        #define d system_delay_ms(WAIT_SPACING);
        #define s shortBuzz();
        #define l longBuzz();
        #define b break;
        case 'a':case 'A': s l b
        case 'b':case 'B': l s s s b
        case 'c':case 'C': l s l s b
        case 'd':case 'D': l s s b
        case 'e':case 'E': s b
        case 'f':case 'F': s s l s b
        case 'g':case 'G': l l s b
        case 'h':case 'H': s s s s b
        case 'i':case 'I': s s b
        case 'j':case 'J': s l l l b
        case 'k':case 'K': l s l b
        case 'l':case 'L': s l s s b
        case 'm':case 'M': l l b
        case 'n':case 'N': l s b
        case 'o':case 'O': l l l b
        case 'p':case 'P': s l l s b
        case 'q':case 'Q': l l s l b
        case 'r':case 'R': s l s b
        case 's':case 'S': s s s b
        case 't':case 'T': l b
        case 'u':case 'U': s s l b
        case 'v':case 'V': s s s l b
        case 'w':case 'W': s l l b
        case 'x':case 'X': l s s l b
        case 'y':case 'Y': l s l l b
        case 'z':case 'Z': l l s s b
        case '1': s l l l l b
        case '2': s s l l l b
        case '3': s s s l l b
        case '4': s s s s l b
        case '5': s s s s s b
        case '6': l s s s s b
        case '7': l l s s s b
        case '8': l l l s s b
        case '9': l l l l s b
        case '0': l l l l l b
        case '.': s l s l s l b
        case ',': l l s s l l b
        case ':': l l l s s s b
        case ';': l s l s l s b
        case '?': s s l l s s b
        case '=': l s s s l b
        case '\'': s l l l l s b
        case '/': l s s l s b
        case '!': l s l s l l b
        case '-': l s s s s l b
        case '_': s s l l s l b
        case '\"': s l s s l s b
        case '(': l s l l s b
        case ')': l s l l s l b
        case '$': s s s l s s l b
        case '&': s s s s b
        case '@': s l l s l s b
        case ' ': d d d d b
        default: b
        #undef s
        #undef d
        #undef l
        #undef b
    }
}
void buzzSentence(char *str){
    int n=strlen(str);
    for(int i=0;i<n;i++){
        buzzLetter(str[i]);
        system_delay_ms(WAIT_SPACING*2);
    }
}
