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

#include "../glinject/ShmStructs.h"

class SSRVideoStreamReader {

private:
	struct FrameData {
		std::string m_filename_frame;
		int m_fd_frame;
		void *m_mmap_ptr_frame;
		size_t m_mmap_size_frame;
	};

private:
	std::string m_filename_main;
	size_t m_page_size;

	int64_t m_info_last_timestamp;
	uint32_t m_last_frame_counter;

	int m_fd_main, m_file_lock;
	void *m_mmap_ptr_main;
	size_t m_mmap_size_main;

	FrameData m_frame_data[GLINJECT_RING_BUFFER_SIZE];

public:
	SSRVideoStreamReader(const std::string& pid, const std::string& source, const std::string& program_name);
	~SSRVideoStreamReader();

private:
	void Init();
	void Free();

	GLInjectHeader* GetGLInjectHeader();
	GLInjectFrameInfo* GetGLInjectFrameInfo(unsigned int frame);

public:
	// Reads the current size of the stream. If the stream hasn't been started yet, this will be 0x0.
	void GetCurrentSize(unsigned int* width, unsigned int* height);

	// Returns the current fps.
	double GetFPS();

	// Changes the capture parameters.
	void ChangeCaptureParameters(unsigned int flags, unsigned int target_fps);

	// Clears the ring buffer (i.e. drops all frames).
	void Clear();

	// Checks whether a new frame is available, and returns a pointer to the frame memory if it is. Otherwise it returns NULL.
	void* GetFrame(int64_t* timestamp, unsigned int* width, unsigned int* height, int* stride);

	// Drops the current frame and goes to the next frame.
	void NextFrame();

};
