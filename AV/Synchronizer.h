/*
Copyright (c) 2012 Maarten Baert <maarten-baert@hotmail.com>

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
#include "StdAfx.h"

#include "VPair.h"
#include "ByteQueue.h"
#include "AVWrapper.h"

class VideoEncoder;
class AudioEncoder;

class Synchronizer {

private:
	struct SharedData {

		std::deque<std::unique_ptr<AVFrameWrapper> > m_video_buffer;
		ByteQueue m_audio_buffer;
		std::unique_ptr<AVFrameWrapper> m_partial_audio_frame;
		int64_t m_video_pts, m_audio_samples; // video and audio position in the final stream (encoded frames and samples in the partial audio frame)
		double m_time_correction_factor; // correction factor used to synchronize video and audio time

		int64_t m_time_offset; // the length of all previous segments combined (in microseconds)
		bool m_segment_video_started, m_segment_audio_started; // whether video and audio have started (always true if the corresponding stream is disabled)
		int64_t m_segment_video_start_time, m_segment_audio_start_time; // the start time of video and audio (real-time, in microseconds)
		int64_t m_segment_video_stop_time, m_segment_audio_stop_time; // the stop time of video and audio (real-time, in microseconds)
		int64_t m_segment_audio_offset; // the offset in the final stream corresponding to the audio start time
		int64_t m_segment_audio_samples_read; // the number of samples that have been read from the audio buffer (including dropped samples)

		bool m_warn_drop_video, m_warn_drop_audio, m_warn_desync;

	};
	typedef VPair<SharedData>::Lock SharedLock;

private:
	static const double CORRECTION_SPEED;
	static const size_t MAX_VIDEO_FRAMES_BUFFERED, MAX_AUDIO_SAMPLES_BUFFERED;

private:
	VideoEncoder *m_video_encoder;
	AudioEncoder *m_audio_encoder;

	unsigned int m_video_frame_rate;
	unsigned int m_audio_sample_rate, m_audio_sample_size;
	unsigned int m_audio_required_frame_size, m_audio_required_sample_size;
	AVSampleFormat m_audio_required_sample_format;

	std::vector<char> m_temp_audio_buffer; // stores the original samples for a partial frame with a different sample format

	VPair<SharedData> m_shared_data;

public:
	// The arguments 'video_encoder' and 'audio_encoder' can be NULL to disable video or audio.
	Synchronizer(VideoEncoder* video_encoder, AudioEncoder* audio_encoder);

	// This function tells the synchronizer to end the current segment and reset the synchronization system
	// in preparation for a new segment. This is required for pausing and continuing a recording.
	// This function has no effect if there are no frames in the current segment, so it is safe to call this multiple times.
	// This function is thread-safe, but for best results you should still make sure that no input is running
	// while this function is called, because otherwise frames may end up in the wrong segment.
	void NewSegment();

	// Returns the total recording time (in microseconds).
	// This function is thread-safe.
	int64_t GetTotalTime();

	inline VideoEncoder* GetVideoEncoder() { return m_video_encoder; }
	inline AudioEncoder* GetAudioEncoder() { return m_audio_encoder; }

public: // internal

	// Adds a frame to the video queue. Called by the input.
	// The timestamp contains the capture time of the frame.
	// This function is thread-safe.
	void AddVideoFrame(std::unique_ptr<AVFrameWrapper> frame, int64_t timestamp);

	// Adds samples to the audio queue. Called by the input.
	// The timestamp contains the capture time of the first sample.
	// This function is thread-safe.
	void AddAudioSamples(const char* samples, size_t samplecount, int64_t timestamp);

private:

	void GetSegmentStartStop(SharedData* lock, int64_t* segment_start_time, int64_t* segment_stop_time);
	void FlushBuffers(SharedData* lock);
	void ClearBuffers(SharedData* lock);

};
