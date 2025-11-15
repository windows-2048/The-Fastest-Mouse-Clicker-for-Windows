#pragma once


#define MOUSE_WHEEL_UP 16777216
#define MOUSE_WHEEL_DOWN 16777217

#define CONDITION_PRESS_SELF 134217728
#define CONDITION_TOGGLE_SELF 134217729


#include <atomic>
#include <cstdint>
//#include <cstdio>

#include <QMutex>
#include <QWaitCondition>


extern QMutex g_postMutex;
extern QWaitCondition g_postWaitCondition;


extern std::atomic<uint64_t> g_shared_data_mouse_coords_x;
extern std::atomic<uint64_t> g_shared_data_mouse_coords_y;
extern std::atomic<uint64_t> g_shared_data_clicking_mode;
extern std::atomic<uint64_t> g_shared_data_clicking_emit;
extern std::atomic<double> g_shared_data_clicking_mean_interval;
extern std::atomic<double> g_shared_data_clicking_reldev_interval;
extern std::atomic<double> g_shared_data_clicking_radius;
extern std::atomic<double> g_shared_data_clicking_current_timeout;
extern std::atomic<uint64_t> g_shared_data_trigger;
extern std::atomic<uint64_t> g_shared_data_event;
extern std::atomic<uint64_t> g_shared_data_condition;
extern std::atomic<uint64_t> g_shared_data_number_of_clicks;


// Writer thread - only one thread should call this
template<typename T>
void atomic_writer_thread(std::atomic<T>& shared_data, T new_value)
{
    //fprintf(stderr, "shared_data.is_lock_free() %d\n", shared_data.is_lock_free());
    // Release semantics: ensure all previous writes are visible
    // to the reader when they see this value
    shared_data.store(new_value, std::memory_order_release);
}

template<typename T>
// Reader thread - only one thread should call this  
T atomic_reader_thread(const std::atomic<T>& shared_data)
{
    //fprintf(stderr, "shared_data.is_lock_free() %d\n", shared_data.is_lock_free());
    // Acquire semantics: ensure we see all writes that happened
    // before the store in the writer thread
    return shared_data.load(std::memory_order_acquire);
}

template<typename T>
T atomic_increment(std::atomic<T>& shared_data, T increment_by = 1)
{
    // fetch_add returns the OLD value (before increment), not the new value
    return shared_data.fetch_add(increment_by, std::memory_order_acq_rel);
    // Returns: value before the addition
    // But the atomic variable now contains: old_value + increment_by
}

template<typename T>
T atomic_increment_get_new(std::atomic<T>& shared_data, T increment_by = 1)
{
    // Returns the value AFTER increment
    return shared_data.fetch_add(increment_by, std::memory_order_acq_rel) + increment_by;
}

