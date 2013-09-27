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

class GLInjectLauncher {

public:
	static const unsigned int RING_BUFFER_SIZE;

private:
	struct ShmFrame {
		int m_id;
		char *m_shm_ptr;
		inline ShmFrame() : m_id(-1), m_shm_ptr((char*) -1) {}
	};

private:
	QString m_command;
	bool m_run_command, m_relax_permissions;
	unsigned int m_max_bytes;
	unsigned int m_target_fps;
	bool m_record_cursor, m_capture_front, m_limit_fps;

	unsigned int m_hotkey_last_counter;

	int m_shm_main_id;
	char *m_shm_main_ptr;
	std::vector<ShmFrame> m_shm_frames;

public:
	GLInjectLauncher(const QString& command, bool run_command, bool relax_permissions, unsigned int max_bytes, unsigned int target_fps, bool record_cursor, bool capture_front, bool limit_fps);
	~GLInjectLauncher();

	// Reads the current size of the OpenGL application. If the application hasn't been started yet, this will be 0x0.
	void GetCurrentSize(unsigned int* width, unsigned int* height);

	// Returns the total number of captured frames.
	uint32_t GetFrameCounter();

	// Updates the hotkey used to start the capture.
	void UpdateHotkey(bool enabled, unsigned int keysym, unsigned int modifiers);

	// Returns whether the hotkey was pressed in the injected code.
	bool GetHotkeyPressed();

	// Returns the maximum number of bytes.
	inline unsigned int GetMaxBytes() { return m_max_bytes; }

	// Returns the number of frames in the ring buffer.
	inline unsigned int GetRingBufferSize() { return RING_BUFFER_SIZE; }

	// Returns a pointer to the main shared memory block used for communication with the injected library.
	inline char* GetMainSharedPointer() { return m_shm_main_ptr; }

	// Returns a pointer to a frame shared memory block.
	inline char* GetFrameSharedPointer(unsigned int frame) { return m_shm_frames[frame].m_shm_ptr; }

private:
	void Init();
	void Free();

};
