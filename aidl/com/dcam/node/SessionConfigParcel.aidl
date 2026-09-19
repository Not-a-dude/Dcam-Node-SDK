package com.dcam.node;

parcelable SessionConfigParcel {
    long requestedSessionId = 0;
    int clientAbi = 1;
    String nodeTypeId = "";
    String instanceId = "";
    String configJson = "{}";
    int requestedZslDepth = 0;
    boolean rendersPreview = false;
    int runtimeAbi = 1;
    String presetJson = "{}";
    String streamCatalogJson = "{}";
    String requestKeyCatalogJson = "{}";
    String characteristicsJson = "{}";
    String initialRequestStateJson = "{}";
    String initialParametersJson = "{}";
}
