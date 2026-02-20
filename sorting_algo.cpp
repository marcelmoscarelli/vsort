#include <algorithm> // std::swap()
#include "sorting_algo.h"

/* BUBBLE SORT IMPLEMENTATION */
const char* BubbleSort::name() const {
    return "Bubble Sort";
}

void BubbleSort::reset(int size) {
    m_size = size;
    m_i = 0;
    m_j = 0;
    m_swapped_in_pass = false;
    m_done = (size <= 1);
}

SortStepResult BubbleSort::step(std::vector<int>& arr) {
    SortStepResult result;

    // Base case checks
    if (m_done || m_size <= 1 || (int)arr.size() < m_size) {
        result.done = true;
        m_done = true;
        return result;
    }
    if (m_i >= m_size - 1) {
        m_done = true;
        result.done = true;
        return result;
    }

    // Compare the current pair of elements
    result.hi1 = m_j;
    result.hi2 = m_j + 1;
    result.compared = true;
    if (arr[m_j] > arr[m_j + 1]) {
        std::swap(arr[m_j], arr[m_j + 1]);
        result.swapped = true;
        m_swapped_in_pass = true;
    }
    ++m_j;

    // Check if we reached the end of the current pass to start the next one
    if (m_j >= m_size - m_i - 1) {
        if (!m_swapped_in_pass) { // Early termination if no swaps were made in the pass
            m_done = true;
            result.done = true;
        } else {
            m_swapped_in_pass = false;
            m_j = 0;
            ++m_i;
        }
    }

    return result;
}

/* INSERTION SORT IMPLEMENTATION */
const char* InsertionSort::name() const {
    return "Insertion Sort";
}

void InsertionSort::reset(int size) {
    m_size = size;
    m_i = 1;
    m_j = 1;
    m_done = (size <= 1);
}

SortStepResult InsertionSort::step(std::vector<int>& arr) {
    SortStepResult result;

    // Base case checks
    if (m_done || m_size <= 1 || (int)arr.size() < m_size) {
        result.done = true;
        m_done = true;
        return result;
    }
    if (m_i >= m_size) {
        m_done = true;
        result.done = true;
        return result;
    }
    
    // Ensure m_j is within the bounds of the sorted portion of the array
    if (m_j <= 0 || m_j > m_i) {
        m_j = m_i;
    }

    // Compare the current element with the previous one
    result.hi1 = m_j - 1;
    result.hi2 = m_j;
    result.compared = true;
    if (arr[m_j - 1] > arr[m_j]) {
        std::swap(arr[m_j - 1], arr[m_j]);
        result.swapped = true;
        --m_j;

        if (m_j == 0) {
            ++m_i;
            m_j = m_i;
        }
    } else { // If no swap is needed, move to the next element
        ++m_i;
        m_j = m_i;
    }

    return result;
}

/* COCKTAIL SORT IMPLEMENTATION */
const char* CocktailSort::name() const {
    return "Cocktail Sort";
}

void CocktailSort::reset(int size) {
    m_size = size;
    m_start = 0;
    m_end = size - 1;
    m_j = 0;
    m_forward = true;
    m_swapped_in_pass = false;
    m_done = (size <= 1);
}

