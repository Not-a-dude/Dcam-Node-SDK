#pragma once

#if !defined(__aarch64__)
#error "Dcam Node SDK supports only AArch64 (Android arm64-v8a)."
#endif

#if !defined(__ANDROID__)
#error "Dcam Node SDK supports only Android targets."
#endif

#include <stddef.h>
#include <stdint.h>
#include <android/hardware_buffer.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DCAM_NODE_EXPORT __attribute__((visibility("default")))

#define DCAM_NODE_ABI_MAJOR 1u
#define DCAM_NODE_ABI_MINOR 0u
#define DCAM_NODE_ENTRYPOINT "dcam_node_get_api"
#define DCAM_PREVIEW_SURFACE ((DcamSurfaceHandle)1u)
#define DCAM_FRAME_FLAG_PREVIEW_PRESENTED (1u << 0)

#define DCAM_STRUCT_HEADER uint32_t struct_size

typedef int32_t DcamStatus;
typedef uint64_t DcamNodeHandle;
typedef uint64_t DcamBufferHandle;
typedef uint64_t DcamSurfaceHandle;

typedef enum DcamStatusCode {
    DCAM_OK = 0,
    DCAM_ERROR_INVALID_ARGUMENT = -1,
    DCAM_ERROR_UNSUPPORTED = -2,
    DCAM_ERROR_OUT_OF_MEMORY = -3,
    DCAM_ERROR_BAD_STATE = -4,
    DCAM_ERROR_PROCESSING = -5,
    DCAM_ERROR_ABI_MISMATCH = -6,
    DCAM_ERROR_NOT_FOUND = -7,
    DCAM_ERROR_BUSY = -8,
    DCAM_ERROR_PERMISSION = -9,
    DCAM_ERROR_DEPENDENCY = -10,
    DCAM_ERROR_IO = -11,
} DcamStatusCode;

typedef enum DcamFrameDisposition {
    DCAM_FRAME_RELEASE = 0,
    DCAM_FRAME_RETAIN_ZSL = 1,
} DcamFrameDisposition;

typedef enum DcamPortType {
    DCAM_PORT_FRAME = 1,
    DCAM_PORT_METADATA = 2,
    DCAM_PORT_AUDIO = 3,
    DCAM_PORT_ENCODED_PACKET = 4,
    DCAM_PORT_SURFACE = 5,
    DCAM_PORT_REQUEST_STATE = 6,
    DCAM_PORT_OUTPUT_FD = 7,
} DcamPortType;

typedef enum DcamWireType {
    DCAM_WIRE_BYTE = 1,
    DCAM_WIRE_INT32 = 2,
    DCAM_WIRE_INT64 = 3,
    DCAM_WIRE_FLOAT32 = 4,
    DCAM_WIRE_FLOAT64 = 5,
    DCAM_WIRE_RATIONAL = 6,
    DCAM_WIRE_RECT = 7,
    DCAM_WIRE_SIZE = 8,
    DCAM_WIRE_RANGE_INT32 = 9,
    DCAM_WIRE_RANGE_INT64 = 10,
    DCAM_WIRE_METERING_RECTANGLE = 11,
    DCAM_WIRE_STRING_UTF8 = 12,
    DCAM_WIRE_OPAQUE_UTF8 = 13,
    DCAM_WIRE_BOOL = 14,
    DCAM_WIRE_LENS_SHADING_MAP = 15,
} DcamWireType;

typedef enum DcamRequestScope {
    DCAM_REQUEST_REPEATING = 1,
    DCAM_REQUEST_STILL = 2,
    DCAM_REQUEST_RECORD = 3,
} DcamRequestScope;

typedef enum DcamRequestOperation {
    DCAM_REQUEST_SET = 1,
    DCAM_REQUEST_REMOVE = 2,
} DcamRequestOperation;

typedef enum DcamRequestLifetime {
    DCAM_REQUEST_PERSISTENT = 1,
    DCAM_REQUEST_ONE_SHOT = 2,
} DcamRequestLifetime;

