#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/backends/imgui_impl_sdlrenderer2.h"

// Global constants
static const int ARRAY_SIZE = 125;
static const int WINDOW_WIDTH = 1280;
static const int WINDOW_HEIGHT = 720;
static const int STATS_LINE_COUNT = 5;
static const float PADDING = 5.0f;
static const float SECTION_GAP = PADDING;
static const float FONT_SIZE = 17.0f;
static const float BAR_SPACING = 2.0f;

// Global variables
static unsigned int g_num_swaps = 0;
static unsigned int g_num_compar = 0;
static float g_fps = 0.0f;
static bool g_sorting_paused = true;
static bool g_sorting_done = false;
static int g_window_width = WINDOW_WIDTH;
static int g_window_height = WINDOW_HEIGHT;
SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;

// Function prototypes
static int init_sdl();
static int init_imgui();
static void init_array(std::vector<int>& arr);
static void render_bars(const std::vector<int>& arr, int hi1, int hi2, ImU32 color1, ImU32 color2);
static void render_stats();
static void render_controls(std::vector<int>& arr);
static float calc_stats_height();

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    // Initialize SDL and create window and renderer
    if (init_sdl() != 0) {
        fprintf(stderr, "Failed to initialize SDL. Exiting...\n");
        return 1;
    }

    // Initialize ImGui and load external font
    if (init_imgui() != 0) {
        fprintf(stderr, "Failed to initialize ImGui. Exiting...\n");
        return 1;
    }

    // Initialize the array to be sorted
    std::vector<int> arr;
    init_array(arr);

    // Variables for bubble sort state (to be replace when more algorithms are added)
    bool done = false;
    int i = 0;
    int j = 0;
    bool swapped_in_pass = false;

    // Variables for FPS calculation
    Uint32 fps_last_ticks = SDL_GetTicks();
    unsigned int fps_frames = 0;

    // Main loop
    while (!done) {
        Uint32 frame_start = SDL_GetTicks();

        // SDL event handling
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) {
                done = true;
            }
            if (event.type == SDL_WINDOWEVENT && 
                event.window.event == SDL_WINDOWEVENT_CLOSE &&
                event.window.windowID == SDL_GetWindowID(g_window)) {
                done = true;
            }
            if (event.type == SDL_WINDOWEVENT &&
                event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED &&
                event.window.windowID == SDL_GetWindowID(g_window)) {
                g_window_width = event.window.data1;
                g_window_height = event.window.data2;
                ImGui::GetIO().DisplaySize = ImVec2((float)g_window_width, (float)g_window_height);
            }
            if (event.type == SDL_KEYDOWN &&
                event.key.keysym.sym == SDLK_SPACE &&
                event.key.repeat == 0) {
                if (!g_sorting_done) {
                    g_sorting_paused = !g_sorting_paused;
                } else {
                    init_array(arr);
                    g_sorting_done = false;
                    g_sorting_paused = true;
                }
            }
        }

        // One bubble sort step per frame (to be changed later)
        int hi1 = -1;
        int hi2 = -1;
        if (!g_sorting_done && !g_sorting_paused && i < ARRAY_SIZE - 1) {
            hi1 = j;
            hi2 = j + 1;
            ++g_num_compar;
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                ++g_num_swaps;
                swapped_in_pass = true;
            }
            ++j;
            if (j >= ARRAY_SIZE - i - 1) {
                if (!swapped_in_pass) {
                    i = ARRAY_SIZE; // done
                    g_sorting_done = true;
                } else {
                    swapped_in_pass = false;
                    j = 0;
                    ++i;
                }
            }
        } else if (i >= ARRAY_SIZE - 1) {
            g_sorting_done = true;
            i = 0;
            j = 0;
        }

        // Start ImGui frame with the SDL2 backend
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // Render VSort stuff
        render_stats();
        render_controls(arr);
        render_bars(arr, hi1, hi2, IM_COL32(255, 60, 60, 255), IM_COL32(255, 200, 0, 255));

        // Render ImGui and present the frame
        ImGui::Render();
        SDL_SetRenderDrawColor(g_renderer, 13, 13, 13, 255);
        SDL_RenderClear(g_renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), g_renderer);
        SDL_RenderPresent(g_renderer);

        // FPS calculation
        ++fps_frames;
        Uint32 now = SDL_GetTicks();
        Uint32 elapsed = now - fps_last_ticks;
        if (elapsed >= 500) {
            g_fps = (fps_frames * 1000.0f) / (float)elapsed;
            fps_frames = 0;
            fps_last_ticks = now;
        }

        // Cap frame rate to ~30 FPS when sorting is paused or done to reduce CPU usage
        if (g_sorting_done || g_sorting_paused) {
            Uint32 frame_time = SDL_GetTicks() - frame_start;
            if (frame_time < 33) {
                SDL_Delay(33 - frame_time);
            }
        }
    }

    // Cleanup ImGui and SDL
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    SDL_Quit();

    return 0;
}

