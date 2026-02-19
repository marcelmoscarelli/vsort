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

    bool is_done() const {
        return m_done;
    }

protected:
    bool m_done = false;
};

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
