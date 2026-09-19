package com.dcam.node;

parcelable SessionOpenResult {
    int status = 0;
    long sessionId = 0;
    int grantedZslDepth = 0;
    String moduleId = "";
    String moduleVersion = "";
    String nodeTypeId = "";
    String message = "";
    int runtimeAbi = 1;
}
