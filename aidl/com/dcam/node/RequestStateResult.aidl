package com.dcam.node;

parcelable RequestStateResult {
    long sessionId = 0;
    long revision = 0;
    boolean applied = false;
    int errorCode = 0;
    int keyId = 0;
    String keyName = "";
    String message = "";
}