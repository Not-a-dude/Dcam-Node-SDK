#include "dcam_node_abi.h"

#include <stdlib.h>
#include <string.h>

#include <arm_neon.h>

typedef struct StockNode {
    uint64_t processed_frames;
    uint32_t config_checksum;
    DcamHostApi host;
} StockNode;

static const DcamHostApi* g_host;

static const DcamPortDescriptor kInputs[] = {
    {
        .struct_size = sizeof(DcamPortDescriptor),
        .port_id = 1,
        .port_type = DCAM_PORT_FRAME,
        .flags = 0,
        .name = {"frame", 5},
        .schema_json = {NULL, 0},
    },
};

static const DcamPortDescriptor kOutputs[] = {
    {
        .struct_size = sizeof(DcamPortDescriptor),
        .port_id = 2,
        .port_type = DCAM_PORT_FRAME,
        .flags = 0,
        .name = {"frame", 5},
        .schema_json = {NULL, 0},
    },
};

static DcamStringView string_view(const char* value) {
    DcamStringView view = {value, (uint32_t)strlen(value)};
    return view;
}

static DcamStatus get_node_count(uint32_t* out_count) {
    if (out_count == NULL) return DCAM_ERROR_INVALID_ARGUMENT;
    *out_count = 1;
    return DCAM_OK;
}

static DcamStatus get_node_descriptor(uint32_t index, DcamNodeDescriptor* out) {
    if (index != 0) return DCAM_ERROR_NOT_FOUND;
    if (out == NULL || out->struct_size < sizeof(DcamNodeDescriptor)) {
        return DCAM_ERROR_INVALID_ARGUMENT;
    }
    out->module_id = string_view("org.dcam.stock");
    out->module_version = string_view("1.0.0");
    out->node_type_id = string_view("org.dcam.stock.pass_through");
    out->capabilities = 0;
    out->input_port_count = 1;
    out->output_port_count = 1;
    out->input_ports = kInputs;
    out->output_ports = kOutputs;
    return DCAM_OK;
}

static DcamStatus create_node(DcamStringView node_type_id,
                              DcamStringView instance_id,
                              DcamStringView config,
                              DcamNodeHandle* out_handle) {
    (void)instance_id;
    (void)config;
    if (out_handle == NULL || node_type_id.data == NULL) {
        return DCAM_ERROR_INVALID_ARGUMENT;
    }
    const char* expected = "org.dcam.stock.pass_through";
    if (node_type_id.size != strlen(expected) ||
        memcmp(node_type_id.data, expected, node_type_id.size) != 0) {
        return DCAM_ERROR_NOT_FOUND;
    }
    StockNode* node = (StockNode*)calloc(1, sizeof(StockNode));
    if (node == NULL) return DCAM_ERROR_OUT_OF_MEMORY;
    node->host = *g_host;
    uint32_t offset = 0;
    uint32_t checksum = 0;
    while (offset + 16 <= config.size) {
        const uint8x16_t bytes = vld1q_u8((const uint8_t*)config.data + offset);
        checksum += vaddvq_u8(bytes);
        offset += 16;
    }
    node->config_checksum = checksum;
    *out_handle = (DcamNodeHandle)(uintptr_t)node;
    return DCAM_OK;
}

static DcamStatus configure_node(DcamNodeHandle handle, DcamStringView config) {
    (void)config;
    return handle == 0 ? DCAM_ERROR_INVALID_ARGUMENT : DCAM_OK;
}

static DcamStatus process_frame(DcamNodeHandle handle, const DcamFrame* frame,
                                DcamFrameResult* out) {
    if (handle == 0 || frame == NULL || out == NULL ||
        frame->struct_size < sizeof(DcamFrame) ||
        out->struct_size < sizeof(DcamFrameResult) ||
        frame->hardware_buffer == NULL) {
        return DCAM_ERROR_INVALID_ARGUMENT;
    }
    StockNode* node = (StockNode*)(uintptr_t)handle;
    node->processed_frames++;
    if (node->host.present_buffer != NULL) {
        const DcamStatus present = node->host.present_buffer(
                node->host.context,
                DCAM_PREVIEW_SURFACE,
                frame->hardware_buffer,
                frame->acquire_fence_fd);
        if (present != DCAM_OK) return present;
    }
    out->disposition = DCAM_FRAME_RELEASE;
    out->release_fence_fd = -1;
    out->flags = 0;
    return DCAM_OK;
}

static DcamStatus flush_node(DcamNodeHandle handle) {
    return handle == 0 ? DCAM_ERROR_INVALID_ARGUMENT : DCAM_OK;
}

static DcamStatus update_parameters(DcamNodeHandle handle, uint64_t revision,
                                    DcamStringView patch) {
    (void)revision;
    (void)patch;
    return handle == 0 ? DCAM_ERROR_INVALID_ARGUMENT : DCAM_OK;
}

static DcamStatus on_capture_event(DcamNodeHandle handle,
                                   const DcamCaptureEvent* event) {
    if (handle == 0 || event == NULL ||
        event->struct_size < sizeof(DcamCaptureEvent)) {
        return DCAM_ERROR_INVALID_ARGUMENT;
    }
    if (event->event_type == DCAM_CAPTURE_SHUTTER_PRESSED &&
        ((StockNode*)(uintptr_t)handle)->host.request_single_capture != NULL) {
        StockNode* node = (StockNode*)(uintptr_t)handle;
        return node->host.request_single_capture(
                node->host.context, DCAM_CAPTURE_FRESH, 0);
    }
    return DCAM_OK;
}

static DcamStatus on_request_state_result(
        DcamNodeHandle handle, const DcamRequestStateResult* result) {
    if (handle == 0 || result == NULL ||
        result->struct_size < sizeof(DcamRequestStateResult)) {
        return DCAM_ERROR_INVALID_ARGUMENT;
    }
    return DCAM_OK;
}

static void destroy_node(DcamNodeHandle handle) {
    free((void*)(uintptr_t)handle);
}

DCAM_NODE_EXPORT DcamStatus dcam_node_get_api(uint32_t requested_abi_major,
                                               const DcamHostApi* host_api,
                                               DcamNodeApi* out_api) {
    if (requested_abi_major != DCAM_NODE_ABI_MAJOR || host_api == NULL ||
        host_api->abi_major != DCAM_NODE_ABI_MAJOR || out_api == NULL ||
        out_api->struct_size < sizeof(DcamNodeApi)) {
        return DCAM_ERROR_ABI_MISMATCH;
    }
    g_host = host_api;
    (void)g_host;
    out_api->abi_major = DCAM_NODE_ABI_MAJOR;
    out_api->abi_minor = DCAM_NODE_ABI_MINOR;
    out_api->get_node_count = get_node_count;
    out_api->get_node_descriptor = get_node_descriptor;
    out_api->create = create_node;
    out_api->configure = configure_node;
    out_api->process = process_frame;
    out_api->flush = flush_node;
    out_api->update_parameters = update_parameters;
    out_api->on_capture_event = on_capture_event;
    out_api->on_request_state_result = on_request_state_result;
    out_api->destroy = destroy_node;
    return DCAM_OK;
}
