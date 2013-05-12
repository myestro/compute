//---------------------------------------------------------------------------//
// Copyright (c) 2013 Kyle Lutz <kyle.r.lutz@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// See http://kylelutz.github.com/compute for more information.
//---------------------------------------------------------------------------//

#define BOOST_TEST_MODULE TestTuple
#include <boost/test/unit_test.hpp>

#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>
#include <boost/tuple/tuple_comparison.hpp>

#include <boost/compute/tuple.hpp>
#include <boost/compute/algorithm/copy.hpp>
#include <boost/compute/algorithm/fill.hpp>
#include <boost/compute/algorithm/find.hpp>
#include <boost/compute/algorithm/transform.hpp>
#include <boost/compute/container/vector.hpp>

#include "context_setup.hpp"

namespace compute = boost::compute;

BOOST_AUTO_TEST_CASE(vector_tuple_int_float)
{
    boost::compute::vector<boost::tuple<int, float> > vector;

    vector.push_back(boost::make_tuple(1, 2.1f));
    vector.push_back(boost::make_tuple(2, 3.2f));
    vector.push_back(boost::make_tuple(3, 4.3f));
}

BOOST_AUTO_TEST_CASE(copy_vector_tuple)
{
    // create vector of tuples on device
    boost::compute::vector<boost::tuple<char, int, float> > input(context);
    input.push_back(boost::make_tuple('a', 1, 2.3f));
    input.push_back(boost::make_tuple('c', 3, 4.5f));
    input.push_back(boost::make_tuple('f', 6, 7.8f));

    // copy on device
    boost::compute::vector<boost::tuple<char, int, float> > output(context);

    boost::compute::copy(
        input.begin(),
        input.end(),
        output.begin()
    );

    // copy to host
    std::vector<boost::tuple<char, int, float> > host_output(3);

    boost::compute::copy(
        input.begin(),
        input.end(),
        host_output.begin()
    );

    // check tuple data
    BOOST_CHECK_EQUAL(host_output[0], boost::make_tuple('a', 1, 2.3f));
    BOOST_CHECK_EQUAL(host_output[1], boost::make_tuple('c', 3, 4.5f));
    BOOST_CHECK_EQUAL(host_output[2], boost::make_tuple('f', 6, 7.8f));
}

BOOST_AUTO_TEST_CASE(extract_tuple_elements)
{
    compute::vector<boost::tuple<char, int, float> > vector(context);
    vector.push_back(boost::make_tuple('a', 1, 2.3f));
    vector.push_back(boost::make_tuple('c', 3, 4.5f));
    vector.push_back(boost::make_tuple('f', 6, 7.8f));

    compute::vector<char> chars(3, context);
    compute::transform(
        vector.begin(), vector.end(), chars.begin(), compute::get<0>(), queue
    );

    compute::vector<int> ints(3, context);
    compute::transform(
        vector.begin(), vector.end(), ints.begin(), compute::get<1>(), queue
    );

    compute::vector<float> floats(3, context);
    compute::transform(
        vector.begin(), vector.end(), floats.begin(), compute::get<2>(), queue
    );

    char chars_data[3];
    compute::copy(chars.begin(), chars.end(), chars_data, queue);
    queue.finish();
    BOOST_CHECK_EQUAL(chars_data[0], 'a');
    BOOST_CHECK_EQUAL(chars_data[1], 'c');
    BOOST_CHECK_EQUAL(chars_data[2], 'f');

    int ints_data[3];
    compute::copy(ints.begin(), ints.end(), ints_data, queue);
    queue.finish();
    BOOST_CHECK_EQUAL(ints_data[0], 1);
    BOOST_CHECK_EQUAL(ints_data[1], 3);
    BOOST_CHECK_EQUAL(ints_data[2], 6);

    float floats_data[3];
    compute::copy(floats.begin(), floats.end(), floats_data, queue);
    queue.finish();
    BOOST_CHECK_EQUAL(floats_data[0], 2.3f);
    BOOST_CHECK_EQUAL(floats_data[1], 4.5f);
    BOOST_CHECK_EQUAL(floats_data[2], 7.8f);
}

BOOST_AUTO_TEST_SUITE_END()