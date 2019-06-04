/*
Play auditory cue from a file
Author: Chenghao Liu
Create: 2019/06/03
Modified: 2019/06/03
*/

#include <gst/gst.h>
#include <iostream>
#include <QString>
#pragma once
using namespace std;


class AudioPlayer {
	public:
		// Construct by setting up pipeline for playback audio file.
		AudioPlayer(const QString &audioFile);
		// Destruct, release resources.
		~AudioPlayer();
		// Play audio stream.
		int play();
		// Pause pipeline and seek to time 0.
		int stop();
	protected:
		GstElement *pipeline;
		GstElement *decoder, *converter, *sink;
};
