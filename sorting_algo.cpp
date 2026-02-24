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
    return "Comb Sort (1.3)";
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

/* SHELL SORT IMPLEMENTATION */
const char* ShellSort::name() const {
    return "Shell Sort (Ciura + 2.25)";
}

void ShellSort::reset(int size) {
    m_size = size;
    m_gaps.clear();
    m_gap_idx = 0;
    m_i = 0;
    m_j = 0;
    m_done = (size <= 1);

    if (m_done) {
        return;
    }

    std::vector<int> ciura = {1, 4, 10, 23, 57, 132, 301, 701};
    int last_gap = ciura.back();
    while (last_gap < size) {
        int next = (last_gap * 9) / 4;
        if (next <= last_gap) {
            next = last_gap + 1;
        }
        ciura.push_back(next);
        last_gap = next;
    }

    for (int index = (int)ciura.size() - 1; index >= 0; --index) {
        if (ciura[index] < size) {
            m_gaps.push_back(ciura[index]);
        }
    }

    if (m_gaps.empty()) {
        m_done = true;
        return;
    }

    m_gap_idx = 0;
    m_i = m_gaps[m_gap_idx];
    m_j = m_i;
}

SortStepResult ShellSort::step(std::vector<int>& arr) {
    SortStepResult result;

    if (m_done || m_size <= 1 || (int)arr.size() < m_size || m_gaps.empty()) {
        result.done = true;
        m_done = true;
        return result;
    }

    int gap = m_gaps[m_gap_idx];
    if (m_i >= m_size) {
        ++m_gap_idx;
        if (m_gap_idx >= (int)m_gaps.size()) {
            m_done = true;
            result.done = true;
            return result;
        }

        gap = m_gaps[m_gap_idx];
        m_i = gap;
        m_j = m_i;
    }

    if (m_j < gap || m_j > m_i) {
        m_j = m_i;
    }

    result.hi1 = m_j - gap;
    result.hi2 = m_j;
    result.compared = true;

    if (arr[m_j - gap] > arr[m_j]) {
        std::swap(arr[m_j - gap], arr[m_j]);
        result.swapped = true;
        m_j -= gap;

        if (m_j < gap) {
            ++m_i;
            m_j = m_i;
        }
    } else {
        ++m_i;
        m_j = m_i;
    }

    if (m_gap_idx == (int)m_gaps.size() - 1 && m_i >= m_size) {
        m_done = true;
        result.done = true;
    }

    return result;
}

/* QUICK SORT IMPLEMENTATION */
const char* QuickSort::name() const {
    return "Quick Sort (Mid P + 3-way)";
}

void QuickSort::reset(int size) {
    m_size = size;
    m_stack.clear();
    m_has_active_partition = false;
    m_in_insertion = false;
    m_done = (size <= 1);

    if (!m_done) {
        m_stack.push_back({0, size - 1});
    }
}

SortStepResult QuickSort::step(std::vector<int>& arr) {
    SortStepResult result;

    if (m_done || m_size <= 1 || (int)arr.size() < m_size) {
        result.done = true;
        m_done = true;
        return result;
    }

    while (true) {
        if (m_in_insertion) {
            if (m_ins_i > m_ins_hi) {
                m_in_insertion = false;
                continue;
            }

            if (m_ins_j <= m_ins_lo) {
                ++m_ins_i;
                m_ins_j = m_ins_i;
                continue;
            }

            result.hi1 = m_ins_j - 1;
            result.hi2 = m_ins_j;
            result.compared = true;
            if (arr[m_ins_j - 1] > arr[m_ins_j]) {
                std::swap(arr[m_ins_j - 1], arr[m_ins_j]);
                result.swapped = true;
                --m_ins_j;
            } else {
                ++m_ins_i;
                m_ins_j = m_ins_i;
            }
            return result;
        }

        if (!m_has_active_partition) {
            if (m_stack.empty()) {
                m_done = true;
                result.done = true;
                return result;
            }

            const Range range = m_stack.back();
            m_stack.pop_back();

            if (range.lo >= range.hi) {
                continue;
            }

            const int range_size = range.hi - range.lo + 1;
            if (range_size <= INSERTION_SORT_THRESHOLD) {
                m_in_insertion = true;
                m_ins_lo = range.lo;
                m_ins_hi = range.hi;
                m_ins_i = m_ins_lo + 1;
                m_ins_j = m_ins_i;
                continue;
            }

            m_lo = range.lo;
            m_hi = range.hi;

            const int mid = m_lo + ((m_hi - m_lo) / 2);
            m_pivot = arr[mid];
            m_lt = m_lo;
            m_i = m_lo;
            m_gt = m_hi;
            m_has_active_partition = true;
            continue;
        }

        if (m_i <= m_gt) {
            result.hi1 = m_i;
            result.hi2 = m_gt;
            result.compared = true;

            if (arr[m_i] < m_pivot) {
                if (m_lt != m_i) {
                    std::swap(arr[m_lt], arr[m_i]);
                    result.swapped = true;
                }
                ++m_lt;
                ++m_i;
            } else if (arr[m_i] > m_pivot) {
                if (m_i != m_gt) {
                    std::swap(arr[m_i], arr[m_gt]);
                    result.swapped = true;
                }
                --m_gt;
            } else {
                ++m_i;
            }

            return result;
        }

        const int left_lo = m_lo;
        const int left_hi = m_lt - 1;
        const int right_lo = m_gt + 1;
        const int right_hi = m_hi;

        const int left_size = (left_hi >= left_lo) ? (left_hi - left_lo + 1) : 0;
        const int right_size = (right_hi >= right_lo) ? (right_hi - right_lo + 1) : 0;

        if (left_size > 1 || right_size > 1) {
            if (left_size < right_size) {
                if (right_size > 1) {
                    m_stack.push_back({right_lo, right_hi});
                }
                if (left_size > 1) {
                    m_stack.push_back({left_lo, left_hi});
                }
            } else {
                if (left_size > 1) {
                    m_stack.push_back({left_lo, left_hi});
                }
                if (right_size > 1) {
                    m_stack.push_back({right_lo, right_hi});
                }
            }
        }

        m_has_active_partition = false;
    }
}

