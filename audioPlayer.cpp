#include "audioPlayer.h"

AudioPlayer::AudioPlayer(const QString &audioFile) {
	// Initialized streamer
	gst_init(nullptr, nullptr);
	// construct pipeline
	decoder = gst_element_factory_make("uridecodebin", "decoder");
	converter = gst_element_factory_make("audioconvert", "converter");
	sink = gst_element_factory_make("alsasink", "sink");
	pipeline = gst_pipline_new("cue");
	if (!pipeline || !decoder || !converter || !sink) {
		cout << "Not all gstreamer elements could be created" << endl;
		return;
	}
	gst_bin_add_many(GST_BIN(pipeline), decoder, converter, sink, NULL);
	if (gst_element_link(decoder, converter) != true ||
			gst_element_link(converter, sink) != true) {
		cout << "Gstreamer element link error" << endl;
		return;
	}
	audioFileStr = "file://" + audioFile.toStdString();
	g_object_set(decoder, "uri", audioFileStr.c_str());
}

AudioPlayer::~AudioPlayer() {
	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(pipeline);
}

int AudioPlayer::play() {
	ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
	if (ret == GST_STATE_CHANGE_FAILURE) {
		cout << "Unable to set pipeline to playing state." << endl;
		return -1;
	}
	return 0;
}

int AudioPlayer::stop() {
	ret = gst_element_set_state(pipeline, GST_STATE_PAUSE);
	if (ret == GST_STATE_CHANGE_FAILURE) {
		cout << "Unable to set pipeline to pause state." << endl;
		return -1;
	}
	// Seek to 0, ready to restart.
	gst_element_seek_simple(pipeline, GST_FORMAT_TIME,
			GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT, 
			0 * GST_SECOND);
	return 0;
}
