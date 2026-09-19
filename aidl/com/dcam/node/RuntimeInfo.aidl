package com.dcam.node;

parcelable RuntimeInfo {
    int runtimeAbi = 1;
    int pid = 0;
    int uid = 0;
    boolean isolated = false;
    boolean vulkanAvailable = false;
    String runtimeVersion = "";
    String diagnostics = "";
}
