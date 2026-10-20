#include <ESPressio_Platform.hpp>
#include <array>
#include <atomic>
#include <cassert>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <functional>
#include <future>
#include <mutex>
#include <thread>
#include <vector>

namespace F=ESPressio::System::CompositionFramework;
namespace P=ESPressio::Platform;
namespace C=P::Concurrency;
struct Signature final { std::uint64_t Sequence,Inverse,Doubled,Check; };
constexpr Signature Make(std::uint64_t n) noexcept { return {n,~n,n*2U,n^0xA5A55A5AF0F00F0FULL}; }
constexpr bool Coherent(const Signature& s) noexcept {
    return s.Inverse==~s.Sequence && s.Doubled==s.Sequence*2U && s.Check==(s.Sequence^0xA5A55A5AF0F00F0FULL);
}
static_assert(sizeof(Signature)==32U);
using Offers=F::Offers<F::Offer<C::AtomicWord32,F::PropertyValue<C::LockFree,true>,
    F::PropertyValue<C::AtomicWordStorageBytes,4U>>>;

// Explicit abstract atomic-memory MODEL, not a native provider or real C++ thread execution.
// Stores have per-object modification order; acquires import a release's causal lower bounds.
// A reader may retain an old generation only when read/read and happens-before coherence allow it.
// The scheduler runs actual Snapshot::Publish bodies in a distinct logical writer actor.
namespace VisibilityModel {
    constexpr std::size_t MaxWords=64U;
    using Frontier=std::array<std::size_t,MaxWords>;
    struct Event final { std::uint32_t Value;bool Release;Frontier Imports; };
    inline std::vector<std::vector<Event>> History;
    inline std::array<Frontier,2U> Seen{};
    inline unsigned Actor=0U;
    inline std::size_t LowSlot=0U,InitialLow=0U,DeniedStale=0U;
    inline bool Fired=false;
    inline std::function<void()> ScheduleWriter;
    inline void Reset() {
        History.clear();Seen={};Actor=0U;LowSlot=InitialLow=DeniedStale=0U;Fired=false;ScheduleWriter={};
    }
    inline void Store(std::uint32_t slot,std::uint32_t value,bool release) {
        Seen[Actor][slot]=History[slot].size();
        History[slot].push_back({value,release,Seen[Actor]});
    }
    inline std::uint32_t Load(std::uint32_t slot,bool acquire) {
        auto chosen=History[slot].size()-1U;
        if(Actor==1U && slot==LowSlot) {
            chosen=InitialLow; // Try the older value only if this actor is still allowed to see it.
            if(chosen<Seen[Actor][slot]) {chosen=History[slot].size()-1U;++DeniedStale;}
        }
        assert(chosen>=Seen[Actor][slot]);
        const auto event=History[slot][chosen];
        Seen[Actor][slot]=chosen;
        if(acquire && event.Release) {
            for(std::size_t i=0U;i<MaxWords;++i)
                if(event.Imports[i]>Seen[Actor][i])Seen[Actor][i]=event.Imports[i];
        }
        if(Actor==1U && slot==0U && !Fired) {
            Fired=true;Actor=0U;ScheduleWriter();Actor=1U;
        }
        return event.Value;
    }
    class Provider final:public F::Provider<P::Domain,Offers> {
    public:
        class Word final {
            std::uint32_t _slot;
        public:
            Word() noexcept:_slot(static_cast<std::uint32_t>(History.size())) {
                assert(_slot<MaxWords);History.push_back({Event{0U,false,{}}});
            }
            Word(const Word&)=delete;Word& operator=(const Word&)=delete;
            Word(Word&&)=delete;Word& operator=(Word&&)=delete;
            std::uint32_t LoadRelaxed() const noexcept{return Load(_slot,false);}
            std::uint32_t LoadAcquire() const noexcept{return Load(_slot,true);}
            void StoreRelaxed(std::uint32_t v) noexcept{Store(_slot,v,false);}
            void StoreRelease(std::uint32_t v) noexcept{Store(_slot,v,true);}
            bool CompareExchangeAcqRel(std::uint32_t& expected,std::uint32_t desired) noexcept {
                const auto value=Load(_slot,true);
                if(value!=expected){expected=value;return false;}
                Store(_slot,desired,true);return true;
            }
        };
    };
    inline bool Check() {
        Reset();
        C::ConcurrentSnapshot<Signature,Provider> snapshot(Make(0U));
        static_assert(sizeof(Provider::Word)==4U);
        static_assert(sizeof(snapshot)==72U && decltype(snapshot)::AtomicStorageBytes==72U);
        assert(History.size()==18U);
        LowSlot=History.size()-1U;InitialLow=History[LowSlot].size()-1U;
        for(std::size_t i=0U;i<History.size();++i)Seen[1U][i]=History[i].size()-1U;
        ScheduleWriter=[&snapshot](){snapshot.Publish(Make(1U));snapshot.Publish(Make(2U));};
        Actor=1U;const auto result=snapshot.Read();
        const bool coherent=Coherent(result);
        std::printf("H471_ALLOWED_VISIBILITY_MODEL|coherent=%u|stale_generation_reads_denied_by_acquire=%zu|hardware=0\n",
            static_cast<unsigned>(coherent),DeniedStale);
        return coherent && DeniedStale>0U;
    }
}

