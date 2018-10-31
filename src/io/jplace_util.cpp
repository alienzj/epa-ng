#include "io/jplace_util.hpp"

#include <sstream>

void merge_into(std::ofstream& dest, const std::vector<std::string>& sources)
{
  size_t i = 0;
  for (const auto& file_n : sources)
  {
    std::ifstream file(file_n);
    dest << file.rdbuf();
    dest.clear(); // empty input files silently set failure flags!
    if (++i < sources.size()) {
      dest << ",";
    }
    dest << NEWL;
    file.close();
  }
}

void placement_to_jplace_string(const Placement& p, std::ostream& os)
{
  os << "[" << p.branch_id() << ", ";
  os << p.likelihood() << ", ";
  os << p.lwr() << ", ";
  os << p.distal_length() << ", ";
  os << p.pendant_length() << "]";
}

void pquery_to_jplace_string(const PQuery<Placement>& pquery, std::ostream& os)
{
  os << "    {\"p\": [" << NEWL; // p for pquery

  size_t i = 0;
  for (const auto& place : pquery)
  {
    // individual pquery
    os << "      ";
    placement_to_jplace_string(place, os);
    if (++i < pquery.size()) {
      os << ",";
    }
    os << NEWL;
  }

  // closing bracket for pquery array
  os << "      ]," << NEWL;

  // start of name column
  os <<"    \"n\": [";

  // sequence header
  const auto& header = pquery.header();
  os << "\"" << header.c_str() << "\"";


  os << "]" << NEWL; // close name bracket

  os << "    }";// final bracket

}

void init_jplace_string(const std::string& numbered_newick, std::ostream& os)
{
  os << "{" << NEWL;
  os << "  \"tree\": \"" << numbered_newick << "\"," << NEWL;
  os << "  \"placements\": " << NEWL;
  os << "  [" << NEWL;
}

void finalize_jplace_string(const std::string& invocation, std::ostream& os)
{
  assert(invocation.length() > 0);

  os << "  ]," << NEWL;

  os << "  \"metadata\": {\"invocation\": \"" << invocation << "\"}," << NEWL;

  os << "  \"version\": 3," << NEWL;
  os << "  \"fields\": ";
  os << "[\"edge_num\", \"likelihood\", \"like_weight_ratio\", \"distal_length\"";
  os << ", \"pendant_length\"]" << NEWL;

  os << "}" << NEWL;
}

void sample_to_jplace_string(const Sample<Placement>& sample, std::ostream& os)
{
  size_t i = 0;
  for (const auto& p : sample) {
    pquery_to_jplace_string(p, os);
    if (++i < sample.size()) {
      os << ",";
    }
    os << NEWL;
  }
}

void full_jplace_string(const Sample<Placement>& sample,
                        const std::string& invocation,
                        std::ostream& os)
{
  // tree and other init
  init_jplace_string(sample.newick(), os);

  // actual placements
  sample_to_jplace_string(sample, os);

  // metadata std::string
  finalize_jplace_string(invocation, os);

}
