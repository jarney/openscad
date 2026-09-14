#include <catch2/catch_all.hpp>
#include <fstream>

#include "NodeProgramSerializer.hpp"
#include "OpenSCADModels.hpp"

static std::string get_test_path(const std::string& name)
{
  return std::string(OPENSCAD_TEST_DATA_DIR) + "/nodes/" + name;
}

TEST_CASE("NodeProgramSerializer", "[nodes][NodeProgramSerializer]")
{
    SECTION("Node program write to file")
    {
	const NodeProgramSerializer & serializer = NodeProgramSerializerJSON::instance();

	NodeProgram loadedProgram(SCADModels::registerDataModels());
	fprintf(stderr, "Going to write an example from a file %s\n",
		get_test_path(std::string("example.json")).c_str()
	    );
	std::ifstream exampleInputFile(get_test_path("example.json"));
	std::ofstream exampleOutputFile(get_test_path("example2.json"));
	serializer.read(loadedProgram, exampleInputFile);
	serializer.write(loadedProgram, exampleOutputFile);
    }
}

