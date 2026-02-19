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
