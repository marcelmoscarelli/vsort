#include "sorting_algo.h"

#include <algorithm>

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

    result.hi1 = m_j;
    result.hi2 = m_j + 1;
    result.compared = true;

    if (arr[m_j] > arr[m_j + 1]) {
        std::swap(arr[m_j], arr[m_j + 1]);
        result.swapped = true;
        m_swapped_in_pass = true;
    }

    ++m_j;

    if (m_j >= m_size - m_i - 1) {
        if (!m_swapped_in_pass) {
            m_done = true;
            result.done = true;
        } else {
            m_swapped_in_pass = false;
            m_j = 0;
            ++m_i;
            if (m_i >= m_size - 1) {
                m_done = true;
                result.done = true;
            }
        }
    }

    return result;
}

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

    if (m_j <= 0 || m_j > m_i) {
        m_j = m_i;
    }

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
    } else {
        ++m_i;
        m_j = m_i;
    }

    if (m_i >= m_size) {
        m_done = true;
        result.done = true;
    }

    return result;
}

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

    if (m_forward) {
        result.hi1 = m_j;
        result.hi2 = m_j + 1;
        result.compared = true;

        if (arr[m_j] > arr[m_j + 1]) {
            std::swap(arr[m_j], arr[m_j + 1]);
            result.swapped = true;
            m_swapped_in_pass = true;
        }

        ++m_j;
        if (m_j >= m_end) {
            if (!m_swapped_in_pass) {
                m_done = true;
                result.done = true;
            } else {
                m_swapped_in_pass = false;
                --m_end;
                m_forward = false;
                m_j = m_end;
            }
        }
    } else {
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
            if (!m_swapped_in_pass) {
                m_done = true;
                result.done = true;
            } else {
                m_swapped_in_pass = false;
                ++m_start;
                m_forward = true;
                m_j = m_start;
            }
        }
    }

    if (m_start >= m_end) {
        m_done = true;
        result.done = true;
    }

    return result;
}

const char* CombSort::name() const {
    return "Comb Sort";
}

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

    result.hi1 = m_i;
    result.hi2 = m_i + m_gap;
    result.compared = true;

    if (arr[m_i] > arr[m_i + m_gap]) {
        std::swap(arr[m_i], arr[m_i + m_gap]);
        result.swapped = true;
        m_swapped_in_pass = true;
    }

    ++m_i;

    if (m_i >= m_size - m_gap && m_gap == 1 && !m_swapped_in_pass) {
        m_done = true;
        result.done = true;
    }

    return result;
}
