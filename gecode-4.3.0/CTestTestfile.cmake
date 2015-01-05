# CMake generated Testfile for 
# Source directory: /home/punyone/CODE/TREMPPI/gecode-4.3.0
# Build directory: /home/punyone/CODE/TREMPPI/gecode-4.3.0
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(test "gecode-test" "-iter" "2" "-test" "Branch::Int::Dense::3" "-test" "Int::Linear::Int::Int::Eq::Bnd::12::4" "-test" "Int::Distinct::Random" "-test" "Int::Arithmetic::Mult::XYZ::Bnd::C" "-test" "Int::Arithmetic::Mult::XYZ::Dom::A" "-test" "Search::BAB::Sol::BalGr::Binary::Binary::Binary::1::1")
