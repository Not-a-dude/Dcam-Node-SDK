package com.dcam.node;

import com.dcam.node.RequestStateEntry;

parcelable RequestStateTransaction {
    long sessionId = 0;
    int scope = 0;
    int templateId = 0;
    long revision = 0;
    long baseRevision = 0;
    int[] targetStreamIds = {};
    RequestStateEntry[] entries = {};
    int lifetime = 0;
}