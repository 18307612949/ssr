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

#pragma once
#include "Global.h"

#include "Muxer.h"
#include "VideoEncoder.h"
#include "AudioEncoder.h"
#include "Synchronizer.h"
#include "OutputSettings.h"

class OutputManager {

private:
	OutputSettings m_output_settings;

	Muxer *m_muxer;
	VideoEncoder *m_video_encoder;
	AudioEncoder *m_audio_encoder;
	Synchronizer *m_synchronizer;

public:
	OutputManager(const OutputSettings& output_settings);
	~OutputManager();

	void Finish();

private:
	void Init();
	void Free(bool save);

public:
	inline Muxer* GetMuxer() { return m_muxer; }
	inline VideoEncoder* GetVideoEncoder() { return m_video_encoder; }
	inline AudioEncoder* GetAudioEncoder() { return m_audio_encoder; }
	inline Synchronizer* GetSynchronizer() { return m_synchronizer; }

};
