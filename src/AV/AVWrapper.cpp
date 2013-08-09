/*
Copyright (c) 2012-2013 Maarten Baert <maarten-baert@hotmail.com>

This file is part of SimpleScreenRecorder.

SimpleScreenRecorder is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

SimpleScreenRecorder is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with SimpleScreenRecorder.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Global.h"
#include "AVWrapper.h"

int lock_manager(void** m, AVLockOp op) {
	QMutex *&mutex = *(QMutex**) m;
	switch(op) {
		case AV_LOCK_CREATE: mutex = new QMutex(); break;
		case AV_LOCK_DESTROY: delete mutex; break;
		case AV_LOCK_OBTAIN: mutex->lock(); break;
		case AV_LOCK_RELEASE: mutex->unlock(); break;
	}
	return 0;
}

class AVGlobal {
public:
	AVGlobal() {
		av_register_all();
		av_lockmgr_register(&lock_manager);
#if SSR_USE_AVFORMAT_NETWORK_INIT
		avformat_network_init();
#endif
	}
} g_av_global;

AVFrameWrapper::AVFrameWrapper(size_t size) {
	avcodec_get_frame_defaults(this);
	m_refcounted_data = std::make_shared<FrameData>(size);
}

AVPacketWrapper::AVPacketWrapper() {
	m_free_on_destruct = true;
	av_init_packet(this);
	data = NULL;
	size = 0;
}

AVPacketWrapper::AVPacketWrapper(size_t size) {
	m_free_on_destruct = true;
	if(av_new_packet(this, size) != 0) {
		throw std::bad_alloc();
	}
}

AVPacketWrapper::~AVPacketWrapper() {
	if(m_free_on_destruct)
		av_free_packet(this);
}

bool AVFormatIsInstalled(const QString& format_name) {
	return (av_guess_format(qPrintable(format_name), NULL, NULL) != NULL);
}

bool AVCodecIsInstalled(const QString& codec_name) {
	return (avcodec_find_encoder_by_name(qPrintable(codec_name)) != NULL);
}

bool AVCodecSupportsPixelFormat(AVCodec* codec, PixelFormat pixel_fmt) {
	const PixelFormat *p = codec->pix_fmts;
	if(p == NULL)
		return true; // NULL means 'unknown' or 'any', assume it is supported
	while(*p != PIX_FMT_NONE) {
		if(*p == pixel_fmt)
			return true;
		++p;
	}
	return false;
}

bool AVCodecSupportsSampleFormat(AVCodec* codec, AVSampleFormat sample_fmt) {
	const AVSampleFormat *p = codec->sample_fmts;
	if(p == NULL)
		return true; // NULL means 'unknown' or 'any', assume it is supported
	while(*p != AV_SAMPLE_FMT_NONE) {
		if(*p == sample_fmt)
			return true;
		++p;
	}
	return false;
}
