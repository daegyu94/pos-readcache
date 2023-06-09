#pragma once

#include <gmock/gmock.h>
#include <list>
#include <string>
#include <vector>

#include "src/allocator/context_manager/segment_ctx/segment_ctx.h"
#include "test/integration-tests/allocator/address/allocator_address_info_tester.h"
#include "test/unit-tests/allocator/context_manager/gc_ctx/gc_ctx_mock.h"
#include "test/unit-tests/allocator/context_manager/rebuild_ctx/rebuild_ctx_mock.h"
#include "test/unit-tests/allocator/context_manager/segment_ctx/segment_ctx_mock.h"
#include "test/unit-tests/telemetry/telemetry_client/telemetry_publisher_mock.h"

using testing::NiceMock;

namespace pos
{
class EventScheduler;
class SegmentCtxTester : public SegmentCtx
{
public:
    SegmentCtxTester(void)
    {
        addressInfo = new NiceMock<AddressInfoTester>;
        gcContext = new NiceMock<MockGcCtx>;
        rebuildContext = new NiceMock<MockRebuildCtx>;
        tp = new NiceMock<MockTelemetryPublisher>;

        ON_CALL(*addressInfo, GetArrayId).WillByDefault(Return(arrayId));

        realCtx = new SegmentCtx(tp, rebuildContext, addressInfo, gcContext, nullptr);
        MakeStubs();
    };

    virtual ~SegmentCtxTester(void)
    {
        delete realCtx;
        delete rebuildContext;
        delete gcContext;
        delete addressInfo;
        delete tp;
    };

    MOCK_METHOD(void, Init, (EventScheduler* eventScheduler_), (override));
    MOCK_METHOD(void, Dispose, (), (override));
    MOCK_METHOD(SegmentId, AllocateFreeSegment, (), (override));
    MOCK_METHOD(uint64_t, GetNumOfFreeSegment, (), (override));
    MOCK_METHOD(int, GetAllocatedSegmentCount, (), (override));
    MOCK_METHOD(void, ValidateBlks, (VirtualBlks blks), (override));
    MOCK_METHOD(bool, InvalidateBlks, (VirtualBlks blks, bool allowVictimSegRelease), (override));
    MOCK_METHOD(int, GetValidBlockCount, (SegmentId segId), (override));
    MOCK_METHOD(int, GetOccupiedStripeCount, (SegmentId segId), (override));
    MOCK_METHOD(bool, UpdateOccupiedStripeCount, (StripeId lsid), (override));
    MOCK_METHOD(SegmentInfoData*, GetSegmentInfoDataArray, (), (override));

    inline uint32_t GetNumOfSegment(void) { return addressInfo->GetnumUserAreaSegments(); }
    inline uint32_t GetNumOfBlksPerStripe(void) { return addressInfo->GetblksPerStripe(); }
    inline uint32_t GetStripesPerSegment(void) { return addressInfo->GetstripesPerSegment(); }

protected:
    NiceMock<AddressInfoTester>* addressInfo;
    NiceMock<MockGcCtx>* gcContext;
    NiceMock<MockRebuildCtx>* rebuildContext;
    NiceMock<MockTelemetryPublisher>* tp;

private:
    virtual void MakeStubs(void)
    {
        InitStubs();
        DisposeStub();
        AllocateFreeSegmentStub();
        GetNumOfFreeSegmentStub();
        GetAllocatedSegmentCountStub();
        ValidateBlksStub();
        GetValidBlockCountStub();
        GetOccupiedStripeCountStub();
        UpdateOccupiedStripeCountStub();
        GetSegmentInfosStub();
    };

    virtual void DisposeStub(void)
    {
        ON_CALL(*this, Dispose).WillByDefault([this]()
        {
            realCtx->Dispose();
        });
    };

    virtual void AllocateFreeSegmentStub(void)
    {
        ON_CALL(*this, AllocateFreeSegment).WillByDefault([this]()
        {
            SegmentId segId = realCtx->AllocateFreeSegment();
            return segId;
        });
    }

    virtual void GetNumOfFreeSegmentStub(void)
    {
        ON_CALL(*this, GetNumOfFreeSegment).WillByDefault([this]()
        {
            uint64_t count = realCtx->GetNumOfFreeSegment();
            return count;
        });
    };

    virtual void InitStubs(void)
    {
        ON_CALL(*this, Init).WillByDefault([this](EventScheduler* eventScheduler_)
        {
            realCtx->Init(eventScheduler);
        });
    };

    virtual void GetAllocatedSegmentCountStub(void)
    {
        ON_CALL(*this, GetAllocatedSegmentCount).WillByDefault([this]()
        {
            int count = realCtx->GetAllocatedSegmentCount();
            return count;
        });
    };

    virtual void ValidateBlksStub(void)
    {
        ON_CALL(*this, ValidateBlks).WillByDefault([this](VirtualBlks blks)
        {
            realCtx->ValidateBlks(blks);
        });
    };

    virtual void GetValidBlockCountStub(void)
    {
        ON_CALL(*this, GetValidBlockCount).WillByDefault([this](SegmentId segId)
        {
            uint32_t count = realCtx->GetValidBlockCount(segId);
            return count;
        });
    };

    virtual void GetOccupiedStripeCountStub(void)
    {
        ON_CALL(*this, GetOccupiedStripeCount).WillByDefault([this](SegmentId segId)
        {
            int count = realCtx->GetOccupiedStripeCount(segId);
            return count;
        });
    };

    virtual void UpdateOccupiedStripeCountStub(void)
    {
        ON_CALL(*this, UpdateOccupiedStripeCount).WillByDefault([this](StripeId lsid)
        {
            bool ret = realCtx->UpdateOccupiedStripeCount(lsid);
            return ret;
        });
    };

    virtual void GetSegmentInfosStub(void)
    {
        ON_CALL(*this, GetSegmentInfoDataArray).WillByDefault([this]()
        {
            return realCtx->GetSegmentInfoDataArray();
        });
    };

    SegmentCtx* realCtx;

    const uint32_t maxValidBlockCount = 128;
    const uint32_t maxOccupiedStripeCount = 128;
    const uint32_t arrayId = 0;
};

class SegmentCtxTesterExample : public SegmentCtxTester
{
public:
    SegmentCtxTesterExample(void)
    {
        SegmentCtxTester();

        // overwrite previous test configuration with new mock function.
        GetNumOfFreeSegmentStub();
    };
    virtual ~SegmentCtxTesterExample(void) {};

    // re-define function of Tester class.
    virtual void GetNumOfFreeSegmentStub() override
    {
        ON_CALL(*this, GetNumOfFreeSegment).WillByDefault([this]()
        {
            return 100;
        });
    };
};
}
