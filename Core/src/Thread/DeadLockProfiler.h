#pragma once

namespace Engine {
    class DeadLockProfiler {
    public:
        static void Init();
        
        void PushLock(char const* name);
        void PopLock(char const* name);
        void CheckCycle();
    private:
        void DFS(int32_t idx);
    private:
        std::unordered_map<std::string, int32_t>  _nameToID;
        std::unordered_map<int32_t, std::string>  _iDtoName;
        std::map<int32_t, std::set<int32_t>>      _lockHistory;

        std::mutex                                _lock;
    private:
        std::vector<int32_t>                      _discoveredOrder;
        int32_t                                   _discoveredCount {};
        std::vector<bool>                         _finished;
        std::vector<int32_t>                      _parent;
    public:
        inline static std::unique_ptr<DeadLockProfiler> GDeadLockProfiler {};
    };
}


