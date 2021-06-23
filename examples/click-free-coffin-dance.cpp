// click-free melody - smooth transition from/to zero with volume adjustment function

#include <cstdio>
#include <melody.h>

#define SAMPLE_RATE 44100
#define SAMPLE_WIDTH 16

#define BUFF_SIZE SAMPLE_RATE * 28

#define PART_LENGTH 112
Note part[PART_LENGTH] = {
        {NOTE_A4, 250, 3},
        {NOTE_A4, 250, 3},
        {NOTE_A4, 250, 3},
        {NOTE_A4, 250, 3},
        {NOTE_A4, 250, 3},
        {NOTE_A4, 250, 3},
        {NOTE_A4, 250, 3},
        {NOTE_A4, 250, 3},
        {NOTE_AS4, 250, 3},
        {NOTE_AS4, 250, 3},
        {NOTE_AS4, 250, 3},
        {NOTE_AS4, 250, 3},
        {NOTE_AS4, 250, 3},
        {NOTE_AS4, 250, 3},
        {NOTE_AS4, 250, 3},
        {NOTE_AS4, 250, 3},
        {NOTE_AS4, 250, 3},
        {NOTE_AS4, 250, 3},
        {NOTE_AS4, 250, 3},
        {NOTE_AS4, 250, 3},
        {NOTE_D5,  250, 3},
        {NOTE_D5,  250, 3},
        {NOTE_D5,  250, 3},
        {NOTE_D5,  250, 3},
        {NOTE_C5,  250, 3},
        {NOTE_C5,  250, 3},
        {NOTE_C5,  250, 3},
        {NOTE_C5,  250, 3},
        {NOTE_F5,  250, 3},
        {NOTE_F5,  250, 3},
        {NOTE_F5,  250, 3},
        {NOTE_F5,  250, 3},
        {NOTE_G5,  250, 3},
        {NOTE_G5,  250, 3},
        {NOTE_G5,  250, 3},
        {NOTE_G5,  250, 3},
        {NOTE_G5,  250, 3},
        {NOTE_G5,  250, 3},
        {NOTE_G5,  250, 3},
        {NOTE_G5,  250, 3},
        {NOTE_G5,  250, 3},
        {NOTE_G5,  250, 3},
        {NOTE_G5,  250, 3},
        {NOTE_G5,  250, 3},
        {NOTE_C5,  250, 3},
        {NOTE_AS4, 250, 3},
        {NOTE_A4,  250, 3},
        {NOTE_F4,  250, 3},
        {NOTE_G4,  250, 3},
        {0,        250, 3},
        {NOTE_G4,  250, 3},
        {NOTE_D5,  250, 3},
        {NOTE_C5,  250, 3},
        {0,        250, 3},
        {NOTE_AS4, 250, 3},
        {0,        250, 3},
        {NOTE_A4,  250, 3},
        {0,        250, 3},
        {NOTE_A4,  250, 3},
        {NOTE_A4,  250, 3},
        {NOTE_C5,  250, 3},
        {0,        250, 3},
        {NOTE_AS4, 250, 3},
        {NOTE_A4,  250, 3},
        {NOTE_G4,  250, 3},
        {0,        250, 3},
        {NOTE_G4,  250, 3},
        {NOTE_AS5, 250, 3},
        {NOTE_A5,  250, 3},
        {NOTE_AS5, 250, 3},
        {NOTE_A5,  250, 3},
        {NOTE_AS5, 250, 3},
        {NOTE_G4,  250, 3},
        {0,        250, 3},
        {NOTE_G4,  250, 3},
        {NOTE_AS5, 250, 3},
        {NOTE_A5,  250, 3},
        {NOTE_AS5, 250, 3},
        {NOTE_A5,  250, 3},
        {NOTE_AS5, 250, 3},
        {NOTE_G4,  250, 3},
        {0,        250, 3},
        {NOTE_G4,  250, 3},
        {NOTE_D5,  250, 3},
        {NOTE_C5,  250, 3},
        {0,        250, 3},
        {NOTE_AS4, 250, 3},
        {0,        250, 3},
        {NOTE_A4,  250, 3},
        {0,        250, 3},
        {NOTE_A4,  250, 3},
        {NOTE_A4,  250, 3},
        {NOTE_C5,  250, 3},
        {0,        250, 3},
        {NOTE_AS4, 250, 3},
        {NOTE_A4,  250, 3},
        {NOTE_G4,  250, 3},
        {0,        250, 3},
        {NOTE_G4,  250, 3},
        {NOTE_AS5, 250, 3},
        {NOTE_A5,  250, 3},
        {NOTE_AS5, 250, 3},
        {NOTE_A5,  250, 3},
        {NOTE_AS5, 250, 3},
        {NOTE_G4,  250, 3},
        {0,        250, 3},
        {NOTE_G4,  250, 3},
        {NOTE_AS5, 250, 3},
        {NOTE_A5,  250, 3},
        {NOTE_AS5, 250, 3},
        {NOTE_A5,  250, 3},
        {NOTE_AS5, 250}
};

int main(int argc, char **argv) {
    WaveGenerator waveGenerator(SAMPLE_RATE, SAMPLE_WIDTH, 0.5f);
    Melody melody(&waveGenerator, &part[0], PART_LENGTH);
    static uint32_t buff[BUFF_SIZE];

    uint32_t i = 0;
    while(melody.nextBuffer(&buff[i*4096], 4096)) {
        i++;
    }

    for (int z = 0; z < BUFF_SIZE; z++) {
        printf("%d\t%d\n", z, ((int16_t *) buff)[z * 2]);
    }

    if (argc > 1) {
       melody.wg->writeWav(&buff[0], BUFF_SIZE, argv[1]);
    }
}