/* HEAP SORT IMPLEMENTATION */
const char* HeapSort::name() const {
    return "Heap Sort";
}

void HeapSort::reset(int size) {
    m_size = size;
    m_building_heap = true;
    m_build_index = (size / 2) - 1;
    m_extract_end = size - 1;
    m_sift_active = false;
    m_sift_root = 0;
    m_sift_end = 0;
    m_sift_left = 0;
    m_sift_right = 0;
    m_sift_child = 0;
    m_sift_stage = 0;
    m_done = (size <= 1);
}

SortStepResult HeapSort::step(std::vector<int>& arr) {
    SortStepResult result;

    if (m_done || m_size <= 1 || (int)arr.size() < m_size) {
        result.done = true;
        m_done = true;
        return result;
    }

    while (true) {
        if (!m_sift_active) {
            if (m_building_heap) {
                if (m_build_index < 0) {
                    m_building_heap = false;
                    m_extract_end = m_size - 1;
                    continue;
                }

                m_sift_root = m_build_index;
                m_sift_end = m_size - 1;
                m_sift_stage = 0;
                m_sift_active = true;
                continue;
            }

            if (m_extract_end <= 0) {
                m_done = true;
                result.done = true;
                return result;
            }

            result.hi1 = 0;
            result.hi2 = m_extract_end;
            if (m_extract_end != 0) {
                std::swap(arr[0], arr[m_extract_end]);
                result.swapped = true;
            }

            if (m_extract_end - 1 > 0) {
                m_sift_root = 0;
                m_sift_end = m_extract_end - 1;
                m_sift_stage = 0;
                m_sift_active = true;
            } else {
                --m_extract_end;
                if (m_extract_end <= 0) {
                    m_done = true;
                    result.done = true;
                }
            }

            return result;
        }

        if (m_sift_stage == 0) {
            m_sift_left = (2 * m_sift_root) + 1;
            if (m_sift_left > m_sift_end) {
                m_sift_active = false;
                if (m_building_heap) {
                    --m_build_index;
                } else {
                    --m_extract_end;
                    if (m_extract_end <= 0) {
                        m_done = true;
                        result.done = true;
                        return result;
                    }
                }
                continue;
            }

            m_sift_right = m_sift_left + 1;
            m_sift_child = m_sift_left;
            if (m_sift_right <= m_sift_end) {
                m_sift_stage = 1;
            } else {
                m_sift_stage = 2;
            }
        }

        if (m_sift_stage == 1) {
            result.hi1 = m_sift_left;
            result.hi2 = m_sift_right;
            result.compared = true;
            if (arr[m_sift_left] < arr[m_sift_right]) {
                m_sift_child = m_sift_right;
            }
            m_sift_stage = 2;
            return result;
        }

        result.hi1 = m_sift_root;
        result.hi2 = m_sift_child;
        result.compared = true;
        if (arr[m_sift_root] < arr[m_sift_child]) {
            std::swap(arr[m_sift_root], arr[m_sift_child]);
            result.swapped = true;
            m_sift_root = m_sift_child;
            m_sift_stage = 0;
        } else {
            m_sift_active = false;
            if (m_building_heap) {
                --m_build_index;
            } else {
                --m_extract_end;
                if (m_extract_end <= 0) {
                    m_done = true;
                    result.done = true;
                }
            }
        }
        return result;
    }
}

