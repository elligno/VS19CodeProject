 
#include <algorithm>
#include <array>
#include <ranges>
#include <type_traits>
#include <iostream>
#include <numeric>

namespace vsc19 
{
    // Ref: https://victor-istomin.github.io/c-with-crosses/posts/ref-qualifiers/#fn:1
    // std::ranges prohibits the creation of dangling iterators 
    // and provides an owning_view to take ownership of temporaries. 
    int OwningViewAndDangling()
    {
        // return a temporary
        auto get_array_by_value = []
        { return std::array{0, 1, 0, 1}; };

        // Here, we create a temporary array and search for its maximum element. 
        // Since the temporary is destroyed at the end of the full-expression1, 
        // a trivial implementation might have returned an iterator to an element of the destroyed array. 
        // However, std::ranges detects this and returns a special type called std::ranges::dangling. 
        // This approach accomplishes two things:
        // std::ranges::max_element is executed, any side effects are preserved (e.g. stdout output);
        // dangling_iter is impossible to dereference, preventing access to the destroyed array.

        // (1) a search for the max element in the temporary
        auto dangling_iter = std::ranges::max_element(get_array_by_value());
        static_assert(std::is_same_v<std::ranges::dangling, decltype(dangling_iter)>);
        // Compilation error below:
        // no match for 'operator*' (operand type is 'std::ranges::dangling')
        // std::cout << *dangling_iter << '\n';

        // return a owning view (temporary destroyed) take ownership of temporary
        // can now use max_element safely
        auto retOwningVws = std::views::all(get_array_by_value());
        auto iterOk = std::ranges::max_element(retOwningVws); // can dereference??
        // can i do that? can dereference?
        auto isIterOk = std::ranges::max_element(std::views::all(get_array_by_value()));


        // (2) However, the code below works fine:
        // A temporary is valid to be copied from, but the copy but the iterator 
        // to the end of the source range is std::ranges::dangling, which prevents dereferencing.
        std::array<int, std::size(get_array_by_value())> copied;
        auto copy_result = std::ranges::copy(get_array_by_value(), std::begin(copied));
        static_assert(std::is_same_v<std::ranges::dangling, decltype(copy_result.in)>);

        // (3) and this is fine, too:
        auto isOdd = [](int i)
        { return 0 != (i % 2); };

        //owning view (temporary destroyed)
        // The temporary is filtered and should be destroyed at the end of the full-expression, 
        // but the view takes ownership over it using the owning_view. This means that it is perfectly
        // valid to iterate over the filtered range after the original temporary has been destroyed
        auto filtered = get_array_by_value() | std::views::filter(isOdd);
        return std::accumulate(std::begin(filtered), std::end(filtered), 0);
    }
}