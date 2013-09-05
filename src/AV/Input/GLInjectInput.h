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

#include "SourceSink.h"
#include "MutexDataPair.h"

class Synchronizer;
class GLInjectLauncher;
class VideoPreviewer;

class GLInjectInput : public VideoSource {

private:
	static const int64_t MAX_COMMUNICATION_LATENCY;

private:
	GLInjectLauncher *m_launcher;

	unsigned int m_cbuffer_size, m_max_bytes;

	char *m_shm_main_ptr;
	std::vector<char*> m_shm_frame_ptrs;

	std::thread m_thread;
	std::atomic<bool> m_should_stop, m_error_occurred;

public:
	GLInjectInput(GLInjectLauncher* launcher);
	~GLInjectInput();

	// Returns whether an error has occurred in the input thread.
	// This function is thread-safe.
	inline bool HasErrorOccurred() { return m_error_occurred; }

private:
	void Init();
	void Free();

private:
	void InputThread();

};