// Concrete host std::atomic seam. The hook pauses a thread AFTER its store,
// modeling preemption; reader methods contain no hook, mutex or gate.
namespace Host {
    class Word;
    inline std::vector<Word*> Registry;
    inline std::function<void(const Word*,std::uint32_t)> AfterStore;
    class Word final {
        std::atomic<std::uint32_t> _value{0U};
    public:
        Word() noexcept {Registry.push_back(this);}
        Word(const Word&)=delete;Word& operator=(const Word&)=delete;
        Word(Word&&)=delete;Word& operator=(Word&&)=delete;
        std::uint32_t LoadRelaxed() const noexcept{return _value.load(std::memory_order_relaxed);}
        std::uint32_t LoadAcquire() const noexcept{return _value.load(std::memory_order_acquire);}
        void StoreRelaxed(std::uint32_t v) noexcept {
            _value.store(v,std::memory_order_relaxed);if(AfterStore)AfterStore(this,v);
        }
        void StoreRelease(std::uint32_t v) noexcept {
            _value.store(v,std::memory_order_release);if(AfterStore)AfterStore(this,v);
        }
        bool CompareExchangeAcqRel(std::uint32_t& expected,std::uint32_t desired) noexcept {
            return _value.compare_exchange_strong(expected,desired,std::memory_order_acq_rel,std::memory_order_acquire);
        }
    };
    class Provider final:public F::Provider<P::Domain,Offers> {public:using Word=Host::Word;};
    using Snapshot=C::ConcurrentSnapshot<Signature,Provider>;
    static_assert(std::atomic<std::uint32_t>::is_always_lock_free && sizeof(Word)==4U && sizeof(Snapshot)==72U);
    inline void Reset(){AfterStore={};Registry.clear();}
    inline void ForceGeneration(std::uint32_t high,std::uint32_t low) {
        assert(Registry.size()==18U);
        Registry[16U]->StoreRelaxed(high);Registry[17U]->StoreRelaxed(low);
    }
    struct Gate final {
        std::mutex Mutex;std::condition_variable Changed;bool Reached=false,Released=false;
        void Pause() {
            std::unique_lock lock(Mutex);Reached=true;Changed.notify_all();Changed.wait(lock,[this](){return Released;});
        }
        bool Await() {
            std::unique_lock lock(Mutex);
            return Changed.wait_for(lock,std::chrono::seconds(2),[this](){return Reached;});
        }
        void Release() {std::lock_guard lock(Mutex);Released=true;Changed.notify_all();}
    };
    inline void PausedRollover(bool afterLowZero,std::uint32_t epoch=7U) {
        Reset();Snapshot snapshot(Make(17U));ForceGeneration(epoch,UINT32_MAX);
        Gate gate;const auto* high=Registry[16U];const auto* low=Registry[17U];
        AfterStore=[&](const Word* word,std::uint32_t value) {
            if((!afterLowZero && word==high && value==(epoch|0x80000000U)) ||
               (afterLowZero && word==low && value==0U))gate.Pause();
        };
        std::thread writer([&](){snapshot.Publish(Make(18U));});
        const bool reached=gate.Await();
        auto reader=std::async(std::launch::async,[&](){return snapshot.Read();});
        const bool progressed=reader.wait_for(std::chrono::seconds(1))==std::future_status::ready;
        // Always release/join, including a failing old implementation, so the fixture cannot strand a thread.
        gate.Release();writer.join();const auto read=reader.get();AfterStore={};
        assert(reached && progressed && Coherent(read) && read.Sequence==17U);
        const auto final=snapshot.Read();assert(Coherent(final) && final.Sequence==18U);
        assert(high->LoadAcquire()==((epoch+1U)&0x7FFFFFFFU) && low->LoadAcquire()==0U);
        std::printf("H471_ROLLOVER_PAUSED_WRITER_READER_PROGRESS_PASS|after_low_zero=%u|hardware=0\n",static_cast<unsigned>(afterLowZero));
    }
    inline void ConcurrentRollover() {
        Reset();Snapshot snapshot(Make(0U));ForceGeneration(9U,UINT32_MAX-100U);
        std::atomic<bool> stop{false},failed{false};std::vector<std::thread> readers;
        for(unsigned i=0U;i<8U;++i)readers.emplace_back([&](){
            while(!stop.load(std::memory_order_acquire))if(!Coherent(snapshot.Read()))failed.store(true,std::memory_order_release);
        });
        for(std::uint64_t n=1U;n<=100000U;++n)snapshot.Publish(Make(n));
        stop.store(true,std::memory_order_release);for(auto& reader:readers)reader.join();
        assert(!failed.load(std::memory_order_acquire));
        assert(Registry[16U]->LoadAcquire()==10U);
        assert(Coherent(snapshot.Read()) && snapshot.Read().Sequence==100000U);
        std::puts("H471_REAL_HOST_100K_EIGHT_READER_LOW_WORD_ROLLOVER_SIGNATURE_PASS|hardware=0");
    }
}
int main(int argc,char** argv) {
    if(!VisibilityModel::Check())return 1;
    if(argc==2 && std::strcmp(argv[1],"--model-only")==0)return 0;
    Host::PausedRollover(false);Host::PausedRollover(true);
    Host::PausedRollover(false,0x7FFFFFFFU);Host::PausedRollover(true,0x7FFFFFFFU);Host::ConcurrentRollover();
    std::puts("H471_SAME_72_BYTE_STORAGE_BUFFER_REUSE_AND_NONBLOCKING_ROLLOVER_PASS|hardware=0|authority=0");
}
