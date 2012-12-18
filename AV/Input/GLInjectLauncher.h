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

class Logger;

// Explicit padding is needed here to keep the 32-bit and 64-bit code compatible.
// I could also disable padding of course, but this is easier :)
struct GLInjectHeader {
	uint32_t cbuffer_size, max_pixels;
	uint32_t read_pos, write_pos;
	uint32_t current_width, current_height;
};
struct GLInjectFrameInfo {
	int32_t shm_id, padding1;
	int64_t timestamp;
	uint32_t width, height;
};

class GLInjectLauncher {

public:
	static const unsigned int CBUFFER_SIZE;

private:
	struct ShmFrame {
		int id;
		volatile char *ptr;
		inline ShmFrame() : id(-1), ptr((char*) -1) {}
	};

private:
	Logger *m_logger;

	QString m_command;
	unsigned int m_max_pixels;
	bool m_run_command;

	int m_shm_main_id;
	volatile char *m_shm_main_ptr;
	std::vector<ShmFrame> m_shm_frames;

public:
	GLInjectLauncher(Logger* logger, const QString& command, unsigned int max_pixels, bool run_command);
	~GLInjectLauncher();

	// Reads the current size of the OpenGL application. If the application hasn't been started yet, this will be 0x0.
	void GetCurrentSize(unsigned int* width, unsigned int* height);

	// Returns the maximum number of pixels.
	inline unsigned int GetMaxPixels() { return m_max_pixels; }

	// Returns the number of frames in the circular buffer.
	inline unsigned int GetCBufferSize() { return CBUFFER_SIZE; }

	// Returns a pointer to the main shared memory block used for communication with the injected library.
	inline volatile char* GetMainSharedPointer() { return m_shm_main_ptr; }

	// Returns a pointer to a frame shared memory block.
	inline volatile char* GetFrameSharedPointer(unsigned int frame) { return m_shm_frames[frame].ptr; }

private:
	void Init();
	void Free();

};
