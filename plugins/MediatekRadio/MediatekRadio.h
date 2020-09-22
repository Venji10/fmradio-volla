/*
 * Copyright (C) 2020 venji10 <bennisteinir@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MEDIATEKRADIO_H
#define MEDIATEKRADIO_H

#include <QObject>

class MediatekRadio : public QObject {
	Q_OBJECT

private:
	bool radioRunning = false;
	bool isHeadset = false;
	int frequency = 10250;
	int idx = -1;
	void preparePulseAudio();
	void mute();
	void unmute();
	bool isHeadsetAvailable();
	int getRssi();
	void startVolumeUpdater();
	void stopVolumeUpdater();

public:
	MediatekRadio();
	~MediatekRadio() /* = default*/ ;

	Q_INVOKABLE QByteArray startRadio(int);
	Q_INVOKABLE QByteArray stopRadio();
	Q_INVOKABLE void tune(int);
	Q_INVOKABLE int getFrequency();
	Q_INVOKABLE int seekUp();
	Q_INVOKABLE int seekDown();
	Q_INVOKABLE bool isRadioRunning();

};

#endif
