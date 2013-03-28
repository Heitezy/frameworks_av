/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_SERVERS_CAMERA3_OUTPUT_STREAM_H
#define ANDROID_SERVERS_CAMERA3_OUTPUT_STREAM_H

#include <utils/RefBase.h>
#include <gui/Surface.h>

#include "Camera3Stream.h"

namespace android {

namespace camera3 {

/**
 * A class for managing a single stream of output data from the camera device.
 */
class Camera3OutputStream : public Camera3Stream {
  public:
    /**
     * Set up a stream for formats that have 2 dimensions, such as RAW and YUV.
     */
    Camera3OutputStream(int id, sp<ANativeWindow> consumer,
            uint32_t width, uint32_t height, int format);

    /**
     * Set up a stream for formats that have a variable buffer size for the same
     * dimensions, such as compressed JPEG.
     */
    Camera3OutputStream(int id, sp<ANativeWindow> consumer,
            uint32_t width, uint32_t height, size_t maxSize, int format);

    virtual ~Camera3OutputStream();

    /**
     * Camera3Stream interface
     */

    virtual status_t waitUntilIdle(nsecs_t timeout);
    virtual void     dump(int fd, const Vector<String16> &args) const;

    /**
     * Set the transform on the output stream; one of the
     * HAL_TRANSFORM_* / NATIVE_WINDOW_TRANSFORM_* constants.
     */
    status_t         setTransform(int transform);

  private:
    sp<ANativeWindow> mConsumer;
    int               mTransform;
    size_t            mTotalBufferCount;
    size_t            mDequeuedBufferCount;
    Condition         mBufferReturnedSignal;
    uint32_t          mFrameCount;
    nsecs_t           mLastTimestamp;

    // The merged release fence for all returned buffers
    sp<Fence>         mCombinedFence;

    status_t         setTransformLocked(int transform);

    /**
     * Internal Camera3Stream interface
     */
    virtual status_t getBufferLocked(camera3_stream_buffer *buffer);
    virtual status_t returnBufferLocked(
            const camera3_stream_buffer &buffer,
            nsecs_t timestamp);
    virtual bool     hasOutstandingBuffersLocked() const;

    virtual status_t configureQueueLocked();
    virtual size_t   getBufferCountLocked();
    virtual status_t disconnectLocked();

}; // class Camera3OutputStream

} // namespace camera3

} // namespace android

#endif
