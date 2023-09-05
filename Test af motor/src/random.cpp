#include <random.h>

int32_t random_int_range(int32_t min, int32_t max){
    //https://stackoverflow.com/questions/12657962/how-do-i-generate-a-random-number-between-two-variables-that-i-have-stored

    int32_t n = max - min + 1;
    int32_t remainder = RAND_MAX % n;
    int32_t x;
    do{
        x = rand();
    }while (x >= RAND_MAX - remainder);
    
    return min + x % n;
}