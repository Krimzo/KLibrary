#include "graphics/video.h"

#include "utility/console.h"
#include "utility/strings.h"

#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfplay.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "propsys.lib")


static int vidInstCount = 0;
static bool vidUtilInited = false;
static std::mutex vidThrdLock = {};
void VidUtilInit()
{
	vidThrdLock.lock();
	vidInstCount++;
	if (!vidUtilInited)
	{
		kl::console::error((FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED | COINIT_SPEED_OVER_MEMORY)) || FAILED(MFStartup(MF_VERSION))),
			"Failed to initialize video utility");
		vidUtilInited = true;
	}
	vidThrdLock.unlock();
}
void VidUtilUninit()
{
	vidThrdLock.lock();
	vidInstCount--;
	if (vidUtilInited && vidInstCount == 0)
	{
		MFShutdown();
		CoUninitialize();
		vidUtilInited = false;
	}
	vidThrdLock.unlock();
}

kl::video::video()
{
	VidUtilInit();
}
kl::video::video(const std::string& filePath) : kl::video()
{
	open(filePath);
}
kl::video::~video()
{
	close();
	VidUtilUninit();
}

void ConfigureDecoder(IMFSourceReader* reader)
{
	IMFMediaType* defType = nullptr;
	kl::console::error(FAILED(reader->GetNativeMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &defType)), "Failed to get default video type");

	GUID majorType = {};
	kl::console::error(FAILED(defType->GetGUID(MF_MT_MAJOR_TYPE, &majorType)), "Failed to get major video type");

	IMFMediaType* newType = nullptr;
	kl::console::error(FAILED(MFCreateMediaType(&newType)), "Failed to create new video type");
	kl::console::error(FAILED(newType->SetGUID(MF_MT_MAJOR_TYPE, majorType)), "Failed to set major video type");
	kl::console::error(FAILED(newType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32)), "Failed to set sub video type");
	kl::console::error(FAILED(reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, nullptr, newType)), "Failed to set video type");

	defType->Release();
	newType->Release();
}

void kl::video::open(const std::string& filePath)
{
	close();
	IMFAttributes* attr = nullptr;
	MFCreateAttributes(&attr, 1);
	attr->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, true);
	std::wstring wFilePath = kl::to::wstring(filePath);
	MFCreateSourceReaderFromURL(wFilePath.c_str(), attr, &m_Reader);
	if (m_Reader)
	{
		ConfigureDecoder(m_Reader);
	}
	attr->Release();
}

void kl::video::close()
{
	if (m_Reader)
	{
		m_Reader->Release();
		m_Reader = nullptr;
	}
}

bool kl::video::isOpen() const
{
	return bool(m_Reader);
}

uint64 kl::video::byteSize() const
{
	if (m_Reader)
	{
		PROPVARIANT var = {};
		if (SUCCEEDED(m_Reader->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE, MF_PD_TOTAL_FILE_SIZE, &var)))
		{
			uint64 byteSize = 0;
			PropVariantToUInt64(var, &byteSize);
			PropVariantClear(&var);
			return byteSize;
		}
	}
	return 0;
}

float kl::video::duration() const
{
	if (m_Reader)
	{
		PROPVARIANT var = {};
		if (SUCCEEDED(m_Reader->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE, MF_PD_DURATION, &var)))
		{
			int64 nanoTime = 0;
			PropVariantToInt64(var, &nanoTime);
			PropVariantClear(&var);
			return (nanoTime * 1e-7f);
		}
	}
	return 0.0f;
}

float kl::video::frameRate() const
{
	if (m_Reader)
	{
		IMFMediaType* currentType = nullptr;
		m_Reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, &currentType);
		if (currentType)
		{
			uint num = 0, den = 0;
			MFGetAttributeRatio(currentType, MF_MT_FRAME_RATE, &num, &den);
			currentType->Release();
			return float(num) / den;
		}
	}
	return 0.0f;
}

float kl::video::frameTime() const
{
	return 1.0f / frameRate();
}

uint64 kl::video::frameCount() const
{
	return uint64(frameRate() * duration());
}

kl::uint2 kl::video::frameSize() const
{
	if (m_Reader)
	{
		IMFMediaType* currentType = nullptr;
		m_Reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, &currentType);
		if (currentType)
		{
			kl::uint2 frameSize;
			MFGetAttributeSize(currentType, MF_MT_FRAME_SIZE, &frameSize.x, &frameSize.y);
			currentType->Release();
			return frameSize;
		}
	}
	return {};
}

bool kl::video::getFrame(kl::image& out)
{
	bool state = false;
	if (m_Reader)
	{
		DWORD flags = 0; int64 timeStamp = 0; IMFSample* sample = nullptr;
		m_Reader->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, NULL, nullptr, &flags, &timeStamp, &sample);
		if (sample)
		{
			IMFMediaBuffer* frameBuff = nullptr;
			sample->ConvertToContiguousBuffer(&frameBuff);
			if (frameBuff)
			{
				byte* frameData = nullptr; DWORD dataLen = 0;
				frameBuff->Lock(&frameData, nullptr, &dataLen);
				out.resize(frameSize());
				memcpy(out.data(), frameData, dataLen);
				frameBuff->Release();
				state = true;
			}
			sample->Release();
		}
	}
	return state;
}
