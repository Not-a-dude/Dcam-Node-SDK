package com.dcam.node;

parcelable RuntimeEvent {
    long sessionId = 0;
    int severity = 0;
    int code = 0;
    String component = "";
    String message = "";
}