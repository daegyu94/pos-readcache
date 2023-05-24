#pragma once

#include "src/lib/block_alignment.h"
#include "src/network/nvmf_target.h"
#include "src/io/general_io/translator.h"
#include "src/io/general_io/rba_state_service.h"

namespace pos {
struct PrefetchMeta {
    int arrayId;
    uint32_t volumeId;
    uint64_t rba;

    PrefetchMeta(std::string s, uint32_t n, uint64_t r) {
        arrayId = NvmfTargetSingleton::Instance()->GetSubsystemArrayID(s);
        volumeId = n;
        rba = r;  /* always 4KB aligned */
    }
};
using PrefetchMetaSmartPtr = std::shared_ptr<PrefetchMeta>;

struct PrefetchIOConfig {
    PrefetchMetaSmartPtr meta;
    
    PhysicalBlkAddr pba;
    uint64_t lba;       /* device lba */
    uintptr_t dst;      /* destination of cached pool */

    PrefetchIOConfig(PrefetchMetaSmartPtr m, uintptr_t d) : meta(m), dst(d)
    {
        BlockAlignment blockAlignment{meta->rba, BLOCK_SIZE};
        Translator translator{meta->volumeId, blockAlignment.GetHeadBlock(),
            blockAlignment.GetBlockCount(), meta->arrayId, true};

        pba = translator.GetPba();
        lba = blockAlignment.AlignHeadLba(0, pba.lba);
    }
};
} // namespace pos
