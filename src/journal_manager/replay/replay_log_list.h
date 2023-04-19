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

#include <map>
#include <vector>

#include "src/journal_manager/log/log_event.h"
#include "src/journal_manager/log/log_handler.h"
#include "src/journal_manager/log/log_list.h"
#include "src/journal_manager/replay/replay_log.h"

namespace pos
{
class ReplayLogList : public LogList
{
public:
    ReplayLogList(void);
    virtual ~ReplayLogList(void);

    virtual void Init(int numLogGroups);

    virtual void AddLog(int logGroupId, LogHandlerInterface* log) override;
    virtual void SetLogGroupFooter(int logGroupId, LogGroupFooter footer) override;
    virtual bool IsEmpty(void) override;

    virtual LogGroupFooter GetLogGroupFooter(int logGroupId);

    virtual int EraseReplayLogGroup(int logGroupId, uint32_t seqNum);
    virtual void SetSegInfoFlushed(int logGroupId);

    virtual std::vector<ReplayLog> PopReplayLogGroup(void);
    std::vector<ReplayLog>& GetDeletingLogs(void);

    virtual void PrintLogStatistics(void);

private:
    uint64_t _GetTime(void);
    void _Reset(void);

    using SeqNumGroup = std::map<uint32_t, ReplayLogGroup>; // key: sequence number

    std::vector<SeqNumGroup> logGroups;  // index: log group id
    std::vector<LogGroupFooter> footers; // index: log group id

    std::vector<ReplayLog> deletingLogs;

    uint64_t time;
};
} // namespace pos
