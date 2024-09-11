#include <stdio.h>
#include <stdint.h>

#define _USE_MATH_DEFINES // for M_PI, before #include <math.h>
#include <math.h>

#define DURATION 5 // music length in sec.
#define AMPLITUDE 32767
#define FREQ 440
#define SAMPLE_RATE 44100

// struct for WAV file header
typedef struct {
    char ChunkId[4];
    uint32_t ChunkSize;
    char Format[4];
    char SubChunk1Id[4];
    uint32_t SubChunk1Size;
    uint16_t AudioFormat;
    uint16_t NumChannels;
    uint32_t SampleRate;
    uint32_t ByteRate;
    uint16_t BlockAlign;
    uint16_t BitsPerSample;
    char SubChunk2Id[4];
    uint32_t SubChunk2Size;
} WavHeader;

void generate_sound_data(FILE *file, WavHeader *h) {
    double k = 0.8; // коэффициент амплитудной модуляции
    double f = 0.1; // 
    for (int i = 0; i < h->SampleRate * DURATION; i++) {
        double t = (double)i / h->SampleRate; // time in sec

        double carrier = (double)((1 + k * sin(2.0 * M_PI * (FREQ/4) * t)));
        double signal = (double)(sin(2.0 * M_PI * FREQ * t));
        int16_t sample = (int16_t)((AMPLITUDE * 0.5) * signal * carrier *exp(-0.8 * t));
        fwrite(&sample, sizeof(sample), 1, file);
    }
}

int main() {
    // BRE - open file for Binary Rig Entry
    FILE *file = fopen("melody.wav", "wb");

    WavHeader header = {
        .ChunkId = "RIFF",
        .ChunkSize = 36 + SAMPLE_RATE * DURATION * 2,
        .Format = "WAVE",
        .SubChunk1Id = "fmt ",
        .SubChunk1Size = 16,
        .AudioFormat = 1,
        .NumChannels = 1,
        .SampleRate = SAMPLE_RATE,
        .ByteRate = SAMPLE_RATE * 2,
        .BlockAlign = 2,
        .BitsPerSample = 16,
        .SubChunk2Id = "data",
        .SubChunk2Size = SAMPLE_RATE * DURATION * 2,
    };

    fwrite(&header, sizeof(header), 1, file);
    printf("record: %d bytes", sizeof(header)); // 44 bytes

    generate_sound_data(file, &header);

    fclose(file);

    return 0;
}


