#pragma once

#include <string> // std::string
#include <vector> // std::vector

// Simple class for returning the state of a single step of the sorting algorithm back to the caller
struct SortStepResult {
    int hi1 = -1;
    int hi2 = -1;
    bool compared = false;
    bool swapped = false;
    bool done = false;
};

// Base class for sorting algorithms. Each algorithm should inherit from this and implement the name(), reset() and step() methods.
class SortingAlgo {
public:
    virtual ~SortingAlgo() = default;

    virtual const char* name() const = 0;

    virtual void reset(int size) = 0;
    virtual SortStepResult step(std::vector<int>& arr) = 0;

    bool is_done() const {
        return m_done;
    }

protected:
    bool m_done = false;
};

// Class for Bubble Sort algorithm
class BubbleSort : public SortingAlgo {
public:
    const char* name() const override;

    void reset(int size) override;
    SortStepResult step(std::vector<int>& arr) override;

private:
    int m_size = 0;
    int m_i = 0;
    int m_j = 0;
    bool m_swapped_in_pass = false;
};

// Class for Selection Sort algorithm
class SelectionSort : public SortingAlgo {
public:
    const char* name() const override;

    void reset(int size) override;
    SortStepResult step(std::vector<int>& arr) override;

private:
    int m_size = 0;
    int m_i = 0;
    int m_j = 1;
    int m_min_idx = 0;
    bool m_ready_to_swap = false;
};


// Class for Insertion Sort algorithm
class InsertionSort : public SortingAlgo {
public:
    const char* name() const override;

    void reset(int size) override;
    SortStepResult step(std::vector<int>& arr) override;

private:
    int m_size = 0;
    int m_i = 1;
    int m_j = 1;
};

// Class for Cocktail Sort algorithm
class CocktailSort : public SortingAlgo {
public:
    const char* name() const override;

    void reset(int size) override;
    SortStepResult step(std::vector<int>& arr) override;

private:
    int m_size = 0;
    int m_start = 0;
    int m_end = 0;
    int m_j = 0;
    bool m_forward = true;
    bool m_swapped_in_pass = false;
};

// Class for Comb Sort algorithm
class CombSort : public SortingAlgo {
public:
    const char* name() const override;

    void reset(int size) override;
    SortStepResult step(std::vector<int>& arr) override;

private:
    static int next_gap(int gap);

    int m_size = 0;
    int m_gap = 1;
    int m_i = 0;
    bool m_swapped_in_pass = false;
};

// Class for Shell Sort algorithm
class ShellSort : public SortingAlgo {
public:
    const char* name() const override;

    void reset(int size) override;
    SortStepResult step(std::vector<int>& arr) override;

private:
    int m_size = 0;
    std::vector<int> m_gaps;
    int m_gap_idx = 0;
    int m_i = 0;
    int m_j = 0;
};

// Class for Quick Sort algorithm
class QuickSort : public SortingAlgo {
public:
    const char* name() const override;

    void reset(int size) override;
    SortStepResult step(std::vector<int>& arr) override;

private:
    struct Range {
        int lo = 0;
        int hi = 0;
    };

    static constexpr int INSERTION_SORT_THRESHOLD = 4;

    int m_size = 0;
    std::vector<Range> m_stack;

    bool m_has_active_partition = false;
    int m_lo = 0;
    int m_hi = 0;
    int m_lt = 0;
    int m_i = 0;
    int m_gt = 0;
    int m_pivot = 0;

    bool m_in_insertion = false;
    int m_ins_lo = 0;
    int m_ins_hi = 0;
    int m_ins_i = 0;
    int m_ins_j = 0;
};

// Class for Heap Sort algorithm
class HeapSort : public SortingAlgo {
public:
    const char* name() const override;

    void reset(int size) override;
    SortStepResult step(std::vector<int>& arr) override;

private:
    int m_size = 0;

    bool m_building_heap = true;
    int m_build_index = 0;
    int m_extract_end = 0;

    bool m_sift_active = false;
    int m_sift_root = 0;
    int m_sift_end = 0;
    int m_sift_left = 0;
    int m_sift_right = 0;
    int m_sift_child = 0;
    int m_sift_stage = 0;
};

// Class for Merge Sort algorithm
class MergeSort : public SortingAlgo {
public:
    const char* name() const override;

    void reset(int size) override;
    SortStepResult step(std::vector<int>& arr) override;

private:
    static constexpr int INSERTION_SORT_THRESHOLD = 4;

    int m_size = 0;
    std::vector<int> m_buffer;

    int m_width = 1;
    int m_left = 0;

    bool m_in_pre_insertion = false;
    int m_run_lo = 0;
    int m_run_hi = 0;
    int m_ins_i = 0;
    int m_ins_j = 0;

    bool m_merge_active = false;
    bool m_copying_back = false;
    int m_mid = 0;
    int m_right = 0;
    int m_i = 0;
    int m_j = 0;
    int m_k = 0;
    int m_copy_idx = 0;
};