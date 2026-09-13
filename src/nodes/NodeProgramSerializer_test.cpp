#include <catch2/catch_all.hpp>

#include "NodeProgramSerializer.hpp"

static std::string get_off_file_path(const std::string& name)
{
  return std::string(OPENSCAD_TEST_DATA_DIR) + "/nodes/" + name;
}

TEST_CASE("NodeProgramSerializer", "[nodes][NodeProgramSerializer]")
{
    SECTION("Node program write to file")
    {
	const NodeProgramSerializer & serializer = NodeProgramSerializerJSON::instance();
	
	fprintf(stderr, "Going to write an example from a file %s\n",
		get_off_file_path(std::string("example.json")).c_str()
	    );
    }
}

