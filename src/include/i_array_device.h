/*
 *   BSD LICENSE
 *   Copyright (c) 2021 Samsung Electronics Corporation
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *     * Neither the name of Samsung Electronics Corporation nor the names of
 *       its contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "src/include/array_device_state.h"
#include "src/array/device/array_device_type.h"
#include "src/include/smart_ptr_type.h"

#include <string>

namespace pos
{

class IArrayDevice
{
public:
// LCOV_EXCL_START
    virtual ~IArrayDevice(void)
    {
    }
// LCOV_EXCL_STOP
    virtual std::string GetName(void) = 0;
    virtual std::string GetSerial(void) = 0;
    virtual uint32_t GetDataIndex(void) = 0;
    virtual uint64_t GetSize(void) = 0;
    virtual ArrayDeviceType GetType(void) = 0;
    virtual ArrayDeviceState GetState(void) = 0;
    virtual UblockSharedPtr GetUblock(void) = 0;
    virtual UBlockDevice* GetUblockPtr(void) = 0;
    virtual void SetState(ArrayDeviceState state) = 0;
    virtual void SetUblock(UblockSharedPtr uBlock) = 0;
};
} // namespace pos
