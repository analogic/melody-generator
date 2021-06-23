#include "melody.h"

#define SMOOTHING 400

Melody::Melody(WaveGenerator *waveGenerator, Note *definition, uint32_t length) {
    this->definition = definition;
    this->length = length;
    this->end = false;
    this->noteIndex = 0;
    this->noteOffset = 0;
    this->wg = waveGenerator;
}

bool Melody::nextBuffer(uint32_t *buffer, uint32_t bufferSize) {
    uint32_t bufferPosition = 0;

    for(uint32_t i = noteIndex; i < length; i++) {
        Note note = this->definition[noteIndex];
        uint32_t noteSize = this->wg->ms32size(note.length_ms) - this->noteOffset;

        if (noteSize <= (bufferSize - bufferPosition)) {
            // printf("smaller then buffer\n");
            this->wg->sine(&buffer[bufferPosition], noteSize, note.frequency);
        } else {
            // printf("larger then buffer\n");
            this->wg->sine(&buffer[bufferPosition], bufferSize - bufferPosition, note.frequency);
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
                this->wg->adjustVolume(&buffer[bufferPosition + noteSize - SMOOTHING], smoothingLength, 1.0f, (float)(SMOOTHING - smoothingLength)/SMOOTHING);
            } else if (bufferPosition == 0 && noteSize < SMOOTHING) {
                this->wg->adjustVolume(&buffer[bufferPosition], noteSize, (float)noteSize/SMOOTHING, 0.0f);
            }
        }
        // ---------------------------------------------------------------------------------------------------------

        if (noteSize <= (bufferSize - bufferPosition)) {
            bufferPosition += noteSize;
            this->noteIndex++;
            this->noteOffset = 0;
        } else {
            this->noteOffset += bufferSize - bufferPosition;
            break;
        }
    }

    if (noteIndex == length) {
        return false;
    } else {
        return true;
    }
}

