#include <cstring>
#include "melody.h"

#define SMOOTHING 400
//#define DEBUG

Beeper::Beeper(WaveGenerator *waveGenerator, Note *definition, uint32_t length) {
    this->definition = definition;
    this->length = length;
    this->noteIndex = 0;
    this->noteOffset = 0;
    this->wg = waveGenerator;

    this->forceStop = false;
    this->forceStopped = false;
    this->newFrequency = 0;
}

uint32_t Beeper::nextBuffer(uint32_t *buffer, uint32_t bufferSize) {
    uint32_t bufferPosition = 0;

#ifdef DEBUG
    printf("nextBuffer(), starting noteIndex: %d =============================================\n", noteIndex);
#endif

    if (noteIndex == length || forceStopped) {
#ifdef DEBUG
        printf("end!\n");
#endif
        return 0;
    }

    for (uint32_t i = noteIndex; i < length; i++) {
#ifdef DEBUG
        printf("-> note index: %d\n", i);
#endif
        Note note = this->definition[noteIndex];

        // if length is 0 we do infinite time
        uint32_t totalSize = note.length_ms != 0 ? this->wg->ms32size(note.length_ms) : UINT32_MAX;
        uint32_t noteSize = totalSize - this->noteOffset;

        if (newFrequency != 0 && note.length_ms == 0) {
            this->wg->sineTransform(&buffer[bufferPosition], bufferSize, note.frequency, newFrequency);
            this->definition[noteIndex].frequency = newFrequency;
            this->definition[noteIndex].edge_smoothing = 0;
            newFrequency = 0;
        } else {
            if (noteSize <= (bufferSize - bufferPosition)) {
#ifdef DEBUG
                printf("noteSize smaller then buffer, bufferPosition: %d, noteSize: %d, freq: %d\n", bufferPosition, noteSize, note.frequency);
#endif
                this->wg->sine(&buffer[bufferPosition], noteSize, note.frequency);
#ifdef DEBUG
                for (int z = bufferPosition; z < bufferPosition + noteSize; z++) {
                    printf("-- %d\t%d\n", z, ((int16_t *) buffer)[z * 2]);
                }
#endif
            } else {
#ifdef DEBUG
                printf("noteSize larger then buffer, bufferPosition: %d, toEnd: %d, freq: %d\n", bufferPosition, bufferSize - bufferPosition, note.frequency);
#endif
                this->wg->sine(&buffer[bufferPosition], bufferSize - bufferPosition, note.frequency);
#ifdef DEBUG
                for (int z = bufferPosition; z < bufferSize; z++) {
                    printf("-- %d\t%d\n", z, ((int16_t *) buffer)[z * 2]);
                }
#endif
            }
        }

        // smoothing -----------------------------------------------------------------------------------------------
        if (note.edge_smoothing == EDGE_RISING || note.edge_smoothing == EDGE_BOTH) {
            if (noteOffset == 0) {
                uint32_t smoothingLength = bufferSize - bufferPosition;
                if (smoothingLength > SMOOTHING) smoothingLength = SMOOTHING;
                float volumeEnd = (float) smoothingLength / SMOOTHING;
                if (volumeEnd > 1.0f) volumeEnd = 1.0f;
                this->wg->adjustVolume(&buffer[bufferPosition], smoothingLength, 0.0f, volumeEnd);
            } else if (bufferPosition == 0 && noteOffset <= SMOOTHING) {
                uint32_t smoothingLength = SMOOTHING - noteOffset;
                float volumeStart = (float) (SMOOTHING - smoothingLength) / SMOOTHING;
                this->wg->adjustVolume(&buffer[bufferPosition], smoothingLength, volumeStart, 1.0f);
            }
        }
        if (note.edge_smoothing == EDGE_FALLING || note.edge_smoothing == EDGE_BOTH) {
            if (noteSize - SMOOTHING < bufferSize) {
                uint32_t smoothingLength = bufferSize - (noteSize - SMOOTHING);
                if (smoothingLength > SMOOTHING) smoothingLength = SMOOTHING;
                this->wg->adjustVolume(&buffer[bufferPosition + noteSize - SMOOTHING], smoothingLength, 1.0f,
                                       (float) (SMOOTHING - smoothingLength) / SMOOTHING);
            } else if (bufferPosition == 0 && noteSize < SMOOTHING) {
                this->wg->adjustVolume(&buffer[bufferPosition], noteSize, (float) noteSize / SMOOTHING, 0.0f);
            }
        }

        // ---------------------------------------------------------------------------------------------------------

        if (noteSize <= (bufferSize - bufferPosition)) {
            bufferPosition += noteSize;
            if (note.length_ms != 0) {
                this->noteIndex++;
            }
            this->noteOffset = 0;
        } else {
            this->noteOffset += bufferSize - bufferPosition;
            bufferPosition = bufferSize;
            break;
        }
    }

    if (forceStop) {
        // we end buffer with smoothed edge
        this->wg->adjustVolume(&buffer[bufferSize - SMOOTHING], SMOOTHING, 1.0f, 0);
        this->forceStopped = true;
    }


    return bufferPosition;
}

void Beeper::stop() {
    this->forceStop = true;
}

void Beeper::adjustFrequency(uint16_t frequency) {
    this->newFrequency = frequency;
}