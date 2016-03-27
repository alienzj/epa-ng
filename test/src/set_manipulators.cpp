#include "Epatest.hpp"

#include "src/set_manipulators.hpp"
#include "src/jplace_util.hpp"
#include "src/Range.hpp"

#include <vector>
#include <iostream>

using namespace std;

TEST(set_manipulators, find_collapse_equal_sequences)
{
  // buildup
  MSA msa;
  msa.append(string("T"),   string("AGCTAGCT"));
  msa.append(string("C"),   string("AGCCAGCT"));
  msa.append(string("G"),   string("AGCGAGCT"));
  msa.append(string("A"),   string("AGCAAGCT"));
  msa.append(string("t1"),  string("AGCTAGCT"));
  msa.append(string("c1"),  string("AGCCAGCT"));
  msa.append(string("g1"),  string("AGCGAGCT"));
  msa.append(string("a1"),  string("AGCAAGCT"));
  msa.append(string("t2"),  string("AGCTAGCT"));
  msa.append(string("c2"),  string("AGCCAGCT"));

  // test
  find_collapse_equal_sequences(msa);
  EXPECT_EQ(msa.size(), 4);

  EXPECT_EQ(msa[0].header_list().size(), 3);
  EXPECT_EQ(msa[1].header_list().size(), 3);
  EXPECT_EQ(msa[2].header_list().size(), 2);
  EXPECT_EQ(msa[3].header_list().size(), 2);

  // for (auto &s : msa)
  //   for (auto &head : s.header_list())
  //     cout << head << endl;

}

TEST(set_manipulators, get_valid_range)
{
  string s1("---------GGGCCCGTAT-------");//(9,19)
  string s2("GGGCCCGTAT-------");         //(0,10)
  string s3("-GGGC---CCG-TAT");           //(1,15)

  Range r;
  r = get_valid_range(s1);
  EXPECT_EQ(r.begin, 9);
  EXPECT_EQ(r.span, 10);

  r = get_valid_range(s2);
  EXPECT_EQ(r.begin, 0);
  EXPECT_EQ(r.span, 10);

  r = get_valid_range(s3);
  EXPECT_EQ(r.begin, 1);
  EXPECT_EQ(r.span, 14);
}

TEST(set_manipulators, discard_bottom_x_percent)
{
  // setup
  Sample sample;
  unsigned int s_a = 0, s_b = 1, s_c = 2;
  sample.emplace_back(s_a, 0);
  vector<double> weights_a({0.001,0.23,0.05,0.02,0.4,0.009,0.2,0.09});
  vector<double> weights_b({0.01,0.02,0.005,0.002,0.94,0.003,0.02});
  unsigned int num_expected[3] = {4,4,1};

  for (auto n : weights_a) {
    sample.back().emplace_back(1,-10,0.9,0.9);
    sample.back().back().lwr(n);
  }
  sample.emplace_back(s_b, 0);
  for (auto n : weights_b) {
    sample.back().emplace_back(1,-10,0.9,0.9);
    sample.back().back().lwr(n);
  }
  sample.emplace_back(s_c, 0);
  sample.back().emplace_back(1,-10,0.9,0.9);
  sample.back().back().lwr(1.0);

  // tests
  discard_bottom_x_percent(sample, 0.5);
  int i =0;
  for (auto pq : sample) {
    unsigned int num = 0;
    for (auto p : pq) {
      (void)p;
      num++;
    }
    EXPECT_EQ(num, num_expected[i++]);
  }

}

TEST(set_manipulators, discard_by_accumulated_threshold)
{
  // setup
  Sample sample;
  unsigned int s_a = 0, s_b = 1, s_c = 2;
  sample.emplace_back(s_a, 0);
  vector<double> weights_a({0.001,0.23,0.05,0.02,0.4,0.009,0.2,0.09});
  vector<double> weights_b({0.01,0.02,0.005,0.002,0.94,0.003,0.02});
  unsigned int num_expected[3] = {5,2,1};

  for (auto n : weights_a) {
    sample.back().emplace_back(1,-10,0.9,0.9);
    sample.back().back().lwr(n);
  }
  sample.emplace_back(s_b, 0);
  for (auto n : weights_b) {
    sample.back().emplace_back(1,-10,0.9,0.9);
    sample.back().back().lwr(n);
  }
  sample.emplace_back(s_c, 0);
  sample.back().emplace_back(1,-10,0.9,0.9);
  sample.back().back().lwr(1.0);

  // tests
  discard_by_accumulated_threshold(sample, 0.95);
  int i =0;
  for (auto pq : sample) {
    unsigned int num = 0;
    for (auto p : pq) {
      (void)p;
      num++;
    }
    EXPECT_EQ(num, num_expected[i++]);
  }

  // string inv("blorp");
  // cout << sample_to_jplace_string(sample, inv);

}

TEST(set_manipulators, discard_by_support_threshold)
{
  // setup
  Sample sample;
  unsigned int s_a = 0, s_b = 1, s_c = 2;
  sample.emplace_back(s_a, 0);
  vector<double> weights_a{0.001,0.23,0.05,0.02,0.4,0.009,0.2,0.09};
  vector<double> weights_b{0.01,0.02,0.005,0.002,0.94,0.003,0.02};
  unsigned int num_expected[3] = {6,3,1};

  for (auto n : weights_a) {
    sample.back().emplace_back(1,-10,0.9,0.9);
    sample.back().back().lwr(n);
  }
  sample.emplace_back(s_b, 0);
  for (auto n : weights_b) {
    sample.back().emplace_back(1,-10,0.9,0.9);
    sample.back().back().lwr(n);
  }
  sample.emplace_back(s_c, 0);
  sample.back().emplace_back(1,-10,0.9,0.9);
  sample.back().back().lwr(1.0);

  // tests
  discard_by_support_threshold(sample, 0.01);
  int i =0;
  for (auto pq : sample) {
    unsigned int num = 0;
    for (auto p : pq) {
      (void)p;
      num++;
    }
    EXPECT_EQ(num, num_expected[i++]);
  }

}
