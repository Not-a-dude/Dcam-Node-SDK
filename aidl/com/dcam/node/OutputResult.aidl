package com.dcam.node;

parcelable OutputResult {
    long sessionId = 0;
    long captureId = 0;
    int status = 0;
    String mimeType = "";
    long bytesWritten = 0;
    String message = "";
}