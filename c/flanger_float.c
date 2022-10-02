#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DATA_MARK (u_int32_t)0x61746164
#define DATA_AMPL (float)32768.0

typedef struct {
    u_int32_t riff;
    u_int32_t chunkSize;
    u_int32_t wave;
    u_int32_t fmt;
    u_int32_t subchunk1Size;
    u_int16_t audioFormat;
    u_int16_t numChanels;
    u_int32_t sampleRate;
    u_int32_t byteRate;
    u_int16_t blockAlign;
    u_int16_t bitsPerSample;
    u_int32_t dataIndicator;
    u_int32_t dataSize;
} Header;

int main() {
  FILE *srcFile, *resFile;

  srcFile = fopen("../assets/music.wav", "r");
  resFile = fopen("../assets/flanger_float.wav", "w+");

  if (!srcFile || !resFile) {
    printf("Error while opening the file.\n");
    return 0;
  }

  Header header;
  fread(&header, sizeof(Header) - 2 * sizeof(u_int32_t), 1, srcFile);

  u_int32_t extendedHeaderIndex = 0;
  u_int8_t extendedHeader[1024] = {};

  u_int32_t chunkId = 0;
  u_int32_t chunkSize = 0;

  while (1) {

    fread(&chunkId, sizeof(u_int32_t), 1, srcFile);
    fread(&chunkSize, sizeof(u_int32_t), 1, srcFile);

    if(chunkId == DATA_MARK) {
      header.dataIndicator = chunkId;
      header.dataSize = chunkSize;
      break;
    };

    u_int8_t* chunkData = (u_int8_t*) malloc(chunkSize*sizeof(u_int8_t));
    fread(chunkData, sizeof(u_int8_t), chunkSize, srcFile);

    *(u_int32_t*)(&(extendedHeader[extendedHeaderIndex])) = chunkId;
    extendedHeaderIndex += sizeof(u_int32_t);

    *(u_int32_t*)(&(extendedHeader[extendedHeaderIndex])) = chunkSize;
    extendedHeaderIndex += sizeof(u_int32_t);

    for(u_int32_t i = 0; i < chunkSize; i++) {
      extendedHeader[extendedHeaderIndex] = chunkData[i];
      extendedHeaderIndex++;
    }

    free(chunkData);
  }

  u_int32_t sampleQty = header.dataSize / ((header.bitsPerSample / 8) * header.numChanels);

  int16_t* srcLeftData  = (int16_t*) malloc(sampleQty*sizeof(int16_t));
  int16_t* srcRightData = (int16_t*) malloc(sampleQty*sizeof(int16_t));

  float* xl = (float*) malloc(sampleQty*sizeof(float));
  float* xr = (float*) malloc(sampleQty*sizeof(float));

  for (u_int32_t i = 0; i < sampleQty; i++) {
    fread(&(srcLeftData[i]), sizeof(int16_t), 1, srcFile);
    if(header.numChanels == 2) {
      fread(&(srcRightData[i]), sizeof(int16_t), 1, srcFile);
    }
  }

  for (u_int32_t i = 0; i < sampleQty; i++) {
    xl[i] = ((float)srcLeftData[i]) / DATA_AMPL;
    if(header.numChanels == 2) {
      xr[i] = ((float)srcRightData[i]) / DATA_AMPL;
    }
  }

  u_int32_t interp = 4;
  u_int32_t interpQty = (sampleQty + (sampleQty - 1)*(interp - 1));
  float fs = header.sampleRate;
  float fsn = fs * (float)interp;
  float mix = 0.75;
  float delay = 0.005 * fsn;
  float width = 0.005 * fsn;
  float rate = (1 / 0.7) * fsn;
  float halfRate = rate / 2;
  float rateSlope =  2 * 2 / rate;

  float* xl_interp = (float*) malloc(interpQty*sizeof(float));
  float* xr_interp = (float*) malloc(interpQty*sizeof(float));

  xl_interp[(sampleQty - 1) * interp] = xl[sampleQty - 1];
  xr_interp[(sampleQty - 1) * interp] = xr[sampleQty - 1];
  for (u_int32_t i = 0; i < (sampleQty - 1); i++) {
    float xl_interp_factor = (xl[i + 1] - xl[i]) / ((float)interp);
    float xr_interp_factor = (xr[i + 1] - xr[i]) / ((float)interp);
    for (u_int32_t j = 0; j < interp; j++) {
      xl_interp[i * interp + j] = xl[i] + xl_interp_factor * j;
      xr_interp[i * interp + j] = xr[i] + xr_interp_factor * j;
    }
  }
  
  u_int32_t maxDelaySamp = (u_int32_t)ceil((delay + width) * interp);

  float* yl_interp = (float*) malloc(interpQty*sizeof(float));
  float* yr_interp = (float*) malloc(interpQty*sizeof(float));

  for (u_int32_t i = 0; i < maxDelaySamp; i++) {
    yl_interp[i] = xl_interp[i];
    yr_interp[i] = xr_interp[i];
  }
  
  for (u_int32_t i = maxDelaySamp; i < interpQty; i++) {
    float r = (float)(i) - (float)((u_int32_t)(i / rate)) * rate;
    float lfo = (r <= halfRate) ?
      (-1.0f + rateSlope * r) :
      (1.0f - rateSlope * (r - halfRate));
    u_int32_t delayedSample = (u_int32_t)ceil(delay + (width / 2) * (1 + lfo));
    delayedSample = (i - delayedSample > 0) ? (i - delayedSample) : 0;
    yl_interp[i] = xl_interp[i] + mix * yl_interp[delayedSample];
    yr_interp[i] = xr_interp[i] + mix * yr_interp[delayedSample];

    // para remover o estouro do áudio
    // if(yl_interp[i] >  0.999) yl_interp[i] =  0.999;
    // if(yl_interp[i] < -0.999) yl_interp[i] = -0.999;
    // if(yr_interp[i] >  0.999) yr_interp[i] =  0.999;
    // if(yr_interp[i] < -0.999) yr_interp[i] = -0.999;
  }

  float* yl = (float*) malloc(sampleQty*sizeof(float));
  float* yr = (float*) malloc(sampleQty*sizeof(float));

  for (u_int32_t i = 0; i < sampleQty; i++) {
    if(i * interp < interpQty) {
      yl[i] = yl_interp[i * interp];
      yr[i] = yr_interp[i * interp];
    } else {
      yl[i] = xl[i];
      yr[i] = xr[i];
    }
  }

  int16_t* resLeftData  = (int16_t*) malloc(sampleQty*sizeof(int16_t));
  int16_t* resRightData = (int16_t*) malloc(sampleQty*sizeof(int16_t));

  for (u_int32_t i = 0; i < sampleQty; i++) {
    resLeftData[i] = (int16_t)(yl[i] * DATA_AMPL);
    if(header.numChanels == 2) {
      resRightData[i] = (int16_t)(yr[i] * DATA_AMPL);
    }
  }

  fwrite(&header, sizeof(Header) - 2 * sizeof(u_int32_t), 1, resFile);
  fwrite(&extendedHeader, sizeof(u_int8_t), extendedHeaderIndex, resFile);
  fwrite(&(header.dataIndicator), sizeof(u_int32_t), 1, resFile);
  fwrite(&(header.dataSize), sizeof(u_int32_t), 1, resFile);

  for (u_int32_t i = 0; i < sampleQty; i++) {
    fwrite(&(resLeftData[i]), sizeof(int16_t), 1, resFile);
    if(header.numChanels == 2) {
      fwrite(&(resRightData[i]), sizeof(int16_t), 1, resFile);
    }
  }

  fclose(srcFile);
  fclose(resFile);

  free(xl);
  free(xr);
  free(yl);
  free(yr);
  free(xl_interp);
  free(xr_interp);
  free(yl_interp);
  free(yr_interp);
  free(srcLeftData);
  free(srcRightData);
  free(resLeftData);
  free(resRightData);

  printf("\nFinish...\n");
  return 1;
}