typedef enum DcamSingleCaptureSource {
    DCAM_CAPTURE_FRESH = 1,
    DCAM_CAPTURE_ZSL_FRAME = 2,
} DcamSingleCaptureSource;

typedef enum DcamCaptureEventType {
    DCAM_CAPTURE_SHUTTER_PRESSED = 1,
    DCAM_CAPTURE_BEGIN = 2,
    DCAM_CAPTURE_END = 3,
    DCAM_CAPTURE_FAILURE = 4,
    DCAM_CAPTURE_ZSL_EVICTED = 5,
} DcamCaptureEventType;

typedef struct DcamStringView {
    const char* data;
    uint32_t size;
} DcamStringView;

typedef struct DcamBytesView {
    const uint8_t* data;
    uint32_t size;
} DcamBytesView;

typedef struct DcamBufferDesc {
    DCAM_STRUCT_HEADER;
    uint32_t width;
    uint32_t height;
    uint32_t layers;
    uint32_t format;
    uint64_t usage;
    int32_t data_space;
    uint32_t reserved[7];
} DcamBufferDesc;

typedef struct DcamMetadataEntry {
    DCAM_STRUCT_HEADER;
    uint32_t key_id;
    uint32_t wire_type;
    uint32_t element_count;
    uint32_t flags;
    DcamStringView name;
    DcamBytesView value;
    uint64_t reserved[4];
} DcamMetadataEntry;

typedef struct DcamMetadata {
    DCAM_STRUCT_HEADER;
    uint32_t wire_version;
    uint32_t entry_count;
    const DcamMetadataEntry* entries;
    uint64_t reserved[5];
} DcamMetadata;

/*
 * hardware_buffer and metadata are borrowed until process() returns.
 * acquire_fence_fd ownership transfers to the node. The node must close it or
 * return it as release_fence_fd. -1 means that the buffer is ready.
 * Pixel storage is never copied by the ABI transport.
 */
typedef struct DcamFrame {
    DCAM_STRUCT_HEADER;
    uint64_t session_id;
    uint32_t stream_id;
    uint32_t flags;
    uint64_t frame_id;
    uint64_t timestamp_ns;
    AHardwareBuffer* hardware_buffer;
    DcamBufferDesc buffer_desc;
    int32_t crop_left;
    int32_t crop_top;
    int32_t crop_right;
    int32_t crop_bottom;
    int32_t acquire_fence_fd;
    uint32_t reserved0;
    uint64_t request_revision;
    const DcamMetadata* metadata;
    uint64_t reserved[5];
} DcamFrame;

/* release_fence_fd ownership transfers from the node to the host. */
typedef struct DcamFrameResult {
    DCAM_STRUCT_HEADER;
    int32_t disposition;
    int32_t release_fence_fd;
    uint32_t flags;
    uint64_t reserved[7];
} DcamFrameResult;

typedef struct DcamPortDescriptor {
    DCAM_STRUCT_HEADER;
    uint32_t port_id;
    uint32_t port_type;
    uint32_t flags;
    DcamStringView name;
    DcamStringView schema_json;
    uint64_t reserved[4];
} DcamPortDescriptor;

typedef struct DcamNodeDescriptor {
    DCAM_STRUCT_HEADER;
    DcamStringView module_id;
    DcamStringView module_version;
    DcamStringView node_type_id;
    uint64_t capabilities;
    uint32_t input_port_count;
    uint32_t output_port_count;
    const DcamPortDescriptor* input_ports;
    const DcamPortDescriptor* output_ports;
    uint64_t reserved[6];
} DcamNodeDescriptor;

typedef struct DcamRequestStateEntry {
    DCAM_STRUCT_HEADER;
    uint32_t key_id;
    uint32_t wire_type;
    uint32_t operation;
    uint32_t element_count;
    DcamStringView name;
    DcamBytesView value;
    uint64_t reserved[4];
} DcamRequestStateEntry;

typedef struct DcamRequestStateTransaction {
    DCAM_STRUCT_HEADER;
    uint32_t scope;
    int32_t template_id;
    uint32_t lifetime;
    uint32_t target_count;
    uint64_t revision;
    uint64_t base_revision;
    const uint32_t* target_stream_ids;
    uint32_t entry_count;
    uint32_t reserved0;
    const DcamRequestStateEntry* entries;
    uint64_t reserved[5];
} DcamRequestStateTransaction;

