package com.dcam.node;

import com.dcam.node.MetadataEntry;

parcelable MetadataParcel {
    int wireVersion = 1;
    long metadataId = 0;
    MetadataEntry[] entries = {};
}