package com.dcam.node;

import android.os.ParcelFileDescriptor;
import android.view.Surface;
import com.dcam.node.CaptureEvent;
import com.dcam.node.DependencyParcel;
import com.dcam.node.FrameParcel;
import com.dcam.node.IDcamNodeHost;
import com.dcam.node.OutputTargetParcel;
import com.dcam.node.RequestStateResult;
import com.dcam.node.RuntimeInfo;
import com.dcam.node.SessionConfigParcel;
import com.dcam.node.SessionOpenResult;

interface IDcamNodeRuntime {
    SessionOpenResult openSession(
        in SessionConfigParcel config,
        in ParcelFileDescriptor nodeLibrary,
        IDcamNodeHost host
    );
    int setPreviewSurface(long sessionId, in Surface surface);
    oneway void processFrame(in FrameParcel frame);
    oneway void updateParameters(long sessionId, long revision, String jsonPatch);
    oneway void shutterPressed(long sessionId, long captureId);
    oneway void deliverCaptureEvent(in CaptureEvent event);
    oneway void deliverRequestStateResult(in RequestStateResult result);
    oneway void setOutputTarget(in OutputTargetParcel target);
    oneway void evictZslFrame(long sessionId, long frameId);
    void flushSession(long sessionId);
    void closeSession(long sessionId);
    RuntimeInfo getRuntimeInfo();
    SessionOpenResult openSessionWithDependencies(
        in SessionConfigParcel config,
        in ParcelFileDescriptor nodeLibrary,
        in DependencyParcel[] dependencies,
        IDcamNodeHost host
    );
}
