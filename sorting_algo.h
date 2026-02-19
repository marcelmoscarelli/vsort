#pragma once

#include <vector>
#include <string>

struct SortStepResult {
    int hi1 = -1;
    int hi2 = -1;
    bool compared = false;
    bool swapped = false;
    bool done = false;
};

class SortingAlgo {
public:
    virtual ~SortingAlgo() = default;

    virtual const char* name() const = 0;

    virtual void reset(int size) = 0;
    virtual SortStepResult step(std::vector<int>& arr) = 0;
    virtual bool is_done() const = 0;
};

class BubbleSort : public SortingAlgo {
public:
    const char* name() const override;

    void reset(int size) override;
    SortStepResult step(std::vector<int>& arr) override;
    bool is_done() const override;

private:
    int m_size = 0;
    int m_i = 0;
    int m_j = 0;
    bool m_swapped_in_pass = false;
    bool m_done = false;
};

class InsertionSort : public SortingAlgo {
public:
    const char* name() const override;

    void reset(int size) override;
    SortStepResult step(std::vector<int>& arr) override;
    bool is_done() const override;

private:
    int m_size = 0;
    int m_i = 1;
    int m_j = 1;
    bool m_done = false;
};

class CocktailSort : public SortingAlgo {
public:
    const char* name() const override;

    void reset(int size) override;
    SortStepResult step(std::vector<int>& arr) override;
    bool is_done() const override;

private:
    int m_size = 0;
    int m_start = 0;
    int m_end = 0;
    int m_j = 0;
    bool m_forward = true;
    bool m_swapped_in_pass = false;
    bool m_done = false;
};
