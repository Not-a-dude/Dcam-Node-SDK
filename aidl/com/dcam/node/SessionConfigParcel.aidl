package com.dcam.node;

parcelable SessionConfigParcel {
    long requestedSessionId = 0;
    int runtimeAbi = 2;
    String nodeTypeId = "";
    String instanceId = "";
    String presetJson = "{}";
    String streamCatalogJson = "{}";
    String requestKeyCatalogJson = "{}";
    String characteristicsJson = "{}";
    String initialRequestStateJson = "{}";
    String initialParametersJson = "{}";
    int requestedZslDepth = 0;
    boolean rendersPreview = false;
}
