package com.dcam.node;

parcelable CaptureEvent {
    long sessionId = 0;
    long captureId = 0;
    int eventType = 0;
    int status = 0;
    long frameId = 0;
    String message = "";
}