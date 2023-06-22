#include "pch.h"
#include "DeadLockProfiler.h"

void Engine::DeadLockProfiler::Init() {
    GDeadLockProfiler = MakeUnique<DeadLockProfiler>();
}

void Engine::DeadLockProfiler::PushLock(char const* name) {
    std::lock_guard guard{_lock};

    // Resolving ID
    int32_t lockID {};
    if (_nameToID.contains(name)) {
        lockID = _nameToID[name];
    }
    else {
        lockID = static_cast<int32_t>(_nameToID.size());
        _nameToID[name] = lockID;
        _iDtoName[lockID] = name;
    }

    // If holding locks
    if (LLockStack.empty() == false) {
        // get the latest locked lock
        int32_t const prevID {LLockStack.top()};
        if (lockID != prevID) {
            auto& history {_lockHistory[prevID]};
            // if this node was not visited
            if (!history.contains(lockID)) {
                history.insert(lockID);
                CheckCycle();
            }
        }
    }

    LLockStack.push(lockID);
}

void Engine::DeadLockProfiler::PopLock(char const* name) {
    std::lock_guard guard {_lock};

    if (LLockStack.empty())
        CRASH("MULTIPLE_UNLOCK");

    int32_t lockID {_nameToID[name]};
    if (LLockStack.top() != lockID)
        CRASH("INVALID_UNLOCK");
    
    LLockStack.pop();
}

void Engine::DeadLockProfiler::CheckCycle() {
    int32_t const lockCount {static_cast<int32_t>(_nameToID.size())};
    _discoveredOrder = std::vector<int32_t>(lockCount, -1);
    _discoveredCount = 0;
    _finished = std::vector<bool>(lockCount, false);
    _parent = std::vector<int32_t>(lockCount, -1);

    for (auto lockID {0}; lockID != lockCount; ++lockID)
        DFS(lockID);

    _discoveredOrder.clear();
    _finished.clear();
    _parent.clear();
}

void Engine::DeadLockProfiler::DFS(int32_t idx) {
    if (_discoveredOrder[idx] != -1)
        return;

    _discoveredOrder[idx] = _discoveredCount++;

    if (!_lockHistory.contains(idx)) {
        _finished[idx] = true;
        return;
    }

    auto const& nextSet = _lockHistory[idx];
    for (auto const there : nextSet) {
        // not visited yet
        if (_discoveredOrder[there] != -1) {
            _parent[there] = idx;
            DFS(there);
            continue;
        }

        // if, idx is discovered before there, there is descendant of idx (forward direction)
        if (_discoveredOrder[idx] < _discoveredOrder[there])
            continue;

        // if there is discovered before idx, there is ancestor of idx (backward direction)
        if (_finished[there] == false) { // and not finished yet, it's a cycle
            std::cout << _iDtoName[idx] << " -> " << _iDtoName[there] << '\n';

            auto now {idx};
            while (true) {
                //std::cout << _iDtoName[_parent[now]] << " -> " << _iDtoName[now] << '\n';
                if (now >= 0)
                    now = _parent[now];
                else
                    break;
                
                if (now == there)
                    break;
            }
            
            CRASH("DEADLOCK_DETECTED");
        }
    }

    _finished[idx] = true;
}
