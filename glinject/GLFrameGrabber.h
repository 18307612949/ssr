/*
Copyright (c) 2012-2014 Maarten Baert <maarten-baert@hotmail.com>

Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#pragma once
#include "Global.h"

class GLFrameGrabber {

public:
	struct HotkeyInfo {
		bool enabled;
		unsigned int keycode, modifiers;
	};

private:
	Display *m_x11_display;
	Window m_x11_window;
	GLXDrawable m_glx_drawable;
	unsigned int m_width, m_height;

	bool m_debug;
	unsigned int m_ring_buffer_size, m_max_bytes;
	unsigned int m_target_fps;
	unsigned int m_flags;

	unsigned int m_gl_version;
	bool m_has_xfixes;

	char *m_shm_main_ptr;
	std::vector<char*> m_shm_frame_ptrs;

	int64_t m_next_frame_time;

	bool m_warn_too_small, m_warn_too_large;

public:
	GLFrameGrabber(Display* display, Window window, GLXDrawable drawable);
	~GLFrameGrabber();

	void GrabFrame();
	HotkeyInfo GetHotkeyInfo();
	void TriggerHotkey();

public:
	inline Display* GetX11Display() { return m_x11_display; }
	inline Window GetX11Window() { return m_x11_window; }
	inline GLXDrawable GetGLXDrawable() { return m_glx_drawable; }

};
