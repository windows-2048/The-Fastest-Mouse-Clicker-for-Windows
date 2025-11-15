#include "global_atomic.h"


QMutex g_postMutex;
QWaitCondition g_postWaitCondition;


// Global atomic variable - no volatile needed
std::atomic<uint64_t> g_shared_data_mouse_coords_x{ 0 };
std::atomic<uint64_t> g_shared_data_mouse_coords_y{ 0 };
std::atomic<uint64_t> g_shared_data_clicking_mode{ 0 };
std::atomic<uint64_t> g_shared_data_clicking_emit{ 0 };
std::atomic<double> g_shared_data_clicking_mean_interval{ 0 };
std::atomic<double> g_shared_data_clicking_reldev_interval{ 0 };
std::atomic<double> g_shared_data_clicking_radius{ 0 };
std::atomic<double> g_shared_data_clicking_current_timeout{ 0 };
std::atomic<uint64_t> g_shared_data_trigger{ 0 };
std::atomic<uint64_t> g_shared_data_event{ 0 };
std::atomic<uint64_t> g_shared_data_condition{ 0 };
std::atomic<uint64_t> g_shared_data_number_of_clicks{ 0 };
