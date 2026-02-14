#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>

#include <SDL2/SDL.h>
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/backends/imgui_impl_sdlrenderer2.h"

static const int ARRAY_SIZE = 150;
static const int WINDOW_WIDTH = 1600;
static const int WINDOW_HEIGHT = 900;

static unsigned int g_num_swaps = 0;
static unsigned int g_num_compar = 0;
static float g_fps = 0.0f;
SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;

static int init_sdl();
static void init_array(std::vector<int>& arr);
static void render_bars(const std::vector<int>& arr, int hi1, int hi2, ImU32 color1, ImU32 color2);

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;
    if (init_sdl() != 0) {
        fprintf(stderr, "Failed to initialize SDL. Exiting...\n");
        return 1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(g_window, g_renderer);
    ImGui_ImplSDLRenderer2_Init(g_renderer);

    std::vector<int> arr;
    init_array(arr);

    bool done = false;
    int i = 0;
    int j = 0;
    bool swapped_in_pass = false;
    bool sorting_done = false;

    Uint32 fps_last_ticks = SDL_GetTicks();
    unsigned int fps_frames = 0;

    while (!done) {
        Uint32 frame_start = SDL_GetTicks();
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) {
                done = true;
            }
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
                event.window.windowID == SDL_GetWindowID(g_window)) {
                done = true;
            }
        }

        // One bubble sort step per frame.
        int hi1 = -1;
        int hi2 = -1;
        if (!sorting_done && i < ARRAY_SIZE - 1) {
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
                    sorting_done = true;
                } else {
                    swapped_in_pass = false;
                    j = 0;
                    ++i;
                }
            }
        } else if (i >= ARRAY_SIZE - 1) {
            sorting_done = true;
        }

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        render_bars(arr, hi1, hi2, IM_COL32(255, 60, 60, 255), IM_COL32(255, 200, 0, 255));

        ImGui::Render();
        SDL_SetRenderDrawColor(g_renderer, 13, 13, 13, 255);
        SDL_RenderClear(g_renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), g_renderer);
        SDL_RenderPresent(g_renderer);

        ++fps_frames;
        Uint32 now = SDL_GetTicks();
        Uint32 elapsed = now - fps_last_ticks;
        if (elapsed >= 500) {
            g_fps = (fps_frames * 1000.0f) / (float)elapsed;
            fps_frames = 0;
            fps_last_ticks = now;
        }

        if (sorting_done) {
            Uint32 frame_time = SDL_GetTicks() - frame_start;
            if (frame_time < 33) {
                SDL_Delay(33 - frame_time);
            }
        }
    }

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

    g_window = SDL_CreateWindow("ImGui VSort", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
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
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT), ImGuiCond_Always);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
    ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("VSort ImGui", nullptr, flags);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImVec2 avail = ImGui::GetContentRegionAvail();

    const float space = 1.0f;
    const float bar_width = (avail.x - (ARRAY_SIZE - 1) * space) / ARRAY_SIZE;
    const float bar_max_height = avail.y - 40.0f; // leave space for stats

    for (int i = 0; i < ARRAY_SIZE; ++i) {
        float h = (arr[i] / (float)ARRAY_SIZE) * bar_max_height;
        float x0 = p.x + i * (bar_width + space);
        float y0 = p.y + (bar_max_height - h);
        float x1 = x0 + bar_width;
        float y1 = p.y + bar_max_height;

        ImU32 col = IM_COL32(220, 220, 220, 255);
        if (i == hi1) col = color1;
        if (i == hi2) col = color2;

        draw_list->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), col);
    }

    ImGui::SetCursorScreenPos(ImVec2(p.x, p.y + bar_max_height + 6.0f));
    ImGui::Text("Swaps: %u", g_num_swaps);
    ImGui::SameLine();
    ImGui::Text("Comparisons: %u", g_num_compar);
    ImGui::Text("FPS: %.1f", g_fps);

    ImGui::End();
}
