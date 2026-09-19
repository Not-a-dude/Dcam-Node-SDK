package com.dcam.node;

import android.os.ParcelFileDescriptor;

import com.dcam.node.ArtifactRequestParcel;
import com.dcam.node.ArtifactResult;
import com.dcam.node.ArtifactTargetParcel;
import com.dcam.node.CaptureEvent;
import com.dcam.node.FrameRelease;
import com.dcam.node.OutputResult;
import com.dcam.node.RequestStateResult;
import com.dcam.node.RequestStateTransaction;
import com.dcam.node.RuntimeEvent;
import com.dcam.node.SingleCaptureRequest;

interface IDcamNodeHost {
    oneway void onFrameReleased(in FrameRelease release);
    oneway void onRequestStateTransaction(in RequestStateTransaction transaction);
    oneway void onSingleCaptureRequest(in SingleCaptureRequest request);
    oneway void onReleaseZslFrame(long sessionId, long frameId, in @nullable ParcelFileDescriptor releaseFence);
    oneway void onReleasePreviewFrame(long sessionId, long frameId, in @nullable ParcelFileDescriptor releaseFence);
    oneway void onRequestStateResult(in RequestStateResult result);
    oneway void onCaptureEvent(in CaptureEvent event);
    oneway void onOutputResult(in OutputResult result);
    oneway void onRuntimeEvent(in RuntimeEvent event);
    ArtifactTargetParcel openArtifact(in ArtifactRequestParcel request);
    oneway void onArtifactResult(in ArtifactResult result);
}