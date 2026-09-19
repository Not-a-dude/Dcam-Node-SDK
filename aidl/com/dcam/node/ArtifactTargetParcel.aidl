package com.dcam.node;

import android.os.ParcelFileDescriptor;

parcelable ArtifactTargetParcel {
    long sessionId = 0;
    long captureId = 0;
    long artifactId = 0;
    String displayName = "";
    String mimeType = "application/octet-stream";
    long maximumBytes = 0;
    @nullable ParcelFileDescriptor outputFd;
}