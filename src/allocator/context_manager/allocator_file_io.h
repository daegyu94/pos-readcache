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

#include <vector>
#include <atomic>

#include "src/allocator/context_manager/i_allocator_file_io_client.h"
#include "src/meta_file_intf/meta_file_intf.h"
#include "src/allocator/context_manager/io_ctx/allocator_io_ctx.h"

namespace pos
{
class AllocatorAddressInfo;
class AllocatorFileIoManager;

class AllocatorFileIo
{
public:
    AllocatorFileIo(void) = default;
    AllocatorFileIo(FileOwner owner, IAllocatorFileIoClient* client_, AllocatorAddressInfo* addrInfo);
    AllocatorFileIo(FileOwner owner, IAllocatorFileIoClient* client_, AllocatorAddressInfo* addrInfo, MetaFileIntf* file);
    virtual ~AllocatorFileIo(void);

    virtual void Init(void);
    virtual void Dispose(void);

    virtual int LoadContext(void);
    virtual int Flush(FnAllocatorCtxIoCompletion clientCallback, ContextSectionBuffer externalBuf = INVALID_CONTEXT_SECTION_BUFFER);

    virtual uint64_t GetStoredVersion(void);
    virtual int GetSectionSize(int section);

    virtual int GetNumOutstandingRead(void);
    virtual int GetNumOutstandingFlush(void);

private:
    int _CreateAndOpenFile(void);
    int _OpenAndLoadFile(void);

    int _Load(void);

    void _LoadCompletedThenCB(AsyncMetaFileIoCtx* ctx);
    void _FlushCompletedThenCB(AsyncMetaFileIoCtx* ctx);

    AllocatorAddressInfo* addrInfo;
    IAllocatorFileIoClient* client;

    FileOwner owner;
    MetaFileIntf* file;

    std::atomic<int> numOutstandingReads;
    std::atomic<int> numOutstandingFlushes;

    bool initialized;
};

} // namespace pos