static int init_sdl() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        std::fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    g_window = SDL_CreateWindow("vsort", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (!g_window) {
        std::fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
    if (!g_renderer) {
        std::fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(g_window);
        SDL_Quit();
        return 1;
    }

    SDL_GetWindowSize(g_window, &g_window_width, &g_window_height);
    return 0;
}

static int init_imgui() {
    // Create and initialize ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    // Initialize ImGui SDL2 and SDLRenderer2 backends
    ImGui_ImplSDL2_InitForSDLRenderer(g_window, g_renderer);
    ImGui_ImplSDLRenderer2_Init(g_renderer);

    // Load an external font
    char* base = SDL_GetBasePath();
    std::string base_str(base);
    SDL_free(base);
    std::replace(base_str.begin(), base_str.end(), '\\', '/'); // Not really needed, but...
    if (base_str.empty()) {
        fprintf(stderr, "SDL_GetBasePath failed: %s\n", SDL_GetError());
        return 1;
    }
    std::string font_path = base_str + "../assets/DejaVuSansMono.ttf";
    ImFont* font = ImGui::GetIO().Fonts->AddFontFromFileTTF(font_path.c_str(), FONT_SIZE);
    if (font) {
        ImGui::GetIO().FontDefault = font;
        fprintf(stdout, "Font loaded from: %s\n", font_path.c_str());
    } else {
        fprintf(stderr, "AddFontFromFileTTF() failed to load font at path: %s\n", font_path.c_str());
        return 1;
    }
    return 0;
}

static void init_array(std::vector<int>& arr) {
    arr.resize(ARRAY_SIZE);
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        arr[i] = i + 1;
    }
    // Shuffle
    std::srand((unsigned int)std::time(nullptr));
    for (int i = ARRAY_SIZE - 1; i >= 1; --i) {
        int j = std::rand() % (i + 1);
        std::swap(arr[i], arr[j]);
    }
    g_num_swaps = 0;
    g_num_compar = 0;
}

static void render_bars(const std::vector<int>& arr, int hi1, int hi2, ImU32 color1, ImU32 color2) {
    const float stats_height = calc_stats_height();
    const float sorting_height = (float)g_window_height - stats_height - (PADDING * 2.0f) - SECTION_GAP;

    ImGui::SetNextWindowPos(ImVec2(PADDING, PADDING), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2((float)g_window_width - (PADDING * 2.0f), sorting_height), ImGuiCond_Always);

    ImGuiWindowFlags flags = /*ImGuiWindowFlags_NoTitleBar | */ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse;
    ImGui::Begin("Sorting", nullptr, flags);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImVec2 avail = ImGui::GetContentRegionAvail();

    const float bar_width = (avail.x - (ARRAY_SIZE - 1) * BAR_SPACING) / ARRAY_SIZE;
    const float bar_max_height = avail.y;

    for (int i = 0; i < ARRAY_SIZE; ++i) {
        float h = (arr[i] / (float)ARRAY_SIZE) * bar_max_height;
        float x0 = p.x + i * (bar_width + BAR_SPACING);
        float y0 = p.y + (bar_max_height - h);
        float x1 = x0 + bar_width;
        float y1 = p.y + bar_max_height;

        ImU32 col = IM_COL32(220, 220, 220, 255);
        if (i == hi1) col = color1;
        if (i == hi2) col = color2;

        draw_list->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), col);
    }

    ImGui::End();
}

static void render_stats() {
    const float stats_height = calc_stats_height();
    const float sorting_height = (float)g_window_height - stats_height - (PADDING * 2.0f) - SECTION_GAP;
    const float stats_y = PADDING + sorting_height + SECTION_GAP;

    ImGui::SetNextWindowPos(ImVec2(PADDING, stats_y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2((float)g_window_width/2 - (PADDING * 2.0f), stats_height), ImGuiCond_Always);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse;
    ImGui::Begin("Stats", nullptr, flags);

    ImGui::Text("Algorithm: %s\t", "Bubble Sort"); // Placeholder for when we have more algorithms
    ImGui::Text("Swaps: %u", g_num_swaps);
    ImGui::Text("Comparisons: %u", g_num_compar);
    ImGui::Text("FPS: %.1f", g_fps);

    ImGui::End();
}

static void render_controls(std::vector<int>& arr) {
    const float stats_height = calc_stats_height();
    const float sorting_height = (float)g_window_height - stats_height - (PADDING * 2.0f) - SECTION_GAP;
    const float stats_y = PADDING + sorting_height + SECTION_GAP;

    ImGui::SetNextWindowPos(ImVec2(g_window_width/2 + PADDING, stats_y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2((float)g_window_width/2 - (PADDING * 2.0f), stats_height), ImGuiCond_Always);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse;
    ImGui::Begin("Controls", nullptr, flags);

    if (ImGui::Button((g_sorting_paused || g_sorting_done) ? "Paused.." : "Running!")) {
        if (!g_sorting_done) {
            g_sorting_paused = !g_sorting_paused;
        }
    }

    if (g_sorting_done &&ImGui::Button("Reset..?")) {
        init_array(arr);
        g_sorting_done = false;
        g_sorting_paused = true;
    }

    ImGui::End();
}

static float calc_stats_height() {
    const ImGuiStyle& style = ImGui::GetStyle();
    const float line_height = ImGui::GetTextLineHeightWithSpacing();
    const float content_height = (STATS_LINE_COUNT * line_height) - style.ItemSpacing.y;
    return (style.WindowPadding.y * 2.0f) + content_height;
}