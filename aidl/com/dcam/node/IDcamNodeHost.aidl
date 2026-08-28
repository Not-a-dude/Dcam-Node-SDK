package com.dcam.node;

import android.os.ParcelFileDescriptor;

import com.dcam.node.CaptureEvent;
import com.dcam.node.FrameRelease;
import com.dcam.node.OutputResult;
import com.dcam.node.RequestStateResult;
import com.dcam.node.RequestStateTransaction;
import com.dcam.node.RuntimeEvent;
import com.dcam.node.SingleCaptureRequest;

oneway interface IDcamNodeHost {
    void onFrameReleased(in FrameRelease release);
    void onRequestStateTransaction(in RequestStateTransaction transaction);
    void onSingleCaptureRequest(in SingleCaptureRequest request);
    void onReleaseZslFrame(long sessionId, long frameId, in @nullable ParcelFileDescriptor releaseFence);
    void onReleasePreviewFrame(long sessionId, long frameId, in @nullable ParcelFileDescriptor releaseFence);
    void onRequestStateResult(in RequestStateResult result);
    void onCaptureEvent(in CaptureEvent event);
    void onOutputResult(in OutputResult result);
    void onRuntimeEvent(in RuntimeEvent event);
}