// continuous melody driven by code outside

#include <cstdio>
#include <melody.h>

#define SAMPLE_RATE 44100
#define SAMPLE_WIDTH 16

#define BUFF_SIZE SAMPLE_RATE * 28

#define PART_LENGTH 1
Note part[PART_LENGTH] = {{NOTE_A4, 0, 0}};

int main(int argc, char **argv) {
    WaveGenerator waveGenerator(SAMPLE_RATE, SAMPLE_WIDTH, 0.5f);
    Melody melody(&waveGenerator, &part[0], PART_LENGTH);

    static uint32_t buff[BUFF_SIZE];

    uint32_t i = 0;
    while(melody.nextBuffer(&buff[i*4096], 4096) != 0) {
        if (i == 2 || i == 8) {
            melody.adjustFrequency(NOTE_B4);
        }
        if (i == 5) {
            melody.adjustFrequency(NOTE_A4);
        }
        if (i == 10) {
            melody.adjustFrequency(NOTE_C6);
        }
        if (i == 20) {
            melody.stop();
        }
        i++;
    }

    for (int z = 0; z < (i + 1)*4096; z++) {
        printf("%d\t%d\n", z, ((int16_t *) buff)[z * 2]);
    }

    if (argc > 1) {
       melody.wg->writeWav(&buff[0], BUFF_SIZE, argv[1]);
    }
}