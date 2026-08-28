package com.dcam.node;

import android.os.ParcelFileDescriptor;

parcelable FrameRelease {
    long sessionId = 0;
    long frameId = 0;
    int disposition = 0;
    int status = 0;
    int flags = 0;
    long hardwareBufferId = 0;
    @nullable ParcelFileDescriptor releaseFence;
    String message = "";
}