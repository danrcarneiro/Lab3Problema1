#include <stdio.h>
#include <stdlib.h>

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
	resFile = fopen("../assets/echo_float.wav", "w+");

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

	u_int32_t delay = header.sampleRate / 2;
	float C = 0.6;

	float* yl = (float*) malloc(sampleQty*sizeof(float));
	float* yr = (float*) malloc(sampleQty*sizeof(float));

	for (u_int32_t i = 0; i < delay; i++) {
		yl[i] = xl[i];
		yr[i] = xr[i];
	}
	
	for (u_int32_t i = delay; i < sampleQty; i++) {
		yl[i] = C * xl[i] + (1-C) * (xl[i-delay]);
		yr[i] = C * xr[i] + (1-C) * (xr[i-delay]);
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
	free(srcLeftData);
	free(srcRightData);
	free(resLeftData);
	free(resRightData);

	printf("\nFinish...\n");
	return 1;
}