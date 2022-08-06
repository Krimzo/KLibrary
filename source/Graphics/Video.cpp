#include "graphics/video.h"

#include "utility/console.h"
#include "utility/strings.h"
#include "utility/Async.h"

#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfplay.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "propsys.lib")


namespace kl {
	static int vidInstCount = 0;
	static bool vidUtilInited = false;
	static Mutex vidThrdLock = {};

	static void VideoUtilityInit() {
		vidThrdLock.lock();
		vidInstCount++;
		if (!vidUtilInited) {
			Assert((FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED | COINIT_SPEED_OVER_MEMORY)) || FAILED(MFStartup(MF_VERSION))), "Failed to initialize video utility");
			vidUtilInited = true;
		}
		vidThrdLock.unlock();
	}

	static void VideoUtilityUninit() {
		vidThrdLock.lock();
		vidInstCount--;
		if (vidUtilInited && vidInstCount == 0) {
			MFShutdown();
			CoUninitialize();
			vidUtilInited = false;
		}
		vidThrdLock.unlock();
	}

	static void ConfigureDecoder(IMFSourceReader* reader) {
		IMFMediaType* defType = nullptr;
		Assert(FAILED(reader->GetNativeMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &defType)), "Failed to get default video type");

		GUID majorType = {};
		Assert(FAILED(defType->GetGUID(MF_MT_MAJOR_TYPE, &majorType)), "Failed to get major video type");

		IMFMediaType* newType = nullptr;
		Assert(FAILED(MFCreateMediaType(&newType)), "Failed to create new video type");
		Assert(FAILED(newType->SetGUID(MF_MT_MAJOR_TYPE, majorType)), "Failed to set major video type");
		Assert(FAILED(newType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32)), "Failed to set sub video type");
		Assert(FAILED(reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, nullptr, newType)), "Failed to set video type");

		defType->Release();
		newType->Release();
	}
}

kl::Video::Video() {
	VideoUtilityInit();
}

kl::Video::Video(const String& filePath) : kl::Video() {
	open(filePath);
}

kl::Video::~Video() {
	close();
	VideoUtilityUninit();
}

kl::Video::operator bool() const {
	return isOpen();
}

bool kl::Video::isOpen() const {
	return bool(m_Reader);
}

void kl::Video::open(const String& filePath) {
	close();
	IMFAttributes* attr = nullptr;
	MFCreateAttributes(&attr, 1);
	attr->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, true);

	WString wFilePath = Strings::ToWString(filePath);
	MFCreateSourceReaderFromURL(wFilePath.c_str(), attr, &m_Reader);
	if (m_Reader) {
		ConfigureDecoder(m_Reader);
	}

	attr->Release();
}

void kl::Video::close() {
	if (m_Reader) {
		m_Reader->Release();
		m_Reader = nullptr;
	}
}

kl::uint64 kl::Video::size() const {
	if (m_Reader) {
		PROPVARIANT var = {};
		if (SUCCEEDED(m_Reader->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE, MF_PD_TOTAL_FILE_SIZE, &var))) {
			uint64 byteSize = 0;
			PropVariantToUInt64(var, &byteSize);
			PropVariantClear(&var);
			return byteSize;
		}
	}
	return 0;
}

float kl::Video::duration() const {
	if (m_Reader) {
		PROPVARIANT var = {};
		if (SUCCEEDED(m_Reader->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE, MF_PD_DURATION, &var))) {
			int64 time = 0;
			PropVariantToInt64(var, &time);
			PropVariantClear(&var);
			return time * 1e-7f;
		}
	}
	return 0.0f;
}

float kl::Video::fps() const {
	if (m_Reader) {
		IMFMediaType* currentType = nullptr;
		m_Reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, &currentType);
		if (currentType) {
			uint num = 0, den = 0;
			MFGetAttributeRatio(currentType, MF_MT_FRAME_RATE, &num, &den);
			currentType->Release();
			return float(num) / den;
		}
	}
	return 0.0f;
}

kl::uint kl::Video::frameCount() const {
	return uint(fps() * duration());
}

kl::UInt2 kl::Video::frameSize() const {
	if (m_Reader) {
		IMFMediaType* currentType = nullptr;
		m_Reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, &currentType);
		if (currentType) {
			UInt2 frameSize = {};
			MFGetAttributeSize(currentType, MF_MT_FRAME_SIZE, &frameSize.x, &frameSize.y);
			currentType->Release();
			return frameSize;
		}
	}
	return {};
}

bool kl::Video::getFrame(Image& out) {
	bool state = false;
	if (m_Reader) {
		DWORD flags = 0; int64 timeStamp = 0; IMFSample* sample = nullptr;
		m_Reader->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, NULL, nullptr, &flags, &timeStamp, &sample);

		if (sample) {
			IMFMediaBuffer* frameBuff = nullptr;
			sample->ConvertToContiguousBuffer(&frameBuff);

			if (frameBuff) {
				byte* frameData = nullptr; DWORD dataLen = 0;
				frameBuff->Lock(&frameData, nullptr, &dataLen);

				out.setSize(frameSize());
				memcpy(out.data(), frameData, dataLen);

				frameBuff->Release();
				state = true;
			}
			sample->Release();
		}
	}
	return state;
}
