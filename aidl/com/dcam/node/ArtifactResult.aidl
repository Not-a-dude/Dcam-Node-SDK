package com.dcam.node;

parcelable ArtifactResult {
    long sessionId = 0;
    long captureId = 0;
    long artifactId = 0;
    int status = 0;
    String displayName = "";
    String mimeType = "application/octet-stream";
    long bytesWritten = 0;
    String message = "";
}