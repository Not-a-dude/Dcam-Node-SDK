package com.dcam.node;

import android.hardware.HardwareBuffer;
import android.os.ParcelFileDescriptor;
import com.dcam.node.MetadataParcel;

parcelable FrameParcel {
    long sessionId = 0;
    int streamId = 0;
    long frameId = 0;
    long timestampNs = 0;
    int width = 0;
    int height = 0;
    int layers = 1;
    int format = 0;
    long usage = 0;
    int dataSpace = 0;
    int cropLeft = 0;
    int cropTop = 0;
    int cropRight = 0;
    int cropBottom = 0;
    long requestRevision = 0;
    @nullable HardwareBuffer hardwareBuffer;
    @nullable ParcelFileDescriptor acquireFence;
    @nullable MetadataParcel metadata;
}