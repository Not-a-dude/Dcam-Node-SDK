package com.dcam.node;

import android.os.ParcelFileDescriptor;

parcelable DependencyParcel {
    String moduleId = "";
    String version = "";
    String soname = "";
    String sha256 = "";
    @nullable ParcelFileDescriptor libraryFd;
}