typedef struct DcamCaptureEvent {
    DCAM_STRUCT_HEADER;
    uint32_t event_type;
    int32_t status;
    uint64_t capture_id;
    uint64_t frame_id;
    DcamStringView message;
    uint64_t reserved[5];
} DcamCaptureEvent;

typedef struct DcamRequestStateResult {
    DCAM_STRUCT_HEADER;
    uint64_t revision;
    uint32_t applied;
    int32_t error_code;
    uint32_t key_id;
    uint32_t reserved0;
    DcamStringView key_name;
    DcamStringView message;
    uint64_t reserved[4];
} DcamRequestStateResult;

typedef struct DcamHostApi {
    DCAM_STRUCT_HEADER;
    uint32_t abi_major;
    uint32_t abi_minor;
    void* context;

    void (*log)(void* context, int32_t priority, DcamStringView tag,
                DcamStringView message);
    DcamStatus (*retain_buffer)(void* context, AHardwareBuffer* buffer,
                                DcamBufferHandle* out_handle);
    void (*release_buffer)(void* context, DcamBufferHandle handle,
                           int32_t release_fence_fd);
    DcamStatus (*allocate_buffer)(void* context, const DcamBufferDesc* desc,
                                  DcamBufferHandle* out_handle,
                                  AHardwareBuffer** out_buffer);
    DcamStatus (*present_buffer)(void* context, DcamSurfaceHandle surface,
                                 AHardwareBuffer* buffer,
                                 int32_t acquire_fence_fd);
    DcamStatus (*submit_request_state)(
            void* context, const DcamRequestStateTransaction* transaction);
    DcamStatus (*request_single_capture)(void* context, uint32_t source,
                                         uint64_t zsl_frame_id);
    DcamStatus (*release_zsl_frame)(void* context, uint64_t frame_id,
                                    int32_t release_fence_fd);
    DcamStatus (*duplicate_output_fd)(void* context, uint64_t capture_id,
                                      int32_t* out_fd);
    DcamStatus (*complete_output)(void* context, uint64_t capture_id,
                                  DcamStringView mime_type, uint64_t size,
                                  DcamStatus status);
    uint64_t reserved[7];
} DcamHostApi;

typedef struct DcamNodeApi {
    DCAM_STRUCT_HEADER;
    uint32_t abi_major;
    uint32_t abi_minor;

    DcamStatus (*get_node_count)(uint32_t* out_count);
    DcamStatus (*get_node_descriptor)(uint32_t index,
                                      DcamNodeDescriptor* out_descriptor);
    DcamStatus (*create)(DcamStringView node_type_id,
                         DcamStringView instance_id,
                         DcamStringView config_json,
                         DcamNodeHandle* out_handle);
    DcamStatus (*configure)(DcamNodeHandle handle, DcamStringView config_json);
    DcamStatus (*process)(DcamNodeHandle handle, const DcamFrame* frame,
                          DcamFrameResult* out_result);
    DcamStatus (*flush)(DcamNodeHandle handle);
    DcamStatus (*update_parameters)(DcamNodeHandle handle, uint64_t revision,
                                    DcamStringView json_patch);
    DcamStatus (*on_capture_event)(DcamNodeHandle handle,
                                   const DcamCaptureEvent* event);
    DcamStatus (*on_request_state_result)(
            DcamNodeHandle handle, const DcamRequestStateResult* result);
    void (*destroy)(DcamNodeHandle handle);
    uint64_t reserved[9];
} DcamNodeApi;

typedef DcamStatus (*DcamNodeGetApiFn)(uint32_t requested_abi_major,
                                       const DcamHostApi* host_api,
                                       DcamNodeApi* out_node_api);

DCAM_NODE_EXPORT DcamStatus dcam_node_get_api(uint32_t requested_abi_major,
                                               const DcamHostApi* host_api,
                                               DcamNodeApi* out_node_api);

#ifdef __cplusplus
}
#endif