SortStepResult CocktailSort::step(std::vector<int>& arr) {
    SortStepResult result;

    // Base case checks
    if (m_done || m_size <= 1 || (int)arr.size() < m_size) {
        result.done = true;
        m_done = true;
        return result;
    }
    if (m_start >= m_end) {
        m_done = true;
        result.done = true;
        return result;
    }

    if (m_forward) { // Forward pass
        // Compare the current pair of elements (Bubble Sort style)
        result.hi1 = m_j;
        result.hi2 = m_j + 1;
        result.compared = true;
        if (arr[m_j] > arr[m_j + 1]) {
            std::swap(arr[m_j], arr[m_j + 1]);
            result.swapped = true;
            m_swapped_in_pass = true;
        }
        ++m_j;

        // Check if the end of the forward pass is reached
        if (m_j >= m_end) {
            if (!m_swapped_in_pass) { // Early termination
                m_done = true;
                result.done = true;
            } else { // Prepare for the next backward pass
                m_swapped_in_pass = false;
                --m_end;
                m_forward = false;
                m_j = m_end;
            }
        }
    } else { // Backward pass
        // Compare the current pair of elements (Bubble Sort style)
        result.hi1 = m_j - 1;
        result.hi2 = m_j;
        result.compared = true;
        if (arr[m_j - 1] > arr[m_j]) {
            std::swap(arr[m_j - 1], arr[m_j]);
            result.swapped = true;
            m_swapped_in_pass = true;
        }
        --m_j;

        if (m_j <= m_start) {
            if (!m_swapped_in_pass) { // Early termination
                m_done = true;
                result.done = true;
            } else { // Prepare for the next forward pass
                m_swapped_in_pass = false;
                ++m_start;
                m_forward = true;
                m_j = m_start;
            }
        }
    }

    return result;
}

/* COMB SORT IMPLEMENTATION */
const char* CombSort::name() const {
    return "Comb Sort";
}

// Shrinks the gap by a factor of 1.3 and ensures it never goes below 1
int CombSort::next_gap(int gap) {
    gap = (gap * 10) / 13;
    if (gap < 1) {
        return 1;
    }
    return gap;
}

void CombSort::reset(int size) {
    m_size = size;
    m_gap = next_gap(size);
    m_i = 0;
    m_swapped_in_pass = false;
    m_done = (size <= 1);
}

SortStepResult CombSort::step(std::vector<int>& arr) {
    SortStepResult result;

    // Base case checks
    if (m_done || m_size <= 1 || (int)arr.size() < m_size) {
        result.done = true;
        m_done = true;
        return result;
    }
    if (m_i >= m_size - m_gap) {
        if (m_gap == 1 && !m_swapped_in_pass) {
            m_done = true;
            result.done = true;
            return result;
        }

        m_gap = next_gap(m_gap);
        m_i = 0;
        m_swapped_in_pass = false;
    }

    // Compare the current pair of elements
    result.hi1 = m_i;
    result.hi2 = m_i + m_gap;
    result.compared = true;
    if (arr[m_i] > arr[m_i + m_gap]) {
        std::swap(arr[m_i], arr[m_i + m_gap]);
        result.swapped = true;
        m_swapped_in_pass = true;
    }
    ++m_i;

    return result;
}

/* SELECTION SORT IMPLEMENTATION */
const char* SelectionSort::name() const {
    return "Selection Sort";
}

void SelectionSort::reset(int size) {
    m_size = size;
    m_i = 0;
    m_j = 1;
    m_min_idx = 0;
    m_ready_to_swap = false;
    m_done = (size <= 1);
}

SortStepResult SelectionSort::step(std::vector<int>& arr) {
    SortStepResult result;

    // Base case checks
    if (m_done || m_size <= 1 || (int)arr.size() < m_size) {
        result.done = true;
        m_done = true;
        return result;
    }
    if (m_i >= m_size - 1) {
        m_done = true;
        result.done = true;
        return result;
    }

    // Compare the current candidate minimum with the current element
    if (!m_ready_to_swap) {
        result.hi1 = m_min_idx;
        result.hi2 = m_j;
        result.compared = true;
        if (arr[m_j] < arr[m_min_idx]) {
            m_min_idx = m_j;
        }
        ++m_j;

        // Check if the end of the unsorted portion was reached
        if (m_j >= m_size) {
            m_ready_to_swap = true;
        }

        return result;
    }

    // Swap the selected minimum into place and advance to the next position
    result.hi1 = m_i;
    result.hi2 = m_min_idx;
    if (m_min_idx != m_i) {
        std::swap(arr[m_i], arr[m_min_idx]);
        result.swapped = true;
    }

    ++m_i;
    if (m_i >= m_size - 1) {
        m_done = true;
        result.done = true;
        return result;
    }

    m_min_idx = m_i;
    m_j = m_i + 1;
    m_ready_to_swap = false;
    return result;
}
