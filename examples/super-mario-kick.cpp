// small buffer to test "cutting" through notes

#include <cstdio>
#include <cstring>
#include <melody.h>

#define SAMPLE_RATE 33333
#define SAMPLE_WIDTH 16

#define BUFF_SIZE 3333 // 100 ms
#define WAV_BUFFER_SIZE SAMPLE_RATE * 1 // 1 second1

#define PART_LENGTH 3
static Note startupMelody[PART_LENGTH] = {
    {500, 20, 0},
    {0, 80, 0},
    {693, 20, 0}
};

int main(int argc, char **argv) {
    WaveGenerator waveGenerator(SAMPLE_RATE, SAMPLE_WIDTH, 0.5f);
    Melody melody(&waveGenerator, &startupMelody[0], PART_LENGTH);
    static uint32_t buff[BUFF_SIZE];
    static uint32_t wav[WAV_BUFFER_SIZE];

    uint32_t i = 0;
    uint32_t generated = 0;
    while(generated = melody.nextBuffer(&buff[0], BUFF_SIZE)) {
        memcpy(&wav[i * BUFF_SIZE], &buff[0], generated * 4);
        i++;
    }

    for (int z = 0; z < WAV_BUFFER_SIZE; z++) {
        printf("%d\t%d\n", z, ((int16_t *) wav)[z * 2]);
    }

    if (argc > 1) {
       melody.wg->writeWav(&wav[0], WAV_BUFFER_SIZE, argv[1]);
    }
}