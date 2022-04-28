#include "graphics/video.h"

#include "utility/console.h"
#include "utility/convert.h"


static int vidInstCount = 0;
static bool vidUtilInited = false;
static std::mutex vidThrdLock = {};
void VidUtilInit() {
	vidThrdLock.lock();
	vidInstCount++;
	if (!vidUtilInited) {
		if ((FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED | COINIT_SPEED_OVER_MEMORY)) || FAILED(MFStartup(MF_VERSION)))) {
			kl::console::show();
			std::cout << "Failed to init video loader!";
			std::cin.get();
			exit(69);
		}
		vidUtilInited = true;
	}
	vidThrdLock.unlock();
}
void VidUtilUninit() {
	vidThrdLock.lock();
	vidInstCount--;
	if (vidUtilInited && vidInstCount == 0) {
		MFShutdown();
		CoUninitialize();
		vidUtilInited = false;
	}
	vidThrdLock.unlock();
}

kl::video::video() {
	VidUtilInit();
}
kl::video::video(const String& filePath) {
	VidUtilInit();
	open(filePath);
}
kl::video::~video() {
	close();
	VidUtilUninit();
}

void ConfigureDecoder(IMFSourceReader* reader) {
	// Getting default types
	IMFMediaType* defType = nullptr;
	if (FAILED(reader->GetNativeMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &defType))) {
		exit(1);
	}
	GUID majorType = {};
	if (FAILED(defType->GetGUID(MF_MT_MAJOR_TYPE, &majorType))) {
		exit(2);
	}

	// Creating/setting new type
	IMFMediaType* newType = nullptr;
	if (FAILED(MFCreateMediaType(&newType))) {
		exit(3);
	}
	if (FAILED(newType->SetGUID(MF_MT_MAJOR_TYPE, majorType))) {
		exit(4);
	}
	if (FAILED(newType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32))) {
		exit(6);
	}
	if (FAILED(reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, nullptr, newType))) {
		exit(7);
	}

	// Cleanup
	defType->Release();
	newType->Release();
}
void kl::video::open(const String& filePath) {
	close();
	IMFAttributes* attr = nullptr;
	MFCreateAttributes(&attr, 1);
	attr->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, true);
	std::wstring wFilePath = kl::convert::toWString(filePath);
	MFCreateSourceReaderFromURL(wFilePath.c_str(), attr, &reader);
	if (reader) {
		ConfigureDecoder(reader);
	}
	attr->Release();
}
void kl::video::close() {
	if (reader) {
		reader->Release();
		reader = nullptr;
	}
}

bool kl::video::isOpen() const {
	return reader;
}

float kl::video::duration() const {
	if (reader) {
		PROPVARIANT var = {};
		if (SUCCEEDED(reader->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE, MF_PD_DURATION, &var))) {
			LONGLONG nanoTime = 0;
			PropVariantToInt64(var, &nanoTime);
			PropVariantClear(&var);
			return (nanoTime * 1e-7f);
		}
	}
	return 0.0f;
}

size_t kl::video::byteSize() const {
	if (reader) {
		PROPVARIANT var = {};
		if (SUCCEEDED(reader->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE, MF_PD_TOTAL_FILE_SIZE, &var))) {
			size_t byteSize = 0;
			PropVariantToInt64(var, (LONGLONG*)&byteSize);
			PropVariantClear(&var);
			return byteSize;
		}
	}
	return 0;
}

kl::int2 kl::video::frameSize() const {
	if (reader) {
		IMFMediaType* currentType = nullptr;
		reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, &currentType);
		if (currentType) {
			kl::int2 frameSize;
			MFGetAttributeSize(currentType, MF_MT_FRAME_SIZE, (UINT32*)&frameSize.x, (UINT32*)&frameSize.y);
			currentType->Release();
			return frameSize;
		}
	}
	return {};
}

float kl::video::frameRate() const {
	if (reader) {
		IMFMediaType* currentType = nullptr;
		reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, &currentType);
		if (currentType) {
			UINT32 num = 0, den = 0;
			MFGetAttributeRatio(currentType, MF_MT_FRAME_RATE, &num, &den);
			currentType->Release();
			return float(num) / den;
		}
	}
	return 0.0f;
}

bool kl::video::getFrame(kl::image& out) {
	bool state = false;
	if (reader) {
		DWORD flags = 0; LONGLONG timeStamp = 0; IMFSample* sample = nullptr;
		reader->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, NULL, nullptr, &flags, &timeStamp, &sample);
		if (sample) {
			IMFMediaBuffer* frameBuff = nullptr;
			sample->ConvertToContiguousBuffer(&frameBuff);
			if (frameBuff) {
				byte* frameData = nullptr; DWORD dataLen = 0;
				frameBuff->Lock(&frameData, nullptr, &dataLen);
				out.resize(frameSize());
				memcpy(out.pointer(), frameData, dataLen);
				frameBuff->Release();
				state = true;
			}
			sample->Release();
		}
	}
	return state;
}
