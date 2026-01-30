#pragma once

// Windows API for shared memory
#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
#endif

#include <cstddef>
#include <cstdint>

/**
 * Generic Shared Memory Library
 * 
 * Template-based library that works with ANY structure size.
 * No hardcoded types - fully reusable!
 * 
 * Note: Uses native C++ types internally to avoid Windows.h conflicts.
 * The CodesysTypes.hpp is included only by the generated types.
 */

class GenericSharedMemory {
private:
    HANDLE hMapFile;
    void* pData;
    size_t dataSize;
    bool connected;
    int32_t* pLifePLC;
    int32_t* pLifeCPP;
    int32_t lastLifePLC;

public:
    GenericSharedMemory() 
        : hMapFile(NULL), pData(nullptr), dataSize(0), connected(false),
          pLifePLC(nullptr), pLifeCPP(nullptr), lastLifePLC(0) {}

    ~GenericSharedMemory() {
        if (pData) UnmapViewOfFile(pData);
        if (hMapFile) CloseHandle(hMapFile);
    }

    /**
     * Connect to shared memory
     * @param name Shared memory name (e.g., L"GlobalLibMem")
     * @param size Total size of structure in bytes
     * @return true if connected successfully
     */
    bool Connect(const wchar_t* name, size_t size) {
        if (connected) return true;

        dataSize = size;
        
        hMapFile = OpenFileMappingW(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, name);
        if (!hMapFile) return false;

        pData = MapViewOfFile(hMapFile, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, dataSize);
        if (!pData) {
            CloseHandle(hMapFile);
            hMapFile = NULL;
            return false;
        }

        // Calculate heartbeat positions (last 8 bytes)
        pLifePLC = reinterpret_cast<int32_t*>(static_cast<char*>(pData) + dataSize - 8);
        pLifeCPP = reinterpret_cast<int32_t*>(static_cast<char*>(pData) + dataSize - 4);
        
        connected = true;
        return true;
    }

    /**
     * Get raw pointer to shared memory
     */
    void* GetPointer() const {
        return pData;
    }

    /**
     * Get data size
     */
    size_t GetSize() const {
        return dataSize;
    }

    /**
     * Check if connected
     */
    bool IsConnected() const {
        return connected;
    }

    /**
     * Increment C++ heartbeat
     */
    void Sync() {
        if (connected && pLifeCPP) {
            (*pLifeCPP)++;
        }
    }

    /**
     * Check if PLC is active (heartbeat changing)
     */
    bool IsPLCActive() {
        if (!connected || !pLifePLC) return false;
        
        int32_t current = *pLifePLC;
        bool active = (current != lastLifePLC);
        lastLifePLC = current;
        return active;
    }
};

/**
 * Type-Safe Wrapper for Shared Memory
 * 
 * Template class that provides compile-time type safety
 * while using the generic library underneath.
 */
template<typename TStruct>
class TypedSharedMemory : public GenericSharedMemory {
public:
    TypedSharedMemory() : GenericSharedMemory() {}

    /**
     * Connect using the structure's size automatically
     */
    bool Connect(const wchar_t* name) {
        return GenericSharedMemory::Connect(name, sizeof(TStruct));
    }

    /**
     * Get typed pointer to data
     */
    TStruct* GetData() const {
        return static_cast<TStruct*>(GetPointer());
    }

    /**
     * Operator overload for easy access
     */
    TStruct* operator->() const {
        return GetData();
    }

    TStruct& operator*() const {
        return *GetData();
    }
};
