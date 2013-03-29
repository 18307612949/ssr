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
#include "StdAfx.h"

class Synchronizer;

class ALSAInput : private QThread {

private:
	Synchronizer *m_synchronizer;

	QString m_device_name;
	unsigned int m_sample_rate, m_channels;

	snd_pcm_t *m_alsa_pcm;
	unsigned int m_alsa_periods, m_alsa_period_size;

	volatile bool m_should_stop, m_error_occurred;

public:
	ALSAInput(Synchronizer* synchronizer, const QString& device_name);
	~ALSAInput();

	// Returns whether an error has occurred in the input thread.
	// This function is thread-safe.
	inline bool HasErrorOccurred() { return m_error_occurred; }

private:
	void Init();
	void Free();
	int64_t GetReadDelay();

private slots:
	void UpdateScreenConfiguration();

private:
	virtual void run();

};
