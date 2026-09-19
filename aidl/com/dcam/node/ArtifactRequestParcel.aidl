package com.dcam.node;

parcelable ArtifactRequestParcel {
    long sessionId = 0;
    long captureId = 0;
    String suggestedName = "";
    String mimeType = "application/octet-stream";
    long maximumBytes = 0;
}