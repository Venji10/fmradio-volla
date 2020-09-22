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


#include <stdio.h>

#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <string.h>
#include <iostream>
#include <thread>

#include "MediatekRadio.h"
#include "common.cpp"

#define FM_DEV_NAME "/dev/fm"

#define FM_BAND_UE      1 // US/Europe band  87.5MHz ~ 108MHz (DEFAULT)
#define FM_BAND_JAPAN   2 // Japan band      76MHz   ~ 90MHz
#define FM_BAND_JAPANW  3 // Japan wideband  76MHZ   ~ 108MHz
#define FM_BAND_SPECIAL 4 // special   band  between 76MHZ   and  108MHz


MediatekRadio::MediatekRadio() {

}
// check if headset/headphones are connected, they are act as antenna
bool MediatekRadio::isHeadsetAvailable() {

	FILE *fp;
	char path[45];

	fp = popen("/usr/bin/pactl list sinks | /bin/grep \"Active Port:\"", "r");

	if(fp == NULL) {
		printf("error checking for headset");
		return false;
	}



	while (fgets(path, sizeof(path), fp) != NULL) {
		printf("%s", path);
	}

	printf("\n");

	pclose(fp);

	if(strcmp(path, "	Active Port: output-wired_headset") < 0) {
		printf("not using headset\n");

		if(!strcmp(path, "	Active Port: output-wired_headphone") < 0) {
			return true;
		}

		return false;

	}

	isHeadset = true;
	return true;

}

// This is needed to route the FM input to the headphones
void MediatekRadio::preparePulseAudio() {

	int ret;

	ret = system("pacmd set-source-port 1 input-fm_tuner");
	ret = system("pactl load-module module-loopback source=1 sink=0");

}

bool MediatekRadio::isRadioRunning() {

        return radioRunning;

}

// Volume is always at 100% without this
void MediatekRadio::startVolumeUpdater() {

	system("touch ~/.radioRunning");
	system("while ( test -f ~/.radioRunning) do $(pactl set-source-volume 1 $(printf \"%.*f\\n\" 0 $(echo print $(dbus-send --session --type=method_call --print-reply --dest=com.canonical.indicator.sound /com/canonical/indicator/sound org.gtk.Actions.DescribeAll | grep -n5 \"string \\\"volume\\\"\" | grep double | cut -b 53-56)*65536 | perl))); done &");

}

void MediatekRadio::stopVolumeUpdater() {

	system("rm ~/.radioRunning");
	system("pactl set-source-volume 1 65536");

}

// Start the radio
QByteArray MediatekRadio::startRadio(int freq) {

	if(isHeadsetAvailable()) {

		int ret = 0;

		if((ret = COM_open_dev(FM_DEV_NAME, &idx)) < 0) {
			printf("error opening device: %d\n", ret);
			return "Error";
		}

		if((ret = COM_pwr_up(idx, FM_BAND_UE, freq)) < 0) {
			printf("error powering up: %d\n", ret);
			return "Error";
		}

		preparePulseAudio();

		radioRunning = true;
		startVolumeUpdater();

		return "Stop radio";
	} else {
		return "Headset not available";
	}

}

// Stop the radio
QByteArray MediatekRadio::stopRadio() {

	int ret = 0;

	if((ret = COM_pwr_down(idx, 0)) < 0) {
		printf("error powering down: %d\n", ret);
		return "Error";
	}

	if((ret = COM_close_dev(idx)) < 0) {
		printf("error closing device; %d\n", ret);
		return "Error";
	}

	ret = system("pactl unload-module module-loopback");

	if(isHeadset) {
		ret = system("pacmd set-source-port 1 input-wired_headset && pacmd set-sink-port 0 output-wired_headset");
	} else {
		ret = system("pacmd set-source-port 1 input-builtin_mic && pacmd set-sink-port 0 output-wired_headphone");
	}

	stopVolumeUpdater();

	radioRunning = false;

	return "Start radio";

}

// Tune to different frequency
void MediatekRadio::tune(int freq) {

	if(isRadioRunning()) {
		int ret;

		if((ret = COM_tune(idx, freq, FM_BAND_UE)) < 0) {
			printf("error tuning: %d\n", ret);
		}
	} else {
		printf("tune: radio not running\n");
	}

	frequency = freq;

}

void MediatekRadio::mute() {

	int ret;

	if((ret = COM_set_mute(idx, 1)) < 0) {
		printf("error muting: %d\n", ret);
	}

}

void MediatekRadio::unmute() {

	int ret;

	if((ret = COM_set_mute(idx, 0)) < 0) {
		printf("error unmuting: %d\n", ret);
	}


}

int MediatekRadio::getRssi() {

	int ret;
	int rssi;

	if((ret = COM_get_rssi(idx, &rssi)) < 0) {
		printf("error getting rssi: %d\n", ret);
		return -100;
	}

	return rssi;

}

int MediatekRadio::seekUp() {

	if(radioRunning) {

		mute();

		bool foundStation = false;
		int tmp = frequency;

		for(int i = frequency + 10; i < 10800; i += 10) {
			tune(i);
			if(getRssi() > -75) {
				foundStation = true;
				break;
			}
		}

		if(!foundStation) {
			tune(tmp);
		}
		unmute();

	}

	return frequency;

}

int MediatekRadio::seekDown() {

	if(radioRunning) {

		mute();

		bool foundStation = false;
		int tmp = frequency;

		for(int i = frequency - 10; i > 8750; i -= 10) {
			tune(i);
			if(getRssi() > -75) {
				foundStation = true;
				break;
			}
		}

		if(!foundStation) {
			tune(tmp);
		}
		unmute();

	}

	return frequency;

}

int MediatekRadio::getFrequency() {

	return frequency;

}

MediatekRadio::~MediatekRadio() {

	stopRadio();

}
