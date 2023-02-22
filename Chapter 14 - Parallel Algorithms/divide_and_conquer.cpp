#include <iostream>
#include <algorithm>
#include <future>
#include <string>

template <class _InputIt, class _OutputIt, class _UnaryOperation>
_OutputIt par_transform(_InputIt __first, _InputIt __last, _OutputIt __result, _UnaryOperation __op, std::size_t chunk) {
    const std::size_t n = static_cast<std::size_t>(std::distance(__first, __last));
    
    if (n <= chunk) {
        std::transform(__first, __last, __result, __op);
        return __result;
    }
    
    const _InputIt __middle_in = std::next(__first, n / 2);
    
    auto future = std::async(std::launch::async, [=, &__op] () {
        par_transform(__first, __middle_in, __result, __op, chunk);
    });
    
    const _OutputIt __middle_out = std::next(__result, n / 2);
    
    par_transform(__middle_in, __last, __middle_out, __op, chunk);
    
    future.wait();
    
    return __result;
}

int main()
{
    std::string s = "hjHFhtretfnbMljLKtyuYHgffreGHFgjWTRtgjH", trans_s(s), par_trans_s(s);
    std::cout << "s:           " << s << "\ntrans_s:     " << trans_s << "\npar_trans_s: " << par_trans_s << "\n\n";
    
    std::transform(s.begin(), s.end(), trans_s.begin(), [] (auto c) { return toupper(c); } );
    std::cout << "s:           " << s << "\ntrans_s:     " << trans_s << "\npar_trans_s: " << par_trans_s << "\n\n";
    
    par_transform(s.begin(), s.end(), par_trans_s.begin(), [] (auto c) { return toupper(c); }, 2);
    std::cout << "s:           " << s << "\ntrans_s:     " << trans_s << "\npar_trans_s: " << par_trans_s << "\n\n";
    
    return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// OUTPUT  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// s:           hjHFhtretfnbMljLKtyuYHgffreGHFgjWTRtgjH
// trans_s:     hjHFhtretfnbMljLKtyuYHgffreGHFgjWTRtgjH
// par_trans_s: hjHFhtretfnbMljLKtyuYHgffreGHFgjWTRtgjH

// s:           hjHFhtretfnbMljLKtyuYHgffreGHFgjWTRtgjH
// trans_s:     HJHFHTRETFNBMLJLKTYUYHGFFREGHFGJWTRTGJH
// par_trans_s: hjHFhtretfnbMljLKtyuYHgffreGHFgjWTRtgjH

// s:           hjHFhtretfnbMljLKtyuYHgffreGHFgjWTRtgjH
// trans_s:     HJHFHTRETFNBMLJLKTYUYHGFFREGHFGJWTRTGJH
// par_trans_s: HJHFHTRETFNBMLJLKTYUYHGFFREGHFGJWTRTGJH

// Program ended with exit code: 0
