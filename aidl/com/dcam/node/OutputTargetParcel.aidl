package com.dcam.node;

import android.os.ParcelFileDescriptor;

parcelable OutputTargetParcel {
    long sessionId = 0;
    long captureId = 0;
    String expectedMimeType = "";
    long maximumBytes = 0;
    @nullable ParcelFileDescriptor outputFd;
}